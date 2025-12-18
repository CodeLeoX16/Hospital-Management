# Hospital Management System - Full Stack Application

## Project Overview
A complete Hospital Management System with:
- **Backend**: C++ HTTP Server (RESTful API)
- **Frontend**: Modern Web Interface (HTML, CSS, JavaScript)
- **Real-world Features**: Patient Management, Doctor Management, Appointment Scheduling

## Architecture

```
┌─────────────────────────────┐
│   Web Frontend (Browser)    │
│  HTML | CSS | JavaScript    │
└──────────────┬──────────────┘
               │ HTTP Requests
               ▼
┌─────────────────────────────┐
│   C++ HTTP Server           │
│   Port: 8080                │
│   REST API Endpoints        │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│   In-Memory Data Storage    │
│   - Patients                │
│   - Doctors                 │
│   - Appointments            │
└─────────────────────────────┘
```

## File Structure

```
hospital management system/
├── server.cpp              # C++ backend server
├── code.cpp                # Original console application
├── index.html              # Web frontend
├── style.css               # Styling
├── script.js               # Frontend logic
└── README.md               # This file
```

## Prerequisites

### For Backend (C++ Server)
- Visual Studio or MinGW (C++ compiler)
- Windows OS (uses Winsock2)
- jsoncpp library (for JSON handling)

### For Frontend
- Any modern web browser (Chrome, Firefox, Edge, Safari)
- No additional installation needed

## Installation & Setup

### Step 1: Install Dependencies (Windows)

#### Using vcpkg (Recommended)
```powershell
# Clone vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\vcpkg integrate install

# Install jsoncpp
.\vcpkg install jsoncpp:x64-windows
```

#### Manual Setup
Download jsoncpp from: https://github.com/open-source-parsers/jsoncpp

### Step 2: Compile C++ Server

**Using Visual Studio:**
1. Open Visual Studio
2. Create new C++ Console Project
3. Add the `server.cpp` file
4. Include jsoncpp headers and libraries in project properties
5. Build the project

**Using g++ (MinGW):**
```bash
g++ -o server server.cpp -lwsock2 -ljsoncpp
```

### Step 3: Run the Server

```bash
# Navigate to project directory
cd "hospital management system"

# Run the compiled server
./server.exe
# or
server.exe
```

You should see:
```
Server running on http://127.0.0.1:8080
```

### Step 4: Open Web Interface

1. Open your web browser
2. Navigate to: `http://localhost:8000` (or open index.html directly)
3. The web interface should load with a beautiful dashboard

**Note:** If you want to serve the HTML from a web server:
```bash
# Using Python 3
python -m http.server 8000

# Using Python 2
python -m SimpleHTTPServer 8000

# Using Node.js (if installed)
npx http-server
```

## API Endpoints

### Patient Management
- **POST** `/api/patient` - Add new patient
- **GET** `/api/patients` - Get all patients

### Doctor Management
- **POST** `/api/doctor` - Add new doctor
- **GET** `/api/doctors` - Get all doctors

### Appointment Management
- **POST** `/api/appointment` - Schedule appointment
- **GET** `/api/appointments` - Get all appointments

### Request/Response Examples

#### Add Patient
**Request:**
```json
POST /api/patient
{
  "name": "John Doe",
  "gender": "Male",
  "age": 30,
  "address": "123 Main St",
  "phone": "555-1234",
  "email": "john@example.com"
}
```

**Response:**
```json
{
  "success": true,
  "message": "Patient added successfully",
  "data": {
    "id": 0,
    "name": "John Doe",
    "gender": "Male",
    "age": 30,
    "address": "123 Main St",
    "phone": "555-1234",
    "email": "john@example.com"
  }
}
```

## Features

### Dashboard
- Real-time statistics
- Recent activity log
- System overview

### Patients Module
- Add new patients
- View patient list
- Store patient details (name, gender, age, address, phone, email)

### Doctors Module
- Register doctors
- View doctor list
- Track specializations
- Store contact information

### Appointments Module
- Schedule appointments
- Link patients with doctors
- Set appointment date and time
- View all appointments

## User Interface Features

✨ **Modern Design**
- Gradient-based color scheme
- Responsive layout
- Smooth animations and transitions
- Card-based information display

📱 **Responsive Design**
- Works on desktop, tablet, and mobile devices
- Adaptive navigation
- Mobile-friendly forms

⚡ **Real-time Updates**
- Auto-refresh data every 30 seconds
- Live notifications
- Instant form validation

## Troubleshooting

### Server won't start
- Check if port 8080 is already in use
- Ensure jsoncpp is properly installed
- Run as Administrator if needed

### Frontend shows "Failed to connect"
- Ensure the C++ server is running
- Check that server is listening on port 8080
- Verify firewall isn't blocking port 8080
- Check browser console for error messages

### CORS Errors
- The server includes CORS headers
- If issues persist, check browser console

## Technical Details

### Backend Architecture
- **Language**: C++ 11+
- **Protocol**: HTTP/1.1
- **Port**: 8080
- **Concurrency**: Multi-threaded (one thread per client)
- **Data Storage**: In-memory vectors

### Frontend Stack
- **HTML5** - Semantic markup
- **CSS3** - Flexbox and Grid layouts
- **JavaScript (ES6+)** - Dynamic interactions
- **Fetch API** - HTTP requests

## Future Enhancements

- [ ] Database integration (SQLite/MySQL)
- [ ] User authentication & authorization
- [ ] Data persistence
- [ ] Email notifications
- [ ] Doctor availability scheduling
- [ ] Patient medical records
- [ ] Admin dashboard
- [ ] Mobile app
- [ ] Billing system
- [ ] Real-time notifications

## Performance Considerations

- Current implementation stores data in-memory
- All data is lost when server restarts
- For production, implement database storage
- Consider load balancing for multiple servers

## Security Notes

⚠️ **Current Limitations:**
- No authentication/authorization
- No input validation beyond basic checks
- No HTTPS/SSL
- No rate limiting
- Not suitable for production without security enhancements

**Before Production Deployment:**
1. Add user authentication
2. Implement input validation
3. Use HTTPS/SSL
4. Add database with backups
5. Implement proper error handling
6. Add logging and monitoring
7. Conduct security audit

## Usage Example

### Adding a Patient:
1. Click "Patients" tab
2. Fill in the form with patient details
3. Click "Add Patient"
4. Notification confirms success
5. Patient appears in the list

### Scheduling an Appointment:
1. Click "Appointments" tab
2. Select a patient from dropdown
3. Select a doctor from dropdown
4. Choose appointment date and time
5. Click "Schedule Appointment"
6. Confirmation appears

## Support & Debugging

### Enable Console Debugging
Open browser Developer Tools (F12) and check:
- Console tab for JavaScript errors
- Network tab to inspect API calls
- Response bodies in the Network tab

### Check Server Logs
The server prints debug information to console:
- Connection received
- Request endpoints
- Processing status

## License
This is an educational project. Feel free to modify and use for learning purposes.

## Author
Hospital Management System - Full Stack Implementation

## Contact & Support
For issues or questions, refer to the code comments and console output for debugging information.

---

**Version:** 1.0  
**Last Updated:** December 2025  
**Status:** Ready for Development/Learning
