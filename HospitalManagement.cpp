#include<iostream>
#include<vector>
#include<limits>

using namespace std;
//globval variables to make unik id and to store all patients,doctors and appointments and increase it for each patient,doctor and 
// appointment
int patientID=0000;
int doctorID=0000;
int appointmentID=0000000;
//class for patient,doctor and appointment
class Patient{
    public:
    string name;
    string Gender;
    int age;
    int ID;
    string address;
    string phone;
    string email;
    Patient(string n,string G,int ag,string ad,string ph,string e){
        ID=patientID++;
        name=n;
        Gender=G;
        age=ag;
        address=ad;
        phone=ph;
        email=e;


    }
    
};
class Doctor{
    public:
    int ID;
    string name;
    string address;
    string phone;
    string email;
    string speciality;
    Doctor(string n,string ad,string ph,string e,string s){
        ID=doctorID++;
        name=n;
        address=ad;
        phone=ph;
        email=e;
        speciality=s;
    }


};
class Appointment{
    public:
    string patientName;
    string doctorName;
    string date;
    string time;
};
vector<Patient> allPatients;
vector<Doctor> allDoctors;
vector<Appointment> allAppointments;
void addPatient(){
    string n;
    string G;
    int ag;
    string ad;
    string ph;
    string e;

    // Patient temp(n,G,ag,ad,ph,e);
    cout<<"Please Enter Patitent Nmae:"<<endl;
    cin>>n;
    cout<<"Enter Gender:"<<endl;
    cin>>G;
    cout<<"Enter age:"<<endl;
    cin>>ag;
    cout<<"Enter Address:"<<endl;
    cin>>ad;
    cout<<"Enter PhoneNumber:"<<endl;
    cin>>ph;
    cout<<"Enter Email:"<<endl;
    cin>>e;
    Patient temp(n,G,ag,ad,ph,e);
    allPatients.push_back(temp);

}
void addDoctor(){
    string n;
    string ad;
    string ph;
    string e;
    string s;
    cout<<"Please Enter Doctor Nmae:"<<endl;
    cin>>n;
    cout<<"Enter Address:"<<endl;
    cin>>ad;
    cout<<"Enter PhoneNumber:"<<endl;
    cin>>ph;
    cout<<"Enter Email:"<<endl;
    cin>>e;
    cout<<"Enter Speciality:"<<endl;
    cin>>s;
    Doctor temp(n,ad,ph,e,s);
    allDoctors.push_back(temp);


}
void scheduleAppointment() {
    int pID;
    int dID;
    string date;
    string time;
    cout << "Schedule Appointment" << endl;
    cout << "Enter Patient ID:" << endl;
    cin >> pID;
    cout << "Enter Doctor ID:" << endl;
    cin >> dID;
    cout << "Enter Date in DD-MM-YY:" << endl;
    cin >> date;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    cout << "Enter Time:" << endl;
    getline(cin, time); // Use getline to read the time
    Appointment temp;
    if (pID < 0 || pID >= allPatients.size()) {
        if (dID < 0 || dID >= allDoctors.size()) {
            cout << "Invalid Patient ID or Doctor ID" << endl;
            cout<<"Please Enter Correct Patient ID and Doctor ID"<<endl;
            return;
        }
        
    }

    temp.patientName = allPatients[pID].name;
    temp.doctorName = allDoctors[dID].name;
    temp.date = date;
    temp.time = time;
    allAppointments.push_back(temp);
}



void viewPatients(){
    cout<<"View Patients Details"<<endl;
    for(int i=0;i<allPatients.size();i++){
        cout<<"ID:"<<allPatients[i].ID<<endl;
        cout<<"Name:"<<allPatients[i].name<<endl;
        cout<<"age:"<<allPatients[i].age<<endl;
        cout<<"Gender:"<<allPatients[i].Gender<<endl;
        cout<<"Address:"<<allPatients[i].address<<endl;
        cout<<"Phone:"<<allPatients[i].phone<<endl;
        cout<<"Email:"<<allPatients[i].email<<endl;
        cout<<endl;

    }
    
}
void viewDoctors(){
    cout<<"View Doctors Details"<<endl;
    for(int i=0;i<allDoctors.size();i++){
        cout<<"ID:"<<allDoctors[i].ID<<endl;
        cout<<"Name:"<<allDoctors[i].name<<endl;
        cout<<"Speciality:"<<allDoctors[i].speciality<<endl;
        cout<<"Address:"<<allDoctors[i].address<<endl;
        cout<<"Phone:"<<allDoctors[i].phone<<endl;
        cout<<"Email:"<<allDoctors[i].email<<endl;
        cout<<endl;
    }
   
}
void viewAppointments(){
    cout<<"View Appointments Details"<<endl;
    for(int i=0;i<allAppointments.size();i++){
        cout<<"Patient Name:"<<allAppointments[i].patientName<<endl;
        cout<<"Doctor Name:"<<allAppointments[i].doctorName<<endl;
        cout<<"Date:"<<allAppointments[i].date<<endl;
        cout<<"Time:"<<allAppointments[i].time<<endl;
        cout<<endl;
    }
   
}
void exit(){

    cout<<"Exit"<<endl;
}

int main(){
     int choice;
    
    do{

        cout<<"1.ADD Patient"<<endl;
        cout<<"2.ADD Doctors"<<endl;
        cout<<"3.Shedule Appointment"<<endl;
        cout<<"4.View Patients"<<endl;
        cout<<"5.View Doctors"<<endl;
        cout<<"6.View Appointmaints"<<endl;
        cout<<"0.Exit"<<endl;
        cout<<"Enter your choice"<<endl;
        cin>>choice;
        switch (choice)
        {
        case 1:
            addPatient();
            break;
        case 2:
            addDoctor();
            break;
        case 3:
            scheduleAppointment();
            break;
        case 4:
            viewPatients();
            break;
        case 5:
            viewDoctors();
            break;
        case 6:
            viewAppointments();
            break;
        default:
        cout<<"Invalid Choice"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<endl;
            break;
        }
    }while(choice!=0);
    return 0;   
}
