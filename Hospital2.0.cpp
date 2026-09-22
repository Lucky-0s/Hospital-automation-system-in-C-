#include<bits/stdc++.h>

using namespace std;

// ============================================================================
// PATIENT RECORD STRUCTURE
// ============================================================================

struct Patient {
    string patientID;
    string name;
    int age;
    string gender;
    string phone;
    string email;
    string bloodType;
    string medicalHistory;
    string currentMedications;
    string address;
    string lastVisit;
    string status; // Active, Inactive, Discharged

    // Default constructor
    Patient() : age(0) {}

    // Parameterized constructor
    Patient(string id, string n, int a, string g, string p, string e,
            string b, string m, string c, string addr)
        : patientID(id),
          name(n),
          age(a),
          gender(g),
          phone(p),
          email(e),
          bloodType(b),
          medicalHistory(m),
          currentMedications(c),
          address(addr),
          status("Active") {

        lastVisit = getCurrentDate();
    }

    // Get current date
    static string getCurrentDate() {
        time_t now = time(nullptr);
        tm* timeinfo = localtime(&now);

        char buffer[80];

        if (timeinfo == nullptr) {
            return "Unknown";
        }

        strftime(buffer, sizeof(buffer),
                 "%d-%m-%Y %H:%M:%S", timeinfo);

        return string(buffer);
    }

    // Display patient details
    void display() const {
        cout << "\n" << string(70, '-') << "\n";
        cout << "  PATIENT RECORD\n";
        cout << string(70, '-') << "\n";

        cout << setw(25) << left << "Patient ID:" << patientID << "\n";
        cout << setw(25) << left << "Name:" << name << "\n";
        cout << setw(25) << left << "Age:" << age << " years\n";
        cout << setw(25) << left << "Gender:" << gender << "\n";
        cout << setw(25) << left << "Phone:" << phone << "\n";
        cout << setw(25) << left << "Email:" << email << "\n";
        cout << setw(25) << left << "Blood Type:" << bloodType << "\n";
        cout << setw(25) << left << "Address:" << address << "\n";
        cout << setw(25) << left << "Medical History:"
             << medicalHistory << "\n";
        cout << setw(25) << left << "Current Medications:"
             << currentMedications << "\n";
        cout << setw(25) << left << "Last Visit:" << lastVisit << "\n";
        cout << setw(25) << left << "Status:" << status << "\n";

        cout << string(70, '-') << "\n";
    }

    // Convert to file format
    string toCSV() const {
        return patientID + "|" +
               name + "|" +
               to_string(age) + "|" +
               gender + "|" +
               phone + "|" +
               email + "|" +
               bloodType + "|" +
               medicalHistory + "|" +
               currentMedications + "|" +
               address + "|" +
               lastVisit + "|" +
               status;
    }

    // Create Patient from file line
    static Patient fromCSV(const string& line) {
        vector<string> fields;
        stringstream ss(line);
        string field;

        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() < 12) {
            return Patient();
        }

        Patient p;

        try {
            p.patientID = fields[0];
            p.name = fields[1];
            p.age = stoi(fields[2]);
            p.gender = fields[3];
            p.phone = fields[4];
            p.email = fields[5];
            p.bloodType = fields[6];
            p.medicalHistory = fields[7];
            p.currentMedications = fields[8];
            p.address = fields[9];
            p.lastVisit = fields[10];
            p.status = fields[11];
        }
        catch (...) {
            return Patient();
        }

        return p;
    }
};

// ============================================================================
// PATIENT DATABASE MANAGEMENT SYSTEM
// ============================================================================

class PatientDatabase {

private:

    unordered_map<string, Patient> patientMap;
    vector<Patient> patientList;

    const string DATABASE_FILE = "patients_database.csv";

public:

    PatientDatabase() {
        loadFromFile();
    }

    // ========================================================================
    // 1. ADD NEW PATIENT
    // ========================================================================

    bool addPatient(const Patient& patient) {

        if (patient.patientID.empty()) {
            cout << "\nError: Patient ID cannot be empty!\n";
            return false;
        }

        if (patientMap.find(patient.patientID) != patientMap.end()) {
            cout << "\nError: Patient ID '"
                 << patient.patientID
                 << "' already exists!\n";
            return false;
        }

        if (patient.age <= 0 || patient.age > 150) {
            cout << "\nError: Invalid age!\n";
            return false;
        }

        if (!isValidPhone(patient.phone)) {
            cout << "\nError: Invalid phone number format!\n";
            return false;
        }

        if (!isValidEmail(patient.email)) {
            cout << "\nError: Invalid email format!\n";
            return false;
        }

        patientMap[patient.patientID] = patient;
        patientList.push_back(patient);

        cout << "\nPatient record added successfully!\n";

        return true;
    }

    // ========================================================================
    // 2. RETRIEVE PATIENT BY ID
    // ========================================================================

    Patient* getPatientByID(const string& patientID) {

        auto it = patientMap.find(patientID);

        if (it != patientMap.end()) {
            return &it->second;
        }

        return nullptr;
    }

    // ========================================================================
    // 3. UPDATE PATIENT RECORD
    // ========================================================================

    bool updatePatient(const string& patientID,
                       const Patient& updatedData) {

        auto it = patientMap.find(patientID);

        if (it == patientMap.end()) {
            cout << "\nError: Patient not found!\n";
            return false;
        }

        patientMap[patientID] = updatedData;

        for (auto& p : patientList) {

            if (p.patientID == patientID) {
                p = updatedData;
                break;
            }
        }

        cout << "\nPatient record updated successfully!\n";

        return true;
    }

    // ========================================================================
    // 4. SEARCH PATIENTS
    // ========================================================================

    vector<Patient> searchByName(const string& name) {

        vector<Patient> results;

        string lowerName = toLowercase(name);

        for (const auto& patient : patientList) {

            if (toLowercase(patient.name).find(lowerName)
                != string::npos) {

                results.push_back(patient);
            }
        }

        return results;
    }

    vector<Patient> searchByPhone(const string& phone) {

        vector<Patient> results;

        for (const auto& patient : patientList) {

            if (patient.phone.find(phone) != string::npos) {
                results.push_back(patient);
            }
        }

        return results;
    }

    vector<Patient> searchByBloodType(const string& bloodType) {

        vector<Patient> results;

        for (const auto& patient : patientList) {

            if (toLowercase(patient.bloodType)
                == toLowercase(bloodType)) {

                results.push_back(patient);
            }
        }

        return results;
    }

    vector<Patient> searchByAge(int minAge, int maxAge) {

        vector<Patient> results;

        for (const auto& patient : patientList) {

            if (patient.age >= minAge &&
                patient.age <= maxAge) {

                results.push_back(patient);
            }
        }

        return results;
    }

    vector<Patient> searchByStatus(const string& status) {

        vector<Patient> results;

        string lowerStatus = toLowercase(status);

        for (const auto& patient : patientList) {

            if (toLowercase(patient.status) == lowerStatus) {
                results.push_back(patient);
            }
        }

        return results;
    }

    // ========================================================================
    // 5. DELETE PATIENT
    // ========================================================================

    bool deletePatient(const string& patientID) {

        auto it = patientMap.find(patientID);

        if (it == patientMap.end()) {
            cout << "\nError: Patient not found!\n";
            return false;
        }

        patientMap.erase(it);

        patientList.erase(
            remove_if(
                patientList.begin(),
                patientList.end(),
                [&](const Patient& p) {
                    return p.patientID == patientID;
                }
            ),
            patientList.end()
        );

        cout << "\nPatient record deleted successfully!\n";

        return true;
    }

    // ========================================================================
    // 6. DISPLAY ALL PATIENTS
    // ========================================================================

    void displayAllPatients() const {

        if (patientList.empty()) {
            cout << "\nNo patients in database.\n";
            return;
        }

        cout << "\n" << string(100, '=') << "\n";
        cout << "  ALL PATIENT RECORDS\n";
        cout << string(100, '=') << "\n";

        cout << setw(12) << left << "ID"
             << setw(20) << left << "Name"
             << setw(6) << left << "Age"
             << setw(10) << left << "Gender"
             << setw(15) << left << "Phone"
             << setw(12) << left << "Blood Type"
             << setw(15) << left << "Status"
             << "\n";

        cout << string(100, '-') << "\n";

        for (const auto& patient : patientList) {

            cout << setw(12) << left << patient.patientID
                 << setw(20) << left << patient.name
                 << setw(6) << left << patient.age
                 << setw(10) << left << patient.gender
                 << setw(15) << left << patient.phone
                 << setw(12) << left << patient.bloodType
                 << setw(15) << left << patient.status
                 << "\n";
        }

        cout << string(100, '=') << "\n";

        cout << "Total Patients: "
             << patientList.size() << "\n";
    }

    // ========================================================================
    // 7. DATABASE STATISTICS
    // ========================================================================

    void displayStatistics() const {

        if (patientList.empty()) {
            cout << "\nNo patients in database.\n";
            return;
        }

        int totalPatients =
            static_cast<int>(patientList.size());

        int activePatients = 0;
        int inactivePatients = 0;
        int dischargedPatients = 0;

        int maleCount = 0;
        int femaleCount = 0;

        double averageAge = 0.0;

        map<string, int> bloodTypeCount;

        for (const auto& patient : patientList) {

            if (patient.status == "Active")
                activePatients++;

            else if (patient.status == "Inactive")
                inactivePatients++;

            else if (patient.status == "Discharged")
                dischargedPatients++;

            if (patient.gender == "Male")
                maleCount++;

            else if (patient.gender == "Female")
                femaleCount++;

            averageAge += patient.age;

            bloodTypeCount[patient.bloodType]++;
        }

        averageAge /= totalPatients;

        cout << "\n" << string(60, '=') << "\n";
        cout << "  HOSPITAL STATISTICS\n";
        cout << string(60, '=') << "\n";

        cout << "Total Patients: "
             << totalPatients << "\n";

        cout << "  - Active: "
             << activePatients << "\n";

        cout << "  - Inactive: "
             << inactivePatients << "\n";

        cout << "  - Discharged: "
             << dischargedPatients << "\n";

        cout << "\nGender Distribution:\n";

        cout << "  - Male: "
             << maleCount << "\n";

        cout << "  - Female: "
             << femaleCount << "\n";

        cout << "\nAverage Age: "
             << fixed << setprecision(2)
             << averageAge << "\n";

        cout << "\nBlood Type Distribution:\n";

        for (const auto& entry : bloodTypeCount) {

            cout << "  - "
                 << entry.first
                 << ": "
                 << entry.second
                 << "\n";
        }

        cout << string(60, '=') << "\n";
    }

    // ========================================================================
    // 8. SAVE DATABASE
    // ========================================================================

    bool saveToFile() {

        ofstream file(DATABASE_FILE);

        if (!file.is_open()) {

            cout << "\nError: Could not open file for writing!\n";

            return false;
        }

        file << "PatientID|Name|Age|Gender|Phone|Email|BloodType|"
             << "MedicalHistory|CurrentMedications|Address|"
             << "LastVisit|Status\n";

        for (const auto& patient : patientList) {
            file << patient.toCSV() << "\n";
        }

        file.close();

        cout << "\nDatabase saved successfully!\n";

        return true;
    }

    // ========================================================================
    // 9. LOAD DATABASE
    // ========================================================================

    bool loadFromFile() {

        ifstream file(DATABASE_FILE);

        if (!file.is_open()) {

            cout << "No existing database found. "
                 << "Starting fresh.\n";

            return false;
        }

        string line;

        bool isHeader = true;

        while (getline(file, line)) {

            if (isHeader) {
                isHeader = false;
                continue;
            }

            if (line.empty())
                continue;

            Patient p = Patient::fromCSV(line);

            if (!p.patientID.empty()) {

                patientMap[p.patientID] = p;
                patientList.push_back(p);
            }
        }

        file.close();

        cout << "Loaded "
             << patientList.size()
             << " patients from file.\n";

        return true;
    }

private:

    // ========================================================================
    // UTILITY FUNCTIONS
    // ========================================================================

    string toLowercase(const string& str) const {

        string result = str;

        transform(
            result.begin(),
            result.end(),
            result.begin(),
            [](unsigned char c) {
                return static_cast<char>(tolower(c));
            }
        );

        return result;
    }

    bool isValidEmail(const string& email) const {

        regex emailRegex(
            R"([a-zA-Z0-9._%-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})"
        );

        return regex_match(email, emailRegex);
    }

    bool isValidPhone(const string& phone) const {

        regex phoneRegex(R"(\d{10,15})");

        return regex_match(phone, phoneRegex);
    }
};

// ============================================================================
// USER INTERFACE
// ============================================================================

class HospitalUI {

private:

    PatientDatabase db;

public:

    void run() {

        cout << "\n"
             << string(70, '=')
             << "\n";

        cout << "  HOSPITAL PATIENT RECORD MANAGEMENT SYSTEM\n";

        cout << string(70, '=')
             << "\n";

        int choice;

        do {

            displayMainMenu();

            cout << "\nEnter your choice: ";

            cin >> choice;

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            switch (choice) {

                case 1:
                    addNewPatient();
                    break;

                case 2:
                    retrievePatient();
                    break;

                case 3:
                    updatePatientRecord();
                    break;

                case 4:
                    searchPatients();
                    break;

                case 5:
                    deletePatientRecord();
                    break;

                case 6:
                    db.displayAllPatients();
                    break;

                case 7:
                    db.displayStatistics();
                    break;

                case 8:
                    db.saveToFile();
                    break;

                case 9:

                    cout << "\nSaving data and exiting...\n";

                    db.saveToFile();

                    cout << "Thank you for using "
                         << "Hospital Management System!\n";

                    break;

                default:

                    cout << "\nInvalid choice! "
                         << "Please try again.\n";
            }

        } while (choice != 9);
    }

private:

    // ========================================================================
    // MAIN MENU
    // ========================================================================

    void displayMainMenu() {

        cout << "\n"
             << string(50, '-')
             << "\n";

        cout << "  MAIN MENU\n";

        cout << string(50, '-')
             << "\n";

        cout << "1. Add New Patient\n";
        cout << "2. Retrieve Patient by ID\n";
        cout << "3. Update Patient Record\n";
        cout << "4. Search Patients\n";
        cout << "5. Delete Patient Record\n";
        cout << "6. View All Patients\n";
        cout << "7. View Statistics\n";
        cout << "8. Save Database\n";
        cout << "9. Exit\n";

        cout << string(50, '-')
             << "\n";
    }

    // ========================================================================
    // ADD PATIENT
    // ========================================================================

    void addNewPatient() {

        cout << "\n--- ADD NEW PATIENT ---\n";

        string id;
        string name;
        string gender;
        string phone;
        string email;
        string bloodType;
        string medical;
        string medications;
        string address;

        int age;

        cout << "Patient ID: ";
        getline(cin, id);

        cout << "Name: ";
        getline(cin, name);

        cout << "Age: ";
        cin >> age;

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        cout << "Gender (Male/Female/Other): ";
        getline(cin, gender);

        cout << "Phone Number: ";
        getline(cin, phone);

        cout << "Email: ";
        getline(cin, email);

        cout << "Blood Type "
             << "(A+, A-, B+, B-, O+, O-, AB+, AB-): ";

        getline(cin, bloodType);

        cout << "Medical History: ";
        getline(cin, medical);

        cout << "Current Medications: ";
        getline(cin, medications);

        cout << "Address: ";
        getline(cin, address);

        Patient newPatient(
            id,
            name,
            age,
            gender,
            phone,
            email,
            bloodType,
            medical,
            medications,
            address
        );

        db.addPatient(newPatient);
    }

    // ========================================================================
    // RETRIEVE PATIENT
    // ========================================================================

    void retrievePatient() {

        cout << "\n--- RETRIEVE PATIENT ---\n";

        cout << "Enter Patient ID: ";

        string id;

        getline(cin, id);

        Patient* patient =
            db.getPatientByID(id);

        if (patient) {

            patient->display();

        } else {

            cout << "\nPatient not found!\n";
        }
    }

    // ========================================================================
    // UPDATE PATIENT
    // ========================================================================

    void updatePatientRecord() {

        cout << "\n--- UPDATE PATIENT RECORD ---\n";

        cout << "Enter Patient ID to update: ";

        string id;

        getline(cin, id);

        Patient* patient =
            db.getPatientByID(id);

        if (!patient) {

            cout << "\nPatient not found!\n";

            return;
        }

        cout << "\nCurrent Record:\n";

        patient->display();

        cout << "\nEnter new details "
             << "(or press Enter to keep existing):\n";

        string name;

        cout << "Name (" << patient->name << "): ";

        getline(cin, name);

        if (name.empty())
            name = patient->name;

        string ageStr;

        cout << "Age (" << patient->age << "): ";

        getline(cin, ageStr);

        int age = patient->age;

        if (!ageStr.empty()) {

            try {
                age = stoi(ageStr);
            }
            catch (...) {

                cout << "Invalid age. "
                     << "Keeping existing age.\n";

                age = patient->age;
            }
        }

        string phone;

        cout << "Phone (" << patient->phone << "): ";

        getline(cin, phone);

        if (phone.empty())
            phone = patient->phone;

        string medical;

        cout << "Medical History: ";

        getline(cin, medical);

        if (medical.empty())
            medical = patient->medicalHistory;

        string medications;

        cout << "Current Medications: ";

        getline(cin, medications);

        if (medications.empty())
            medications = patient->currentMedications;

        string status;

        cout << "Status "
             << "(Active/Inactive/Discharged) "
             << "(" << patient->status << "): ";

        getline(cin, status);

        if (status.empty())
            status = patient->status;

        Patient updated = *patient;

        updated.name = name;
        updated.age = age;
        updated.phone = phone;
        updated.medicalHistory = medical;
        updated.currentMedications = medications;
        updated.status = status;

        updated.lastVisit =
            Patient::getCurrentDate();

        db.updatePatient(id, updated);
    }

    // ========================================================================
    // SEARCH PATIENTS
    // ========================================================================

    void searchPatients() {

        cout << "\n--- SEARCH PATIENTS ---\n";

        cout << "1. Search by Name\n";
        cout << "2. Search by Phone\n";
        cout << "3. Search by Blood Type\n";
        cout << "4. Search by Age Range\n";
        cout << "5. Search by Status\n";

        cout << "Enter choice: ";

        int choice;

        cin >> choice;

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        vector<Patient> results;

        switch (choice) {

            case 1: {

                cout << "Enter name: ";

                string name;

                getline(cin, name);

                results =
                    db.searchByName(name);

                break;
            }

            case 2: {

                cout << "Enter phone: ";

                string phone;

                getline(cin, phone);

                results =
                    db.searchByPhone(phone);

                break;
            }

            case 3: {

                cout << "Enter blood type: ";

                string blood;

                getline(cin, blood);

                results =
                    db.searchByBloodType(blood);

                break;
            }

            case 4: {

                int minAge;
                int maxAge;

                cout << "Enter minimum age: ";
                cin >> minAge;

                cout << "Enter maximum age: ";
                cin >> maxAge;

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                results =
                    db.searchByAge(
                        minAge,
                        maxAge
                    );

                break;
            }

            case 5: {

                cout << "Enter status "
                     << "(Active/Inactive/Discharged): ";

                string status;

                getline(cin, status);

                results =
                    db.searchByStatus(status);

                break;
            }

            default:

                cout << "\nInvalid choice!\n";

                return;
        }

        displaySearchResults(results);
    }

    // ========================================================================
    // DISPLAY SEARCH RESULTS
    // ========================================================================

    void displaySearchResults(
        const vector<Patient>& results
    ) {

        if (results.empty()) {

            cout << "\nNo patients found "
                 << "matching your criteria.\n";

            return;
        }

        cout << "\n"
             << string(100, '=')
             << "\n";

        cout << "  SEARCH RESULTS ("
             << results.size()
             << " found)\n";

        cout << string(100, '=')
             << "\n";

        cout << setw(12) << left << "ID"
             << setw(20) << left << "Name"
             << setw(6) << left << "Age"
             << setw(10) << left << "Gender"
             << setw(15) << left << "Phone"
             << setw(12) << left << "Blood Type"
             << setw(15) << left << "Status"
             << "\n";

        cout << string(100, '-') << "\n";

        for (const auto& patient : results) {

            cout << setw(12) << left
                 << patient.patientID

                 << setw(20) << left
                 << patient.name

                 << setw(6) << left
                 << patient.age

                 << setw(10) << left
                 << patient.gender

                 << setw(15) << left
                 << patient.phone

                 << setw(12) << left
                 << patient.bloodType

                 << setw(15) << left
                 << patient.status

                 << "\n";
        }

        cout << string(100, '=') << "\n";
    }

    // ========================================================================
    // DELETE PATIENT
    // ========================================================================

    void deletePatientRecord() {

        cout << "\n--- DELETE PATIENT ---\n";

        cout << "Enter Patient ID to delete: ";

        string id;

        getline(cin, id);

        cout << "Are you sure? (yes/no): ";

        string confirm;

        getline(cin, confirm);

        if (confirm == "yes" ||
            confirm == "YES" ||
            confirm == "Yes") {

            db.deletePatient(id);

        } else {

            cout << "\nDeletion cancelled.\n";
        }
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {

    HospitalUI system;

    system.run();

    return 0;
}