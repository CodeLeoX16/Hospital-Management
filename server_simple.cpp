#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Simple data structures
struct Patient {
    int id;
    string name;
    string gender;
    int age;
    string address;
    string phone;
    string email;
};

struct Doctor {
    int id;
    string name;
    string speciality;
    string address;
    string phone;
    string email;
};

struct Appointment {
    int id;
    string patientName;
    string doctorName;
    string date;
    string time;
};

// Global storage
vector<Patient> patients;
vector<Doctor> doctors;
vector<Appointment> appointments;
int patientID = 0, doctorID = 0, appointmentID = 0;

// Simple JSON builder
string buildJSON(string key, string value) {
    return "\"" + key + "\":\"" + value + "\"";
}

string buildJSONInt(string key, int value) {
    return "\"" + key + "\":" + to_string(value);
}

// Simple JSON response builder
string buildResponse(bool success, string message) {
    string response = "{\"success\":" + string(success ? "true" : "false") + ",";
    response += "\"message\":\"" + message + "\"}";
    return response;
}

string buildHTTPResponse(string jsonBody) {
    string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response += "Content-Length: " + to_string(jsonBody.length()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += jsonBody;
    return response;
}

// Extract JSON values (simple parsing)
string extractValue(string json, string key) {
    string searchStr = "\"" + key + "\":\"";
    size_t pos = json.find(searchStr);
    if (pos == string::npos) return "";
    pos += searchStr.length();
    size_t endPos = json.find("\"", pos);
    return json.substr(pos, endPos - pos);
}

int extractIntValue(string json, string key) {
    string searchStr = "\"" + key + "\":";
    size_t pos = json.find(searchStr);
    if (pos == string::npos) return 0;
    pos += searchStr.length();
    size_t endPos = json.find(",", pos);
    if (endPos == string::npos) endPos = json.find("}", pos);
    return stoi(json.substr(pos, endPos - pos));
}

// API Handlers
string handleAddPatient(string body) {
    string name = extractValue(body, "name");
    string gender = extractValue(body, "gender");
    int age = extractIntValue(body, "age");
    string address = extractValue(body, "address");
    string phone = extractValue(body, "phone");
    string email = extractValue(body, "email");

    Patient p;
    p.id = patientID++;
    p.name = name;
    p.gender = gender;
    p.age = age;
    p.address = address;
    p.phone = phone;
    p.email = email;
    
    patients.push_back(p);
    
    return buildResponse(true, "Patient added successfully");
}

string handleAddDoctor(string body) {
    string name = extractValue(body, "name");
    string speciality = extractValue(body, "speciality");
    string address = extractValue(body, "address");
    string phone = extractValue(body, "phone");
    string email = extractValue(body, "email");

    Doctor d;
    d.id = doctorID++;
    d.name = name;
    d.speciality = speciality;
    d.address = address;
    d.phone = phone;
    d.email = email;
    
    doctors.push_back(d);
    
    return buildResponse(true, "Doctor added successfully");
}

string handleScheduleAppointment(string body) {
    int patientId = extractIntValue(body, "patientId");
    int doctorId = extractIntValue(body, "doctorId");
    string date = extractValue(body, "date");
    string time = extractValue(body, "time");

    if (patientId >= patients.size() || doctorId >= doctors.size()) {
        return buildResponse(false, "Invalid Patient or Doctor ID");
    }

    Appointment a;
    a.id = appointmentID++;
    a.patientName = patients[patientId].name;
    a.doctorName = doctors[doctorId].name;
    a.date = date;
    a.time = time;
    
    appointments.push_back(a);
    
    return buildResponse(true, "Appointment scheduled successfully");
}

string handleGetPatients() {
    string response = "{\"success\":true,\"message\":\"Patients retrieved\",\"data\":[";
    
    for (int i = 0; i < patients.size(); i++) {
        response += "{" + buildJSONInt("id", patients[i].id) + ",";
        response += buildJSON("name", patients[i].name) + ",";
        response += buildJSON("gender", patients[i].gender) + ",";
        response += buildJSONInt("age", patients[i].age) + ",";
        response += buildJSON("address", patients[i].address) + ",";
        response += buildJSON("phone", patients[i].phone) + ",";
        response += buildJSON("email", patients[i].email) + "}";
        if (i < patients.size() - 1) response += ",";
    }
    
    response += "]}";
    return response;
}

string handleGetDoctors() {
    string response = "{\"success\":true,\"message\":\"Doctors retrieved\",\"data\":[";
    
    for (int i = 0; i < doctors.size(); i++) {
        response += "{" + buildJSONInt("id", doctors[i].id) + ",";
        response += buildJSON("name", doctors[i].name) + ",";
        response += buildJSON("speciality", doctors[i].speciality) + ",";
        response += buildJSON("address", doctors[i].address) + ",";
        response += buildJSON("phone", doctors[i].phone) + ",";
        response += buildJSON("email", doctors[i].email) + "}";
        if (i < doctors.size() - 1) response += ",";
    }
    
    response += "]}";
    return response;
}

string handleGetAppointments() {
    string response = "{\"success\":true,\"message\":\"Appointments retrieved\",\"data\":[";
    
    for (int i = 0; i < appointments.size(); i++) {
        response += "{" + buildJSONInt("id", appointments[i].id) + ",";
        response += buildJSON("patientName", appointments[i].patientName) + ",";
        response += buildJSON("doctorName", appointments[i].doctorName) + ",";
        response += buildJSON("date", appointments[i].date) + ",";
        response += buildJSON("time", appointments[i].time) + "}";
        if (i < appointments.size() - 1) response += ",";
    }
    
    response += "]}";
    return response;
}

// Process HTTP request
string processRequest(string request) {
    // Extract request body
    size_t bodyStart = request.find("\r\n\r\n");
    string body = "";
    if (bodyStart != string::npos) {
        body = request.substr(bodyStart + 4);
    }

    // Route requests
    if (request.find("POST /api/patient") != string::npos) {
        return buildHTTPResponse(handleAddPatient(body));
    }
    else if (request.find("POST /api/doctor") != string::npos) {
        return buildHTTPResponse(handleAddDoctor(body));
    }
    else if (request.find("POST /api/appointment") != string::npos) {
        return buildHTTPResponse(handleScheduleAppointment(body));
    }
    else if (request.find("GET /api/patients") != string::npos) {
        return buildHTTPResponse(handleGetPatients());
    }
    else if (request.find("GET /api/doctors") != string::npos) {
        return buildHTTPResponse(handleGetDoctors());
    }
    else if (request.find("GET /api/appointments") != string::npos) {
        return buildHTTPResponse(handleGetAppointments());
    }
    else if (request.find("OPTIONS") != string::npos) {
        string response = "HTTP/1.1 200 OK\r\n";
        response += "Access-Control-Allow-Origin: *\r\n";
        response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
        response += "Access-Control-Allow-Headers: Content-Type\r\n";
        response += "Content-Length: 0\r\n\r\n";
        return response;
    }
    
    return buildHTTPResponse(buildResponse(false, "Endpoint not found"));
}

// Handle client connection
void handleClient(SOCKET clientSocket) {
    char buffer[4096] = { 0 };
    int received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (received > 0) {
        buffer[received] = '\0';
        string request(buffer);
        string response = processRequest(request);
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    
    closesocket(clientSocket);
}

// Main server loop
int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
    
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "socket failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    
    // Bind socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "================================================" << endl;
    cout << "Hospital Management System - Server Started" << endl;
    cout << "================================================" << endl;
    cout << "Server running on http://127.0.0.1:8080" << endl;
    cout << "Open index.html in your browser to access the web interface" << endl;
    cout << "Press Ctrl+C to stop the server" << endl;
    cout << "================================================" << endl << endl;
    
    // Accept connections
    while (true) {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        
        if (clientSocket != INVALID_SOCKET) {
            cout << "[" << inet_ntoa(clientAddr.sin_addr) << "] Connected" << endl;
            
            // Handle client synchronously (no threading needed)
            handleClient(clientSocket);
        }
    }
    
    closesocket(serverSocket);
    WSACleanup();
    
    return 0;
}
