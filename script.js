// API Base URL
const API_BASE_URL = 'https://hospital-management2.onrender.com/api';

// DOM Elements
const loginForm = document.getElementById('loginForm');
const dashboardSection = document.getElementById('dashboard');
const loginSection = document.getElementById('login');
const patientsList = document.getElementById('patientsList');
const appointmentsList = document.getElementById('appointmentsList');
const doctorsList = document.getElementById('doctorsList');
const addPatientBtn = document.getElementById('addPatientBtn');
const addAppointmentBtn = document.getElementById('addAppointmentBtn');
const addDoctorBtn = document.getElementById('addDoctorBtn');
const logoutBtn = document.getElementById('logoutBtn');

// Authentication Token
let authToken = null;

// Login Handler
loginForm.addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    try {
        const response = await fetch(`${API_BASE_URL}/auth/login`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ username, password }),
        });

        if (response.ok) {
            const data = await response.json();
            authToken = data.token;
            loginSection.style.display = 'none';
            dashboardSection.style.display = 'block';
            loadDashboardData();
        } else {
            alert('Login failed. Please check your credentials.');
        }
    } catch (error) {
        console.error('Login error:', error);
        alert('An error occurred during login.');
    }
});

// Logout Handler
logoutBtn.addEventListener('click', () => {
    authToken = null;
    dashboardSection.style.display = 'none';
    loginSection.style.display = 'block';
    loginForm.reset();
});

// Load Dashboard Data
async function loadDashboardData() {
    await loadPatients();
    await loadAppointments();
    await loadDoctors();
}

// Load Patients
async function loadPatients() {
    try {
        const response = await fetch(`${API_BASE_URL}/patients`, {
            headers: {
                'Authorization': `Bearer ${authToken}`,
            },
        });

        if (response.ok) {
            const patients = await response.json();
            displayPatients(patients);
        } else {
            console.error('Failed to load patients');
        }
    } catch (error) {
        console.error('Error loading patients:', error);
    }
}

// Display Patients
function displayPatients(patients) {
    patientsList.innerHTML = '';
    patients.forEach(patient => {
        const patientCard = document.createElement('div');
        patientCard.className = 'card';
        patientCard.innerHTML = `
            <h3>${patient.name}</h3>
            <p>Age: ${patient.age}</p>
            <p>Gender: ${patient.gender}</p>
            <p>Contact: ${patient.contact}</p>
            <button onclick="editPatient(${patient.id})">Edit</button>
            <button onclick="deletePatient(${patient.id})">Delete</button>
        `;
        patientsList.appendChild(patientCard);
    });
}

// Load Appointments
async function loadAppointments() {
    try {
        const response = await fetch(`${API_BASE_URL}/appointments`, {
            headers: {
                'Authorization': `Bearer ${authToken}`,
            },
        });

        if (response.ok) {
            const appointments = await response.json();
            displayAppointments(appointments);
        } else {
            console.error('Failed to load appointments');
        }
    } catch (error) {
        console.error('Error loading appointments:', error);
    }
}

// Display Appointments
function displayAppointments(appointments) {
    appointmentsList.innerHTML = '';
    appointments.forEach(appointment => {
        const appointmentCard = document.createElement('div');
        appointmentCard.className = 'card';
        appointmentCard.innerHTML = `
            <h3>Appointment #${appointment.id}</h3>
            <p>Patient: ${appointment.patientName}</p>
            <p>Doctor: ${appointment.doctorName}</p>
            <p>Date: ${new Date(appointment.dateTime).toLocaleString()}</p>
            <p>Status: ${appointment.status}</p>
            <button onclick="editAppointment(${appointment.id})">Edit</button>
            <button onclick="deleteAppointment(${appointment.id})">Delete</button>
        `;
        appointmentsList.appendChild(appointmentCard);
    });
}

// Load Doctors
async function loadDoctors() {
    try {
        const response = await fetch(`${API_BASE_URL}/doctors`, {
            headers: {
                'Authorization': `Bearer ${authToken}`,
            },
        });

        if (response.ok) {
            const doctors = await response.json();
            displayDoctors(doctors);
        } else {
            console.error('Failed to load doctors');
        }
    } catch (error) {
        console.error('Error loading doctors:', error);
    }
}

// Display Doctors
function displayDoctors(doctors) {
    doctorsList.innerHTML = '';
    doctors.forEach(doctor => {
        const doctorCard = document.createElement('div');
        doctorCard.className = 'card';
        doctorCard.innerHTML = `
            <h3>Dr. ${doctor.name}</h3>
            <p>Specialization: ${doctor.specialization}</p>
            <p>Contact: ${doctor.contact}</p>
            <button onclick="editDoctor(${doctor.id})">Edit</button>
            <button onclick="deleteDoctor(${doctor.id})">Delete</button>
        `;
        doctorsList.appendChild(doctorCard);
    });
}

// Add Patient
addPatientBtn.addEventListener('click', async () => {
    const name = prompt('Enter patient name:');
    const age = prompt('Enter patient age:');
    const gender = prompt('Enter patient gender:');
    const contact = prompt('Enter patient contact:');

    if (name && age && gender && contact) {
        try {
            const response = await fetch(`${API_BASE_URL}/patients`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${authToken}`,
                },
                body: JSON.stringify({ name, age: parseInt(age), gender, contact }),
            });

            if (response.ok) {
                alert('Patient added successfully');
                loadPatients();
            } else {
                alert('Failed to add patient');
            }
        } catch (error) {
            console.error('Error adding patient:', error);
            alert('An error occurred while adding patient');
        }
    }
});

// Add Appointment
addAppointmentBtn.addEventListener('click', async () => {
    const patientId = prompt('Enter patient ID:');
    const doctorId = prompt('Enter doctor ID:');
    const dateTime = prompt('Enter date and time (YYYY-MM-DDTHH:mm):');

    if (patientId && doctorId && dateTime) {
        try {
            const response = await fetch(`${API_BASE_URL}/appointments`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${authToken}`,
                },
                body: JSON.stringify({
                    patientId: parseInt(patientId),
                    doctorId: parseInt(doctorId),
                    dateTime,
                    status: 'SCHEDULED'
                }),
            });

            if (response.ok) {
                alert('Appointment added successfully');
                loadAppointments();
            } else {
                alert('Failed to add appointment');
            }
        } catch (error) {
            console.error('Error adding appointment:', error);
            alert('An error occurred while adding appointment');
        }
    }
});

// Add Doctor
addDoctorBtn.addEventListener('click', async () => {
    const name = prompt('Enter doctor name:');
    const specialization = prompt('Enter specialization:');
    const contact = prompt('Enter contact:');

    if (name && specialization && contact) {
        try {
            const response = await fetch(`${API_BASE_URL}/doctors`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${authToken}`,
                },
                body: JSON.stringify({ name, specialization, contact }),
            });

            if (response.ok) {
                alert('Doctor added successfully');
                loadDoctors();
            } else {
                alert('Failed to add doctor');
            }
        } catch (error) {
            console.error('Error adding doctor:', error);
            alert('An error occurred while adding doctor');
        }
    }
});

// Delete Patient
async function deletePatient(id) {
    if (confirm('Are you sure you want to delete this patient?')) {
        try {
            const response = await fetch(`${API_BASE_URL}/patients/${id}`, {
                method: 'DELETE',
                headers: {
                    'Authorization': `Bearer ${authToken}`,
                },
            });

            if (response.ok) {
                alert('Patient deleted successfully');
                loadPatients();
            } else {
                alert('Failed to delete patient');
            }
        } catch (error) {
            console.error('Error deleting patient:', error);
            alert('An error occurred while deleting patient');
        }
    }
}

// Delete Appointment
async function deleteAppointment(id) {
    if (confirm('Are you sure you want to delete this appointment?')) {
        try {
            const response = await fetch(`${API_BASE_URL}/appointments/${id}`, {
                method: 'DELETE',
                headers: {
                    'Authorization': `Bearer ${authToken}`,
                },
            });

            if (response.ok) {
                alert('Appointment deleted successfully');
                loadAppointments();
            } else {
                alert('Failed to delete appointment');
            }
        } catch (error) {
            console.error('Error deleting appointment:', error);
            alert('An error occurred while deleting appointment');
        }
    }
}

// Delete Doctor
async function deleteDoctor(id) {
    if (confirm('Are you sure you want to delete this doctor?')) {
        try {
            const response = await fetch(`${API_BASE_URL}/doctors/${id}`, {
                method: 'DELETE',
                headers: {
                    'Authorization': `Bearer ${authToken}`,
                },
            });

            if (response.ok) {
                alert('Doctor deleted successfully');
                loadDoctors();
            } else {
                alert('Failed to delete doctor');
            }
        } catch (error) {
            console.error('Error deleting doctor:', error);
            alert('An error occurred while deleting doctor');
        }
    }
}

// Edit functions (placeholders)
function editPatient(id) {
    alert('Edit patient functionality to be implemented');
}

function editAppointment(id) {
    alert('Edit appointment functionality to be implemented');
}

function editDoctor(id) {
    alert('Edit doctor functionality to be implemented');
}
