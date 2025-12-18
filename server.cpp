#include<iostream>
#include<vector>
#include<limits>
#include<sstream>
#include<fstream>
#include<winsock2.h>
#include<thread>
#include<json/json.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Global variables
int patientID = 0;
int doctorID = 0;
int appointmentID = 0;

// Patient class
class Patient {
public:
    string name;
    string gender;
    int age;
    int ID;
    string address;
    string phone;
    string email;
    
    Patient(string n, string g, int ag, string ad, string ph, string e) {
        ID = patientID++;
        name = n;
        gender = g;
        age = ag;
        address = ad;
        phone = ph;
        email = e;
    }
    
    Json::Value toJson() {
        Json::Value json;
        json["id"] = ID;
        json["name"] = name;
        json["gender"] = gender;
        json["age"] = age;
        json["address"] = address;
        json["phone"] = phone;
        json["email"] = email;
        return json;
    }
};

// Doctor class
class Doctor {
public:
    int ID;
    string name;
    string address;
    string phone;
    string email;
    string speciality;
    
    Doctor(string n, string ad, string ph, string e, string s) {
        ID = doctorID++;
        name = n;
        address = ad;
        phone = ph;
        email = e;
        speciality = s;
    }
    
    Json::Value toJson() {
        Json::Value json;
        json["id"] = ID;
        json["name"] = name;
        json["address"] = address;
        json["phone"] = phone;
        json["email"] = email;
        json["speciality"] = speciality;
        return json;
    }
};

// Appointment class
class Appointment {
public:
    int ID;
    string patientName;
    string doctorName;
    string date;
    string time;
    
    Appointment() {
        ID = appointmentID++;
    }
    
    Json::Value toJson() {
        Json::Value json;
        json["id"] = ID;
        json["patientName"] = patientName;
        json["doctorName"] = doctorName;
        json["date"] = date;
        json["time"] = time;
        return json;
    }
};

// Global storage
vector<Patient> allPatients;
vector<Doctor> allDoctors;
vector<Appointment> allAppointments;

// API Response builder
string buildJsonResponse(bool success, string message, Json::Value data = Json::Value()) {
    Json::Value response;
    response["success"] = success;
    response["message"] = message;
    response["data"] = data;
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

// API Endpoints
string handleAddPatient(string body) {
    try {
        Json::Value root;
        Json::CharReaderBuilder builder;
        string errs;
        istringstream s(body);
        if (!Json::parseFromStream(builder, s, &root, &errs)) {
            return buildJsonResponse(false, "Invalid JSON");
        }
        
        string name = root["name"].asString();
        string gender = root["gender"].asString();
        int age = root["age"].asInt();
        string address = root["address"].asString();
        string phone = root["phone"].asString();
        string email = root["email"].asString();
        
        Patient p(name, gender, age, address, phone, email);
        allPatients.push_back(p);
        
        return buildJsonResponse(true, "Patient added successfully", p.toJson());
    } catch (exception& e) {
        return buildJsonResponse(false, string(e.what()));
    }
}

string handleAddDoctor(string body) {
    try {
        Json::Value root;
        Json::CharReaderBuilder builder;
        string errs;
        istringstream s(body);
        if (!Json::parseFromStream(builder, s, &root, &errs)) {
            return buildJsonResponse(false, "Invalid JSON");
        }
        
        string name = root["name"].asString();
        string address = root["address"].asString();
        string phone = root["phone"].asString();
        string email = root["email"].asString();
        string speciality = root["speciality"].asString();
        
        Doctor d(name, address, phone, email, speciality);
        allDoctors.push_back(d);
        
        return buildJsonResponse(true, "Doctor added successfully", d.toJson());
    } catch (exception& e) {
        return buildJsonResponse(false, string(e.what()));
    }
}

string handleScheduleAppointment(string body) {
    try {
        Json::Value root;
        Json::CharReaderBuilder builder;
        string errs;
        istringstream s(body);
        if (!Json::parseFromStream(builder, s, &root, &errs)) {
            return buildJsonResponse(false, "Invalid JSON");
        }
        
        int patientID = root["patientId"].asInt();
        int doctorID = root["doctorId"].asInt();
        string date = root["date"].asString();
        string time = root["time"].asString();
        
        if (patientID < 0 || patientID >= allPatients.size()) {
            return buildJsonResponse(false, "Invalid Patient ID");
        }
        if (doctorID < 0 || doctorID >= allDoctors.size()) {
            return buildJsonResponse(false, "Invalid Doctor ID");
        }
        
        Appointment a;
        a.patientName = allPatients[patientID].name;
        a.doctorName = allDoctors[doctorID].name;
        a.date = date;
        a.time = time;
        
        allAppointments.push_back(a);
        
        return buildJsonResponse(true, "Appointment scheduled successfully", a.toJson());
    } catch (exception& e) {
        return buildJsonResponse(false, string(e.what()));
    }
}

string handleGetPatients() {
    Json::Value patients(Json::arrayValue);
    for (auto& p : allPatients) {
        patients.append(p.toJson());
    }
    return buildJsonResponse(true, "Patients retrieved", patients);
}

string handleGetDoctors() {
    Json::Value doctors(Json::arrayValue);
    for (auto& d : allDoctors) {
        doctors.append(d.toJson());
    }
    return buildJsonResponse(true, "Doctors retrieved", doctors);
}

string handleGetAppointments() {
    Json::Value appointments(Json::arrayValue);
    for (auto& a : allAppointments) {
        appointments.append(a.toJson());
    }
    return buildJsonResponse(true, "Appointments retrieved", appointments);
}

// HTTP Server
class SimpleHTTPServer {
public:
    int port;
    SOCKET serverSocket;
    
    SimpleHTTPServer(int p) : port(p), serverSocket(INVALID_SOCKET) {}
    
    bool start() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cout << "WSAStartup failed" << endl;
            return false;
        }
        
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            cout << "Socket creation failed" << endl;
            WSACleanup();
            return false;
        }
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddr.sin_port = htons(port);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cout << "Bind failed" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }
        
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            cout << "Listen failed" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }
        
        cout << "Server running on http://127.0.0.1:" << port << endl;
        return true;
    }
    
    void acceptConnections() {
        while (true) {
            sockaddr_in clientAddr;
            int addrLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
            
            if (clientSocket != INVALID_SOCKET) {
                thread(&SimpleHTTPServer::handleClient, this, clientSocket).detach();
            }
        }
    }
    
    void handleClient(SOCKET clientSocket) {
        char buffer[4096] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        if (bytesReceived > 0) {
            string request(buffer);
            string response = processRequest(request);
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        closesocket(clientSocket);
    }
    
    string processRequest(string request) {
        string response;
        
        if (request.find("POST /api/patient") != string::npos) {
            string body = request.substr(request.find("\r\n\r\n") + 4);
            response = handleAddPatient(body);
        }
        else if (request.find("POST /api/doctor") != string::npos) {
            string body = request.substr(request.find("\r\n\r\n") + 4);
            response = handleAddDoctor(body);
        }
        else if (request.find("POST /api/appointment") != string::npos) {
            string body = request.substr(request.find("\r\n\r\n") + 4);
            response = handleScheduleAppointment(body);
        }
        else if (request.find("GET /api/patients") != string::npos) {
            response = handleGetPatients();
        }
        else if (request.find("GET /api/doctors") != string::npos) {
            response = handleGetDoctors();
        }
        else if (request.find("GET /api/appointments") != string::npos) {
            response = handleGetAppointments();
        }
        else {
            response = buildJsonResponse(false, "Endpoint not found");
        }
        
        string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: application/json\r\n";
        httpResponse += "Access-Control-Allow-Origin: *\r\n";
        httpResponse += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
        httpResponse += "Content-Length: " + to_string(response.length()) + "\r\n";
        httpResponse += "\r\n";
        httpResponse += response;
        
        return httpResponse;
    }
    
    void stop() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        WSACleanup();
    }
};

int main() {
    SimpleHTTPServer server(8080);
    
    if (!server.start()) {
        return 1;
    }
    
    server.acceptConnections();
    server.stop();
    
    return 0;
}
