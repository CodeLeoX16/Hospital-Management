// Configuration
const API_BASE_URL = 'http://localhost:8080/api';
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
    console.log('API URL:', `${API_BASE_URL}/patient`);
    
    try {
        const response = await fetch(`${API_BASE_URL}/patient`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(patientData)
        });
        
        console.log('Response status:', response.status);
        console.log('Response ok:', response.ok);
        
        const result = await response.json();
        
        console.log('Response result:', result);
        
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
        console.error('Error message:', error.message);
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
        const response = await fetch(`${API_BASE_URL}/doctor`, {
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
        const response = await fetch(`${API_BASE_URL}/appointment`, {
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
            addActivity(`Scheduled appointment for ${allPatients[appointmentData.patientId]?.name || 'patient'}`);
        } else {
            showNotification('Error scheduling appointment: ' + result.message, 'error');
        }
    } catch (error) {
        console.error('Error:', error);
        showNotification('Failed to schedule appointment', 'error');
    }
});

// Load data functions
async function loadPatients() {
    try {
        const response = await fetch(`${API_BASE_URL}/patients`);
        const result = await response.json();
        
        if (result.success) {
            allPatients = result.data;
            renderPatients(result.data);
        }
    } catch (error) {
        console.error('Error loading patients:', error);
        document.getElementById('patientList').innerHTML = '<p class="empty-state">Failed to load patients</p>';
    }
}

async function loadDoctors() {
    try {
        const response = await fetch(`${API_BASE_URL}/doctors`);
        const result = await response.json();
        
        if (result.success) {
            allDoctors = result.data;
            renderDoctors(result.data);
        }
    } catch (error) {
        console.error('Error loading doctors:', error);
        document.getElementById('doctorList').innerHTML = '<p class="empty-state">Failed to load doctors</p>';
    }
}

async function loadAppointments() {
    try {
        const response = await fetch(`${API_BASE_URL}/appointments`);
        const result = await response.json();
        
        if (result.success) {
            allAppointments = result.data;
            renderAppointments(result.data);
        }
    } catch (error) {
        console.error('Error loading appointments:', error);
        document.getElementById('appointmentList').innerHTML = '<p class="empty-state">Failed to load appointments</p>';
    }
}

async function loadPatientDropdown() {
    try {
        const response = await fetch(`${API_BASE_URL}/patients`);
        const result = await response.json();
        
        if (result.success && result.data.length > 0) {
            const select = document.getElementById('appointmentPatient');
            select.innerHTML = '<option value="">Select Patient</option>';
            result.data.forEach(patient => {
                const option = document.createElement('option');
                option.value = patient.id;
                option.textContent = `${patient.name} (ID: ${patient.id})`;
                select.appendChild(option);
            });
        }
    } catch (error) {
        console.error('Error loading patient dropdown:', error);
    }
}

async function loadDoctorDropdown() {
    try {
        const response = await fetch(`${API_BASE_URL}/doctors`);
        const result = await response.json();
        
        if (result.success && result.data.length > 0) {
            const select = document.getElementById('appointmentDoctor');
            select.innerHTML = '<option value="">Select Doctor</option>';
            result.data.forEach(doctor => {
                const option = document.createElement('option');
                option.value = doctor.id;
                option.textContent = `${doctor.name} - ${doctor.speciality} (ID: ${doctor.id})`;
                select.appendChild(option);
            });
        }
    } catch (error) {
        console.error('Error loading doctor dropdown:', error);
    }
}

// Render functions
function renderPatients(patients) {
    const container = document.getElementById('patientList');
    
    if (patients.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No patients found</p></div>';
        return;
    }
    
    container.innerHTML = patients.map(patient => `
        <div class="data-card">
            <div class="card-header">
                <div class="card-title">${patient.name}</div>
                <div class="card-id">ID: ${patient.id}</div>
            </div>
            <div class="card-content">
                <div class="info-row">
                    <span class="info-label">Gender:</span>
                    <span class="info-value">${patient.gender}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Age:</span>
                    <span class="info-value">${patient.age} years</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Address:</span>
                    <span class="info-value">${patient.address}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Phone:</span>
                    <span class="info-value">${patient.phone}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Email:</span>
                    <span class="info-value">${patient.email}</span>
                </div>
            </div>
        </div>
    `).join('');
}

function renderDoctors(doctors) {
    const container = document.getElementById('doctorList');
    
    if (doctors.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No doctors found</p></div>';
        return;
    }
    
    container.innerHTML = doctors.map(doctor => `
        <div class="data-card">
            <div class="card-header">
                <div class="card-title">${doctor.name}</div>
                <div class="card-id">ID: ${doctor.id}</div>
            </div>
            <div class="card-content">
                <div class="info-row">
                    <span class="info-label">Speciality:</span>
                    <span class="info-value">${doctor.speciality}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Address:</span>
                    <span class="info-value">${doctor.address}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Phone:</span>
                    <span class="info-value">${doctor.phone}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Email:</span>
                    <span class="info-value">${doctor.email}</span>
                </div>
            </div>
        </div>
    `).join('');
}

function renderAppointments(appointments) {
    const container = document.getElementById('appointmentList');
    
    if (appointments.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No appointments found</p></div>';
        return;
    }
    
    container.innerHTML = appointments.map(appointment => `
        <div class="data-card">
            <div class="card-header">
                <div class="card-title">Appointment #${appointment.id}</div>
            </div>
            <div class="card-content">
                <div class="info-row">
                    <span class="info-label">Patient:</span>
                    <span class="info-value">${appointment.patientName}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Doctor:</span>
                    <span class="info-value">${appointment.doctorName}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Date:</span>
                    <span class="info-value">${appointment.date}</span>
                </div>
                <div class="info-row">
                    <span class="info-label">Time:</span>
                    <span class="info-value">${appointment.time}</span>
                </div>
            </div>
        </div>
    `).join('');
}

// Dashboard update
async function updateDashboard() {
    try {
        const [patientsRes, doctorsRes, appointmentsRes] = await Promise.all([
            fetch(`${API_BASE_URL}/patients`),
            fetch(`${API_BASE_URL}/doctors`),
            fetch(`${API_BASE_URL}/appointments`)
        ]);
        
        const patients = await patientsRes.json();
        const doctors = await doctorsRes.json();
        const appointments = await appointmentsRes.json();
        
        document.getElementById('patientCount').textContent = patients.data?.length || 0;
        document.getElementById('doctorCount').textContent = doctors.data?.length || 0;
        document.getElementById('appointmentCount').textContent = appointments.data?.length || 0;
        
        renderRecentActivity();
    } catch (error) {
        console.error('Error updating dashboard:', error);
    }
}

function renderRecentActivity() {
    const container = document.getElementById('recentActivity');
    
    if (recentActivities.length === 0) {
        container.innerHTML = '<p class="empty-state">No recent activity</p>';
        return;
    }
    
    container.innerHTML = recentActivities.slice(-5).reverse().map(activity => `
        <div class="activity-item">📋 ${activity}</div>
    `).join('');
}

function addActivity(message) {
    const timestamp = new Date().toLocaleTimeString();
    recentActivities.push(`${message} - ${timestamp}`);
    if (recentActivities.length > 20) {
        recentActivities.shift();
    }
}

// Notification system
function showNotification(message, type = 'info') {
    const notification = document.getElementById('notification');
    notification.textContent = message;
    notification.className = `notification show ${type}`;
    
    setTimeout(() => {
        notification.classList.add('hide');
        setTimeout(() => {
            notification.classList.remove('show', 'hide');
        }, 300);
    }, 3000);
}

// Initialize on page load
window.addEventListener('load', () => {
    updateDashboard();
    showNotification('Welcome to Hospital Management System!', 'info');
    addActivity('System started');
});

// Auto-refresh data every 30 seconds
setInterval(() => {
    if (document.getElementById('patients').classList.contains('active')) {
        loadPatients();
    } else if (document.getElementById('doctors').classList.contains('active')) {
        loadDoctors();
    } else if (document.getElementById('appointments').classList.contains('active')) {
        loadAppointments();
    }
}, 30000);
