// API Configuration
const API_BASE_URL = 'https://hospital-management2.onrender.com/api';

// State management
let patients = [];
let doctors = [];
let appointments = [];

// Initialize the application
document.addEventListener('DOMContentLoaded', () => {
    initializeTabs();
    initializeForms();
    loadAllData();
    updateDashboard();
});

// Tab Switching Functionality
function initializeTabs() {
    const tabButtons = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');

    tabButtons.forEach(button => {
        button.addEventListener('click', () => {
            const tabName = button.dataset.tab;

            // Remove active class from all buttons and contents
            tabButtons.forEach(btn => btn.classList.remove('active'));
            tabContents.forEach(content => content.classList.remove('active'));

            // Add active class to clicked button and corresponding content
            button.classList.add('active');
            document.getElementById(tabName).classList.add('active');
        });
    });
}

// Form Initialization and Event Listeners
function initializeForms() {
    // Patient Form
    const patientForm = document.getElementById('patientForm');
    if (patientForm) {
        patientForm.addEventListener('submit', async (e) => {
            e.preventDefault();
            await handlePatientSubmit(e);
        });
    }

    // Doctor Form
    const doctorForm = document.getElementById('doctorForm');
    if (doctorForm) {
        doctorForm.addEventListener('submit', async (e) => {
            e.preventDefault();
            await handleDoctorSubmit(e);
        });
    }

    // Appointment Form
    const appointmentForm = document.getElementById('appointmentForm');
    if (appointmentForm) {
        appointmentForm.addEventListener('submit', async (e) => {
            e.preventDefault();
            await handleAppointmentSubmit(e);
        });
    }
}

// Load All Data
async function loadAllData() {
    await Promise.all([
        loadPatients(),
        loadDoctors(),
        loadAppointments()
    ]);
    updateDashboard();
}

// Patient Management
async function loadPatients() {
    try {
        const response = await fetch(`${API_BASE_URL}/patients`);
        if (response.ok) {
            patients = await response.json();
            renderPatients();
            updatePatientSelect();
        } else {
            showNotification('Failed to load patients', 'error');
        }
    } catch (error) {
        console.error('Error loading patients:', error);
        showNotification('Error loading patients', 'error');
    }
}

async function handlePatientSubmit(e) {
    const formData = new FormData(e.target);
    const patientData = {
        name: formData.get('patientName'),
        age: parseInt(formData.get('patientAge')),
        gender: formData.get('patientGender'),
        contact: formData.get('patientContact'),
        address: formData.get('patientAddress')
    };

    try {
        const response = await fetch(`${API_BASE_URL}/patients`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(patientData)
        });

        if (response.ok) {
            showNotification('Patient added successfully', 'success');
            e.target.reset();
            await loadPatients();
            updateDashboard();
        } else {
            const error = await response.json();
            showNotification(error.message || 'Failed to add patient', 'error');
        }
    } catch (error) {
        console.error('Error adding patient:', error);
        showNotification('Error adding patient', 'error');
    }
}

function renderPatients() {
    const patientList = document.getElementById('patientList');
    if (!patientList) return;

    if (patients.length === 0) {
        patientList.innerHTML = '<p class="no-data">No patients found</p>';
        return;
    }

    patientList.innerHTML = patients.map(patient => `
        <div class="card">
            <h3>${patient.name}</h3>
            <p><strong>Age:</strong> ${patient.age}</p>
            <p><strong>Gender:</strong> ${patient.gender}</p>
            <p><strong>Contact:</strong> ${patient.contact}</p>
            <p><strong>Address:</strong> ${patient.address}</p>
            <button class="btn btn-danger" onclick="deletePatient('${patient._id}')">Delete</button>
        </div>
    `).join('');
}

async function deletePatient(id) {
    if (!confirm('Are you sure you want to delete this patient?')) return;

    try {
        const response = await fetch(`${API_BASE_URL}/patients/${id}`, {
            method: 'DELETE'
        });

        if (response.ok) {
            showNotification('Patient deleted successfully', 'success');
            await loadPatients();
            updateDashboard();
        } else {
            showNotification('Failed to delete patient', 'error');
        }
    } catch (error) {
        console.error('Error deleting patient:', error);
        showNotification('Error deleting patient', 'error');
    }
}

function updatePatientSelect() {
    const patientSelect = document.getElementById('appointmentPatient');
    if (!patientSelect) return;

    patientSelect.innerHTML = '<option value="">Select Patient</option>' +
        patients.map(patient => `
            <option value="${patient._id}">${patient.name}</option>
        `).join('');
}

// Doctor Management
async function loadDoctors() {
    try {
        const response = await fetch(`${API_BASE_URL}/doctors`);
        if (response.ok) {
            doctors = await response.json();
            renderDoctors();
            updateDoctorSelect();
        } else {
            showNotification('Failed to load doctors', 'error');
        }
    } catch (error) {
        console.error('Error loading doctors:', error);
        showNotification('Error loading doctors', 'error');
    }
}

async function handleDoctorSubmit(e) {
    const formData = new FormData(e.target);
    const doctorData = {
        name: formData.get('doctorName'),
        specialization: formData.get('doctorSpecialization'),
        contact: formData.get('doctorContact'),
        email: formData.get('doctorEmail')
    };

    try {
        const response = await fetch(`${API_BASE_URL}/doctors`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(doctorData)
        });

        if (response.ok) {
            showNotification('Doctor added successfully', 'success');
            e.target.reset();
            await loadDoctors();
            updateDashboard();
        } else {
            const error = await response.json();
            showNotification(error.message || 'Failed to add doctor', 'error');
        }
    } catch (error) {
        console.error('Error adding doctor:', error);
        showNotification('Error adding doctor', 'error');
    }
}

function renderDoctors() {
    const doctorList = document.getElementById('doctorList');
    if (!doctorList) return;

    if (doctors.length === 0) {
        doctorList.innerHTML = '<p class="no-data">No doctors found</p>';
        return;
    }

    doctorList.innerHTML = doctors.map(doctor => `
        <div class="card">
            <h3>${doctor.name}</h3>
            <p><strong>Specialization:</strong> ${doctor.specialization}</p>
            <p><strong>Contact:</strong> ${doctor.contact}</p>
            <p><strong>Email:</strong> ${doctor.email}</p>
            <button class="btn btn-danger" onclick="deleteDoctor('${doctor._id}')">Delete</button>
        </div>
    `).join('');
}

async function deleteDoctor(id) {
    if (!confirm('Are you sure you want to delete this doctor?')) return;

    try {
        const response = await fetch(`${API_BASE_URL}/doctors/${id}`, {
            method: 'DELETE'
        });

        if (response.ok) {
            showNotification('Doctor deleted successfully', 'success');
            await loadDoctors();
            updateDashboard();
        } else {
            showNotification('Failed to delete doctor', 'error');
        }
    } catch (error) {
        console.error('Error deleting doctor:', error);
        showNotification('Error deleting doctor', 'error');
    }
}

function updateDoctorSelect() {
    const doctorSelect = document.getElementById('appointmentDoctor');
    if (!doctorSelect) return;

    doctorSelect.innerHTML = '<option value="">Select Doctor</option>' +
        doctors.map(doctor => `
            <option value="${doctor._id}">${doctor.name} - ${doctor.specialization}</option>
        `).join('');
}

// Appointment Management
async function loadAppointments() {
    try {
        const response = await fetch(`${API_BASE_URL}/appointments`);
        if (response.ok) {
            appointments = await response.json();
            renderAppointments();
        } else {
            showNotification('Failed to load appointments', 'error');
        }
    } catch (error) {
        console.error('Error loading appointments:', error);
        showNotification('Error loading appointments', 'error');
    }
}

async function handleAppointmentSubmit(e) {
    const formData = new FormData(e.target);
    const appointmentData = {
        patientId: formData.get('appointmentPatient'),
        doctorId: formData.get('appointmentDoctor'),
        date: formData.get('appointmentDate'),
        time: formData.get('appointmentTime'),
        reason: formData.get('appointmentReason')
    };

    try {
        const response = await fetch(`${API_BASE_URL}/appointments`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(appointmentData)
        });

        if (response.ok) {
            showNotification('Appointment created successfully', 'success');
            e.target.reset();
            await loadAppointments();
            updateDashboard();
        } else {
            const error = await response.json();
            showNotification(error.message || 'Failed to create appointment', 'error');
        }
    } catch (error) {
        console.error('Error creating appointment:', error);
        showNotification('Error creating appointment', 'error');
    }
}

function renderAppointments() {
    const appointmentList = document.getElementById('appointmentList');
    if (!appointmentList) return;

    if (appointments.length === 0) {
        appointmentList.innerHTML = '<p class="no-data">No appointments found</p>';
        return;
    }

    appointmentList.innerHTML = appointments.map(appointment => {
        const patient = patients.find(p => p._id === appointment.patientId);
        const doctor = doctors.find(d => d._id === appointment.doctorId);
        const date = new Date(appointment.date).toLocaleDateString();

        return `
            <div class="card">
                <h3>Appointment</h3>
                <p><strong>Patient:</strong> ${patient ? patient.name : 'Unknown'}</p>
                <p><strong>Doctor:</strong> ${doctor ? doctor.name : 'Unknown'}</p>
                <p><strong>Date:</strong> ${date}</p>
                <p><strong>Time:</strong> ${appointment.time}</p>
                <p><strong>Reason:</strong> ${appointment.reason}</p>
                <p><strong>Status:</strong> <span class="status-${appointment.status}">${appointment.status}</span></p>
                <button class="btn btn-danger" onclick="deleteAppointment('${appointment._id}')">Delete</button>
            </div>
        `;
    }).join('');
}

async function deleteAppointment(id) {
    if (!confirm('Are you sure you want to delete this appointment?')) return;

    try {
        const response = await fetch(`${API_BASE_URL}/appointments/${id}`, {
            method: 'DELETE'
        });

        if (response.ok) {
            showNotification('Appointment deleted successfully', 'success');
            await loadAppointments();
            updateDashboard();
        } else {
            showNotification('Failed to delete appointment', 'error');
        }
    } catch (error) {
        console.error('Error deleting appointment:', error);
        showNotification('Error deleting appointment', 'error');
    }
}

// Dashboard Update
function updateDashboard() {
    const totalPatientsEl = document.getElementById('totalPatients');
    const totalDoctorsEl = document.getElementById('totalDoctors');
    const totalAppointmentsEl = document.getElementById('totalAppointments');

    if (totalPatientsEl) totalPatientsEl.textContent = patients.length;
    if (totalDoctorsEl) totalDoctorsEl.textContent = doctors.length;
    if (totalAppointmentsEl) totalAppointmentsEl.textContent = appointments.length;
}

// Notification System
function showNotification(message, type = 'info') {
    // Remove any existing notifications
    const existingNotification = document.querySelector('.notification');
    if (existingNotification) {
        existingNotification.remove();
    }

    // Create notification element
    const notification = document.createElement('div');
    notification.className = `notification notification-${type}`;
    notification.textContent = message;

    // Add to document
    document.body.appendChild(notification);

    // Trigger animation
    setTimeout(() => {
        notification.classList.add('show');
    }, 10);

    // Remove after 3 seconds
    setTimeout(() => {
        notification.classList.remove('show');
        setTimeout(() => {
            notification.remove();
        }, 300);
    }, 3000);
}

// Make delete functions globally accessible
window.deletePatient = deletePatient;
window.deleteDoctor = deleteDoctor;
window.deleteAppointment = deleteAppointment;
