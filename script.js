// Configuration
const API_BASE_URL = 'https://hospital-mgmt-system-backend.onrender.com';
let recentActivities = [];
let allPatients = [];
let allDoctors = [];
let allAppointments = [];

// Tab switching
document.querySelectorAll('.nav-btn').forEach(btn => {
    btn.addEventListener('click', function() {
        const tabName = this.getAttribute('data-tab');
        switchTab(tabName);
    });
});

function switchTab(tabName) {
    // Hide all tabs
    document.querySelectorAll('.tab-content').forEach(tab => {
        tab.classList.remove('active');
    });
    
    // Remove active class from all buttons
    document.querySelectorAll('.nav-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    
    // Show selected tab
    document.getElementById(tabName).classList.add('active');
    
    // Add active class to clicked button
    event.target.classList.add('active');
    
    // Load data when switching to tabs
    if (tabName === 'patients') {
        loadPatients();
    } else if (tabName === 'doctors') {
        loadDoctors();
    } else if (tabName === 'appointments') {
        loadAppointments();
        loadPatientDropdown();
        loadDoctorDropdown();
    } else if (tabName === 'dashboard') {
        updateDashboard();
    }
}

// Form submissions
document.getElementById('patientForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const patientData = {
        name: document.getElementById('patientName').value,
        gender: document.getElementById('patientGender').value,
        age: parseInt(document.getElementById('patientAge').value),
        address: document.getElementById('patientAddress').value,
        phone: document.getElementById('patientPhone').value,
        email: document.getElementById('patientEmail').value
    };
    
    console.log('Sending patient data:', patientData);
    
    try {
        const response = await fetch(`${API_BASE_URL}/api/patient`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(patientData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showNotification('Patient added successfully!', 'success');
            document.getElementById('patientForm').reset();
            loadPatients();
            addActivity(`Added new patient: ${patientData.name}`);
        } else {
            showNotification('Error adding patient: ' + result.message, 'error');
        }
    } catch (error) {
        console.error('Error:', error);
        showNotification('Failed to add patient: ' + error.message, 'error');
    }
});

document.getElementById('doctorForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const doctorData = {
        name: document.getElementById('doctorName').value,
        speciality: document.getElementById('doctorSpeciality').value,
        address: document.getElementById('doctorAddress').value,
        phone: document.getElementById('doctorPhone').value,
        email: document.getElementById('doctorEmail').value
    };
    
    try {
        const response = await fetch(`${API_BASE_URL}/api/doctor`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(doctorData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showNotification('Doctor added successfully!', 'success');
            document.getElementById('doctorForm').reset();
            loadDoctors();
            addActivity(`Added new doctor: ${doctorData.name}`);
        } else {
            showNotification('Error adding doctor: ' + result.message, 'error');
        }
    } catch (error) {
        console.error('Error:', error);
        showNotification('Failed to add doctor', 'error');
    }
});

document.getElementById('appointmentForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const appointmentData = {
        patientId: parseInt(document.getElementById('appointmentPatient').value),
        doctorId: parseInt(document.getElementById('appointmentDoctor').value),
        date: document.getElementById('appointmentDate').value,
        time: document.getElementById('appointmentTime').value
    };
    
    try {
        const response = await fetch(`${API_BASE_URL}/api/appointment`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(appointmentData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showNotification('Appointment scheduled successfully!', 'success');
            document.getElementById('appointmentForm').reset();
            loadAppointments();
            addActivity('Scheduled new appointment');
        } else {
            showNotification('Error scheduling appointment: ' + result.message, 'error');
        }
    } catch (error) {
        console.error('Error:', error);
        showNotification('Failed to schedule appointment', 'error');
    }
});

// (rest of your script.js code remains unchanged)
