// Process HTTP request
string processRequest(string request) {
    std::cout << "======== RAW REQUEST START ========" << std::endl;
    std::cout << request << std::endl;
    std::cout << "======== RAW REQUEST END ==========" << std::endl;

    // Extract request body
    size_t bodyStart = request.find("\r\n\r\n");
    string body = "";
    if (bodyStart != string::npos) {
        body = request.substr(bodyStart + 4);
    }

    // ... rest of your routing logic ...
}
