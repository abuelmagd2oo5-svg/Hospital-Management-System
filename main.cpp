#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c){
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
    }

    string typeFromEnum(RoomType t){
        switch(t){
            case GENERAL_WARD:
                return "General Ward";
            case ICU:
                return "ICU";
            case PRIVATE_ROOM:
                return "Private Room";
            case SEMI_PRIVATE:
                return "Semi-Private Room";
        }

        return "Unknown Room";
    }

    void admitPatient(RoomType type){
        if(isAdmitted){
            cout << "Patient " << name << " is already admitted.\n";
            return;
        }

        roomType = type;
        cout << "Patient " << name << " admitted to " << typeFromEnum(type) << "\n";
        isAdmitted = true;

    }
    void dischargePatient(){
        if(!isAdmitted){
            cout << "Patient " << name << " is not currently admitted.\n";
            return;
        }
        cout << "Patient " << name << " has been discharged.\n";
        isAdmitted = false;
    }

    void addMedicalRecord(string record){
        medicalHistory.push(record);
        cout << "Medical record added for " << name << ": " << record << "\n";
    }
    void requestTest(string testName){
        testQueue.push(testName);
        cout << "Test requested for " << name << ": " << testName <<"\n";
    }
    string performTest(){
        if(testQueue.empty()){
            cout << "No pending tests for " << name << endl;
            return "";
        }
        string tst = testQueue.front();
        cout << "Performing test for " << name << ": " << tst;
        testQueue.pop();
        return tst;
    }
    void displayHistory(){
        cout << "Medical history for " << name << "\n";
        stack<string> med_hist = medicalHistory;

        if(med_hist.empty()){
            cout << "No medical history" << endl;
            return ;
        }

        while(!med_hist.empty()){
            cout << " - " << med_hist.top() << endl;
            med_hist.pop();
        }
    }

    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    bool getAdmissionStatus(){
        return isAdmitted;
    }
    int getAge() {
        return age;
    }

    string getContact() {
        return contact;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d);

    void addAppointment(int patientId);
    int seePatient();

    int getId();
    string getName();
    string getDepartment();
    int getPendingAppointmentsCount();
    int countAppointmentsForPatient(int patientId);
};

string departmentToString(Department department) {
    switch (department) {
        case CARDIOLOGY:
            return "Cardiology";
        case NEUROLOGY:
            return "Neurology";
        case ORTHOPEDICS:
            return "Orthopedics";
        case PEDIATRICS:
            return "Pediatrics";
        case EMERGENCY:
            return "Emergency";
        case GENERAL:
            return "General";
    }

    return "Unknown Department";
}

Doctor::Doctor(int did, string n, Department d) {
    id = did;
    name = n;
    department = d;
}

void Doctor::addAppointment(int patientId) {
    appointmentQueue.push(patientId);
    cout << "Appointment added for patient " << patientId << " with " << name << "\n";
}

int Doctor::seePatient() {
    if (appointmentQueue.empty()) {
        cout << "No appointments for Dr. " << name << ".\n";
        return -1;
    }

    int patientId = appointmentQueue.front();
    appointmentQueue.pop();
    cout << "Dr. " << name << " is seeing patient " << patientId << "\n";
    return patientId;
}

int Doctor::getId() {
    return id;
}

string Doctor::getName() {
    return name;
}

string Doctor::getDepartment() {
    return departmentToString(department);
}

int Doctor:: getPendingAppointmentsCount() {
    return appointmentQueue.size();
}

int Doctor:: countAppointmentsForPatient(int patientId) {
    int count = 0;
    queue<int> temp = appointmentQueue;

    while (!temp.empty()) {
        if (temp.front() == patientId)
            count++;
        temp.pop();
    }

    return count;
}

// ========== HOSPITAL CLASS ========== //
class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital()
    {
        patientCounter = 1;
        doctorCounter = 1;
    }

    int registerPatient(string name, int age, string contact)
    {
        int newID = patientCounter;
        Patient newPatient(newID, name, age, contact);
        patients.push_back(newPatient);
        patientCounter++;
        cout << "Patient registered: " << name << " (ID: " << newID << ")" << endl;
        return newID;
    }

    int addDoctor(string name, Department dept)
    {

     int newdoctorID = doctorCounter;
     Doctor newDoctor(newdoctorID, name, dept);
     doctors.push_back(newDoctor);
     doctorCounter ++;
     cout << "Doctor added: " << name << " (ID: " << newdoctorID << " )- " << departmentToString(dept) << endl;
     return newdoctorID;
    }

    void admitPatient(int patientId, RoomType type)
    {

        for (int i = 0; i < patients.size(); i++)
        {
          if (patients[i].getId() == patientId)
          {
            patients[i].admitPatient(type);
            return;
          }
        }

        cout << "Patient not found." << endl;
    }

    void addEmergency(int patientId)
    {
     emergencyQueue.push(patientId);
     cout << "Emergency added for patient " << patientId << endl;
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
           cout << "No emergency cases." << endl;
           return -1;
        }
        else{
           int ID = emergencyQueue.front();
           emergencyQueue.pop();
           cout << "Handling emergency for patient " << ID << endl;
           return ID;
        }

    }


    void bookAppointment(int doctorId, int patientId)
    {
        Doctor* doc = nullptr;
        Patient* pat = nullptr;
        for (int i = 0; i < doctors.size(); i++)
        {
            if (doctors[i].getId() == doctorId)
            {
               doc = &doctors[i];
               break;
            }
        }
        for (int i = 0; i < patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                pat = &patients[i];
                break;
            }
        }
            if (doc == nullptr)
        {
            cout << "Doctor not found." << endl;
            return;
        }

        if (pat == nullptr)
        {
            cout << "Patient not found." << endl;
            return;
        }


        doc->addAppointment(patientId);
    }

   void displayPatientInfo(int patientId)
    {
        for (int i = 0; i < patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                int pending = 0;

                for (int j = 0; j < doctors.size(); j++) {
                    pending += doctors[j].countAppointmentsForPatient(patientId);
                }

                cout << "\n=== Patient Information ===" << endl;
                cout << "ID: " << patients[i].getId() << endl;
                cout << "Name: " << patients[i].getName() << endl;
                cout << "Age: " << patients[i].getAge() << endl;
                cout << "Contact: " << patients[i].getContact() << endl;
                cout << "Admitted: "
                     << (patients[i].getAdmissionStatus() ? "Yes" : "No")
                    << endl;
                cout << "Pending Appointments: " << pending << endl;

                return;
            }
        }

        cout << "Patient not found." << endl;
    }

   void displayDoctorInfo(int doctorId)
    {
        for (int i = 0; i < doctors.size(); i++)
        {
            if (doctors[i].getId() == doctorId)
            {
                cout << "\n=== Doctor Information ===" << endl;
                cout << "ID: " << doctors[i].getId() << endl;
                cout << "Name: " << doctors[i].getName() << endl;
                cout << "Department: "
                     << doctors[i].getDepartment() << endl;
                cout << "Pending Appointments: " << doctors[i].getPendingAppointmentsCount() << endl;
                return;
            }
        }

        cout << "Doctor not found." << endl;
    }

    void processDoctorAppointments(int doctorId) {
        for (int i = 0; i < doctors.size(); i++) {
            if (doctors[i].getId() == doctorId) {
                doctors[i].seePatient();
                return;
            }
        }
        cout << "Doctor not found." << endl;
    }

    void dischargePatient(int patientId) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].dischargePatient();
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    void addMedicalRecord(int patientId, string record) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].addMedicalRecord(record);
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    void requestTest(int patientId, string testName) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].requestTest(testName);
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    void performTest(int patientId) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].performTest();
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    void displayPatientHistory(int patientId) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patients[i].displayHistory();
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
};

// ========== MAIN PROGRAM ========== //
int main() {
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");
    cout << "\n==============================\n";

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);
    cout << "\n==============================\n";

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);
    cout << "\n==============================\n";

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient
    cout << "\n==============================\n";

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients
    hospital.addMedicalRecord(p1, "Diabetes");
    hospital.addMedicalRecord(p1, "High Blood Pressure");

    hospital.requestTest(p1, "Blood Test");
    hospital.requestTest(p1, "X-Ray");

    hospital.performTest(p1);
    hospital.performTest(p1);
    hospital.performTest(p1); // no tests left

    hospital.displayPatientHistory(p1);
    cout << "\n==============================\n";

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies
    cout << "\n==============================\n";

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects
    // Test Case 7: Discharging patients
    hospital.dischargePatient(p1);
    hospital.dischargePatient(p2);
    hospital.dischargePatient(999); // invalid patient
    cout << "\n==============================\n";

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor
    cout << "\n==============================\n";

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors
    // Test Case 9: Doctor seeing patients
    hospital.processDoctorAppointments(d1);
    hospital.processDoctorAppointments(d1);
    hospital.processDoctorAppointments(d2);
    hospital.processDoctorAppointments(999); // invalid doctor
    cout << "\n==============================\n";

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
