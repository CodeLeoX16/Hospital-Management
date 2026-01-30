// Simple HTTP server for demo hospital management API
// This file implements lightweight in-memory data models (Patient, Doctor,
// Appointment), small JSON helpers, request handlers (add/get/schedule), and
// a minimal Windows TCP HTTP server wrapper `SimpleHTTPServer`.
// Usage:
//   server.exe --port 8080 --addr 127.0.0.1
//   server.exe -p 8080
// Build (MinGW/g++ on Windows):
//   g++ server.cpp -lws2_32 -std=c++11 -O2 -o server.exe

#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using std::string;
using std::cout;
using std::endl;

static int nextPatientId = 0;
static int nextDoctorId = 0;
static int nextAppointmentId = 0;

// Represents a patient record stored in memory.
// Fields are simple types; `toJson()` serializes to a compact JSON object.
struct Patient {
    int id;
    string name, gender, address, phone, email;
    int age;
    // Convert the `Patient` instance into a compact JSON string.
    // Used by API responses.
    string toJson() const {
        std::ostringstream o;
        o << "{\"id\":" << id
          << ",\"name\":\"" << escapeJson(name) << "\""
          << ",\"gender\":\"" << escapeJson(gender) << "\""
          << ",\"age\":" << age
          << ",\"address\":\"" << escapeJson(address) << "\""
          << ",\"phone\":\"" << escapeJson(phone) << "\""
          << ",\"email\":\"" << escapeJson(email) << "\"}";
        return o.str();
    }
    // Escape characters that would break JSON strings.
    // This is a small helper used by `toJson()` implementations.
    static string escapeJson(const string &s) {
        string r; r.reserve(s.size()*2);
        for (char c: s) {
            if (c == '\\') r += "\\\\";
            else if (c == '"') r += "\\\"";
            else if (c == '\n') r += "\\n";
            else if (c == '\r') r += "\\r";
            else r += c;
        }
        return r;
    }
};

// Represents a doctor record stored in memory.
// `toJson()` serializes the doctor into JSON for API responses.
struct Doctor {
    int id;
    string name, address, phone, email, speciality;
    string toJson() const {
        std::ostringstream o;
        o << "{\"id\":" << id
          << ",\"name\":\"" << Patient::escapeJson(name) << "\""
          << ",\"address\":\"" << Patient::escapeJson(address) << "\""
          << ",\"phone\":\"" << Patient::escapeJson(phone) << "\""
          << ",\"email\":\"" << Patient::escapeJson(email) << "\""
          << ",\"speciality\":\"" << Patient::escapeJson(speciality) << "\"}";
        return o.str();
    }
};

// Represents an appointment linking a patient and a doctor.
// Contains human-readable `patientName` and `doctorName` for simplicity.
struct Appointment {
    int id;
    string patientName, doctorName, date, time;
    string toJson() const {
        std::ostringstream o;
        o << "{\"id\":" << id
          << ",\"patientName\":\"" << Patient::escapeJson(patientName) << "\""
          << ",\"doctorName\":\"" << Patient::escapeJson(doctorName) << "\""
          << ",\"date\":\"" << Patient::escapeJson(date) << "\""
          << ",\"time\":\"" << Patient::escapeJson(time) << "\"}";
        return o.str();
    }
};

// In-memory storage for the running server process. These vectors are
// appended to by POST handlers and read by GET handlers.
static std::vector<Patient> patients;
static std::vector<Doctor> doctors;
static std::vector<Appointment> appointments;

// Build a simple JSON response: {"success":true,"message":"...","data":...}
// Build a standard API response JSON object. `dataJson` is inserted as-is
// (may be a JSON array/object or the literal "null").
static string buildJsonResponse(bool success, const string &message, const string &dataJson = "null") {
        std::ostringstream o;
        o << "{\"success\":" << (success ? "true" : "false")
            << ",\"message\":\"" << Patient::escapeJson(message) << "\""
            << ",\"data\":" << dataJson << "}";
        return o.str();
}

// Very small, naive JSON extractors. These are intentionally minimal and
// only suitable for the simple JSON payloads the demo uses (no nested
// objects or escaping beyond basic cases).
static string extractJsonString(const string &body, const string &key) {
    string pat = '"' + key + '"';
    size_t p = body.find(pat);
    if (p == string::npos) return string();
    p = body.find(':', p);
    if (p == string::npos) return string();
    p++;
    while (p < body.size() && isspace((unsigned char)body[p])) p++;
    if (p < body.size() && body[p] == '"') {
        p++;
        size_t q = p;
        while (q < body.size() && body[q] != '"') q++;
        return body.substr(p, q - p);
    }
    // no quotes: read until comma or brace
    size_t q = p;
    while (q < body.size() && body[q] != ',' && body[q] != '}') q++;
    string val = body.substr(p, q - p);
    // trim
    while (!val.empty() && isspace((unsigned char)val.front())) val.erase(val.begin());
    while (!val.empty() && isspace((unsigned char)val.back())) val.pop_back();
    return val;
}

// Extract an integer value from a flat JSON object. Returns `def` on
// parse/lookup failure.
static int extractJsonInt(const string &body, const string &key, int def = 0) {
    string s = extractJsonString(body, key);
    if (s.empty()) return def;
    try { return std::stoi(s); } catch(...) { return def; }
}

// Handler: POST /api/patient
// Parses the request body for patient fields, assigns an `id`, stores the
// record in `patients`, and returns a success JSON response containing the
// serialized patient object.
static string handleAddPatient(const string &body) {
    Patient p;
    p.id = nextPatientId++;
    p.name = extractJsonString(body, "name");
    p.gender = extractJsonString(body, "gender");
    p.age = extractJsonInt(body, "age", 0);
    p.address = extractJsonString(body, "address");
    p.phone = extractJsonString(body, "phone");
    p.email = extractJsonString(body, "email");
    patients.push_back(p);
    return buildJsonResponse(true, "Patient added", p.toJson());
}

// Handler: POST /api/doctor
// Reads doctor fields from the JSON body, assigns an `id`, stores the
// doctor in `doctors`, and returns the created doctor as JSON.
static string handleAddDoctor(const string &body) {
    Doctor d;
    d.id = nextDoctorId++;
    d.name = extractJsonString(body, "name");
    d.address = extractJsonString(body, "address");
    d.phone = extractJsonString(body, "phone");
    d.email = extractJsonString(body, "email");
    d.speciality = extractJsonString(body, "speciality");
    doctors.push_back(d);
    return buildJsonResponse(true, "Doctor added", d.toJson());
}

// Handler: POST /api/appointment
// Expects `patientId` and `doctorId` (indexes into the vectors). On
// success it creates an `Appointment`, stores it, and returns the record.
static string handleScheduleAppointment(const string &body) {
    int pid = extractJsonInt(body, "patientId", -1);
    int did = extractJsonInt(body, "doctorId", -1);
    if (pid < 0 || pid >= (int)patients.size()) return buildJsonResponse(false, "Invalid patientId");
    if (did < 0 || did >= (int)doctors.size()) return buildJsonResponse(false, "Invalid doctorId");
    Appointment a;
    a.id = nextAppointmentId++;
    a.patientName = patients[pid].name;
    a.doctorName = doctors[did].name;
    a.date = extractJsonString(body, "date");
    a.time = extractJsonString(body, "time");
    appointments.push_back(a);
    return buildJsonResponse(true, "Appointment scheduled", a.toJson());
}

// Handler: GET /api/patients
// Serializes the entire `patients` list as a JSON array inside the
// standard API response envelope.
static string handleGetPatients() {
    std::ostringstream o; o << "[";
    for (size_t i = 0; i < patients.size(); ++i) {
        if (i) o << ",";
        o << patients[i].toJson();
    }
    o << "]";
    return buildJsonResponse(true, "Patients list", o.str());
}

// Handler: GET /api/doctors
// Serializes the `doctors` list into the API response.
static string handleGetDoctors() {
    std::ostringstream o; o << "[";
    for (size_t i = 0; i < doctors.size(); ++i) {
        if (i) o << ",";
        o << doctors[i].toJson();
    }
    o << "]";
    return buildJsonResponse(true, "Doctors list", o.str());
}

// Handler: GET /api/appointments
// Serializes all scheduled appointments and returns them as JSON.
static string handleGetAppointments() {
    std::ostringstream o; o << "[";
    for (size_t i = 0; i < appointments.size(); ++i) {
        if (i) o << ",";
        o << appointments[i].toJson();
    }
    o << "]";
    return buildJsonResponse(true, "Appointments list", o.str());
}

// Arguments passed to a new client thread: pointer to `SimpleHTTPServer`
// instance and the accepted client socket.
struct ClientThreadArgs {
    class SimpleHTTPServer *self;
    SOCKET client;
};

// Thread procedure used to dispatch each accepted client connection to
// `SimpleHTTPServer::handleClient` in a new thread. Frees the argument
// structure when done.
static DWORD WINAPI clientThreadProc(LPVOID lp) {
    ClientThreadArgs *a = (ClientThreadArgs*)lp;
    if (a) {
        a->self->handleClient(a->client);
        delete a;
    }
    return 0;
}

// Minimal Windows-based HTTP server wrapper. Usage:
//   SimpleHTTPServer server("127.0.0.1", 8080);
//   server.start();
//   server.acceptLoop();
// The class handles Winsock startup, binding, listening, and dispatching
// each accepted connection to `handleClient` on a dedicated thread.
class SimpleHTTPServer {
public:
    string addr;
    int port;
    SOCKET serverSocket = INVALID_SOCKET;
    SimpleHTTPServer(const string &a, int p): addr(a), port(p) {}
    // Initialize Winsock, create/bind/listen on the server socket.
    // Returns true on success.
    bool start() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) return false;
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) { WSACleanup(); return false; }
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
        serverAddr.sin_port = htons((unsigned short)port);
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { closesocket(serverSocket); WSACleanup(); return false; }
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) { closesocket(serverSocket); WSACleanup(); return false; }
        cout << "Server listening on http://" << addr << ":" << port << endl;
        return true;
    }
    // Accept loop: blocks waiting for new connections and launches a
    // thread for each client.
    void acceptLoop() {
        while (true) {
            sockaddr_in clientAddr{};
            int len = sizeof(clientAddr);
            SOCKET client = accept(serverSocket, (sockaddr*)&clientAddr, &len);
            if (client != INVALID_SOCKET) {
                ClientThreadArgs *args = new ClientThreadArgs;
                args->self = this;
                args->client = client;
                HANDLE th = CreateThread(NULL, 0, clientThreadProc, args, 0, NULL);
                if (th) CloseHandle(th);
            }
        }
    }
    // Handle a single client request on the calling thread. Reads the
    // incoming HTTP request (headers + optional body), routes to the
    // appropriate handler, and writes back a JSON HTTP response.
    void handleClient(SOCKET client) {
        char buf[8192];
        int r = recv(client, buf, sizeof(buf), 0);
        if (r > 0) {
                        string req(buf, r);
                        string body;
                        size_t pos = req.find("\r\n\r\n");
                        if (pos != string::npos) body = req.substr(pos + 4);

                        // Parse request line (e.g. "POST /api/patient HTTP/1.1")
                        size_t lineEnd = req.find("\r\n");
                        string method, path;
                        if (lineEnd != string::npos) {
                                std::istringstream line(req.substr(0, lineEnd));
                                line >> method >> path; // ignore HTTP version
                        }

                        // Normalize path: strip query string and trailing slash (but keep "/")
                        size_t q = path.find('?'); if (q != string::npos) path = path.substr(0, q);
                        if (path.size() > 1 && path.back() == '/') path.pop_back();

                        // Handle CORS preflight
                        if (method == "OPTIONS" && path.rfind("/api/", 0) == 0) {
                                std::ostringstream h;
                                h << "HTTP/1.1 200 OK\r\n"
                                    << "Content-Type: text/plain\r\n"
                                    << "Access-Control-Allow-Origin: *\r\n"
                                    << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                                    << "Access-Control-Allow-Headers: Content-Type\r\n"
                                    << "Content-Length: 0\r\n\r\n";
                                string header = h.str();
                                send(client, header.c_str(), (int)header.size(), 0);
                                closesocket(client);
                                return;
                        }

                        string respBody;
                        // Route by method + normalized path
                        if (method == "POST" && (path == "/api/patient" || path == "/api/patients")) respBody = handleAddPatient(body);
                        else if (method == "POST" && (path == "/api/doctor" || path == "/api/doctors")) respBody = handleAddDoctor(body);
                        else if (method == "POST" && (path == "/api/appointment" || path == "/api/appointments")) respBody = handleScheduleAppointment(body);
                        else if (method == "GET" && (path == "/api/patients" || path == "/api/patient")) respBody = handleGetPatients();
                        else if (method == "GET" && (path == "/api/doctors" || path == "/api/doctor")) respBody = handleGetDoctors();
                        else if (method == "GET" && (path == "/api/appointments" || path == "/api/appointment")) respBody = handleGetAppointments();
                        else respBody = buildJsonResponse(false, "Endpoint not found");

                        std::ostringstream h;
                        h << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: application/json\r\n"
                            << "Access-Control-Allow-Origin: *\r\n"
                            << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                            << "Access-Control-Allow-Headers: Content-Type\r\n"
                            << "Content-Length: " << respBody.size() << "\r\n\r\n";
                        string header = h.str();
                        send(client, header.c_str(), (int)header.size(), 0);
                        send(client, respBody.c_str(), (int)respBody.size(), 0);
        }
        closesocket(client);
    }
    // Stop the server and clean up Winsock resources.
    void stop() {
        if (serverSocket != INVALID_SOCKET) closesocket(serverSocket);
        WSACleanup();
    }
};

// Print a short usage message for the original command-line entry point.
static void printUsage(const char *exe) {
    cout << "Usage: " << exe << " [--port N] [--addr ADDR] [--help]" << endl;
    cout << " Default port: 8080  Default addr: 127.0.0.1" << endl;
}

// `main` removed: this file now exposes `SimpleHTTPServer` and handlers
// to be used by a separate application entry point.
