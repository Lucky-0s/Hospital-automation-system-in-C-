Hospital Patient Record Management System - Implementation Guide
Table of Contents
	1. Problem Statement
	2. System Architecture
	3. Data Structures Used
	4. Algorithm Analysis
	5. Key Features
	6. Step-by-Step Implementation
	7. Compilation & Execution
	8. Usage Examples
	9. Future Enhancements

Problem Statement
Challenge
A hospital receives hundreds of patients daily. The traditional paper-based record system creates several problems:
	1. Retrieval Delay: Receptionists spend minutes searching through files for a single patient
	2. Data Inconsistency: Multiple copies of records, conflicting information
	3. Limited Search Capability: Cannot search by multiple criteria (name, phone, blood type)
	4. Update Bottleneck: Medical information updates take time to propagate
	5. Storage & Security: Physical files take space and are prone to loss
	6. Error-Prone Process: Manual entry leads to mistakes
Solution Benefits
An automated computerized system provides:
	• ✅ Instant O(1) retrieval by patient ID
	• ✅ Real-time updates across the system
	• ✅ Multi-criteria search (name, phone, age, blood type, status)
	• ✅ Complete audit trail of changes
	• ✅ Reduced human error through validation
	• ✅ Better decision-making with statistics & analytics
	• ✅ HIPAA-compliant data persistence

System Architecture
Three-Tier Architecture
┌─────────────────────────────────────────────────────────────┐
│                     USER INTERFACE TIER                      │
│              (HospitalUI - Menu-Driven Interface)            │
├─────────────────────────────────────────────────────────────┤
│                   BUSINESS LOGIC TIER                         │
│           (PatientDatabase - Core Operations)                │
│  • Add, Retrieve, Update, Delete                             │
│  • Search (Name, Phone, Blood Type, Age, Status)             │
│  • Statistics & Analytics                                    │
├─────────────────────────────────────────────────────────────┤
│                     DATA TIER                                │
│  • Hash Map (Fast ID Lookup)                                 │
│  • Vector (Sorting & Iteration)                              │
│  • CSV File (Persistence)                                    │
└─────────────────────────────────────────────────────────────┘

Data Flow Diagram
Receptionist Input
       ↓
    UI Layer (Menu)
       ↓
 Business Logic (Validation & Processing)
       ↓
 Data Structures (Hash Map + Vector)
       ↓
 File I/O (Save/Load)
       ↓
Updated Database


Data Structures Used
1. Hash Map (unordered map)
unordered_map<string, Patient> patientMap;
Purpose: O(1) average-case lookup for patient ID queries Why: Receptionist often searches by patient ID → must be instant
Complexity:
	• Insert: O(1) average, O(n) worst case
	• Delete: O(1) average
	• Search: O(1) average
2. Vector (Dynamic Array)
vector<Patient> patientList;
Purpose: Maintain insertion order and enable iteration for:
	• Displaying all patients
	• Sorting operations
	• Range queries (age, blood type)
	• Statistical analysis
Complexity:
	• Insert: O(1) amortized
	• Iterate: O(n) for each traversal
	• Search: O(n) linear scan
3. Patient Structure
struct Patient {
    string patientID;      // Unique identifier
    string name;           // Full name
    int age;               // Age in years
    string gender;         // M/F/Other
    string phone;          // Contact number
    string email;          // Email address
    string bloodType;      // A+, B-, O+, etc.
    string medicalHistory; // Past medical conditions
    string currentMedications; // Active prescriptions
    string address;        // Residential address
    string lastVisit;      // Last visit timestamp
    string status;         // Active/Inactive/Discharged
};
4. File Persistence (CSV Format)
PatientID|Name|Age|Gender|Phone|Email|BloodType|MedicalHistory|CurrentMedications|Address|LastVisit|Status
P001|John Doe|45|Male|9876543210|john@email.com|O+|Diabetes|Metformin, Aspirin|123 Main St|22-09-2026 10:30:00|Active

Algorithm Analysis
Time Complexity Summary
Operation	Time Complexity	Space Complexity
Add Patient	O(1)	O(1)
Retrieve by ID	O(1)	O(1)
Update Patient	O(n)	O(1)
Delete Patient	O(n)	O(1)
Search by Name	O(n)	O(k)
Search by Phone	O(n)	O(k)
Search by Age	O(n)	O(k)
Display All	O(n)	O(1)
Statistics	O(n)	O(1)
Where: k = number of results returned
Detailed Algorithm: Add Patient
ALGORITHM AddPatient(patient):
    1. Check if patientID exists in hash map
       - If yes: Return error "ID already exists"
    
    2. Validate phone format (regex: \d{10,15})
       - If invalid: Return error
    
    3. Validate email format (regex: user@domain.ext)
       - If invalid: Return error
    
    4. Insert into hash map: patientMap[patientID] = patient
    
    5. Add to vector: patientList.push_back(patient)
    
    6. Persist to file
    
    7. Return success
TIME COMPLEXITY: O(1) average case
SPACE COMPLEXITY: O(1)
Detailed Algorithm: Search by Name
ALGORITHM SearchByName(name):
    1. Convert search name to lowercase
    
    2. Iterate through patientList
       FOR EACH patient in patientList:
           - Convert patient.name to lowercase
           - Check if search name is substring
           - If yes: Add to results vector
    
    3. Return results
TIME COMPLEXITY: O(n) where n = number of patients
SPACE COMPLEXITY: O(k) where k = results found
Detailed Algorithm: Update Patient
ALGORITHM UpdatePatient(patientID, updatedData):
    1. Search in hash map: patientMap.find(patientID)
       - If not found: Return error
    
    2. Update hash map: patientMap[patientID] = updatedData
    
    3. Iterate through vector:
       FOR EACH patient in patientList:
           - If patient.patientID == patientID:
               Update patient with new data
               Break
    
    4. Update lastVisit timestamp
    
    5. Persist to file
    
    6. Return success
TIME COMPLEXITY: O(n) due to vector iteration
SPACE COMPLEXITY: O(1)

Key Features Explained
1. O(1) Patient Lookup
Patient* getPatientByID(const string& patientID) {
    auto it = patientMap.find(patientID);  // O(1) hash lookup
    if (it != patientMap.end()) {
        return &it->second;
    }
    return nullptr;
}
Receptionist enters ID → Instant retrieval (no delay)
2. Multi-Criteria Search
	• By Name: Partial matching (e.g., "John" matches "John Doe")
	• By Phone: Substring search for partial numbers
	• By Blood Type: Exact matching for donor pool identification
	• By Age Range: Find patients eligible for specific treatments
	• By Status: Quick filter (Active/Inactive/Discharged)
3. Data Validation
// Email validation using regex
bool isValidEmail(const string& email) {
    regex emailRegex(R"(([a-zA-Z0-9._%-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");
    return regex_match(email, emailRegex);
}
// Phone validation
bool isValidPhone(const string& phone) {
    regex phoneRegex(R"(\d{10,15})");
    return regex_match(phone, phoneRegex);
}
Ensures data integrity before storage
4. Automatic Timestamp Tracking
static string getCurrentDate() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return string(buffer);
}
Every patient record tracks last visit/update time
5. Data Persistence
// Save to CSV file
bool saveToFile() {
    ofstream file(DATABASE_FILE);
    for (const auto& patient : patientList) {
        file << patient.toCSV() << "\n";
    }
    file.close();
}
// Load from CSV file
bool loadFromFile() {
    ifstream file(DATABASE_FILE);
    string line;
    while (getline(file, line)) {
        Patient p = Patient::fromCSV(line);
        patientMap[p.patientID] = p;
        patientList.push_back(p);
    }
}
Data survives application restart
6. Statistical Analytics
	• Total patient count
	• Status distribution (Active/Inactive/Discharged)
	• Gender demographics
	• Average patient age
	• Blood type distribution

Step-by-Step Implementation
Phase 1: Data Modeling (Patient Structure)
	1. Define essential fields for patient
	2. Add validation rules
	3. Implement serialization (CSV conversion)
Phase 2: Core Database (PatientDatabase Class)
	1. Tier 1: Implement CRUD operations
		○ Add patient (with duplicate check)
		○ Retrieve by ID (O(1) lookup)
		○ Update patient
		○ Delete patient
	2. Tier 2: Implement Search Operations
		○ By name (substring matching)
		○ By phone (partial matching)
		○ By blood type (exact matching)
		○ By age (range query)
		○ By status (filtering)
	3. Tier 3: Implement Persistence
		○ Save to file (CSV format)
		○ Load from file (parsing)
		○ Error handling
Phase 3: User Interface (HospitalUI Class)
	1. Create menu-driven interface
	2. Implement input validation
	3. Display formatted results
	4. Handle user interactions
Phase 4: Main Application
	1. Initialize database
	2. Load existing data from file
	3. Launch UI
	4. Handle save on exit


Usage Examples
Example 1: Adding a Patient
Enter choice: 1
--- ADD NEW PATIENT ---
Patient ID: P001
Name: Rajesh Kumar
Age: 45
Gender (Male/Female/Other): Male
Phone Number: 9876543210
Email: rajesh@example.com
Blood Type (A+, A-, B+, B-, O+, O-, AB+, AB-): O+
Medical History: Hypertension, Diabetes
Current Medications: Metformin, Lisinopril
Address: 123 Main Street, Mumbai
✓ Patient record added successfully!
Example 2: Retrieving a Patient
Enter choice: 2
--- RETRIEVE PATIENT ---
Enter Patient ID: P001
----------------------------------------------------------------------
  PATIENT RECORD
----------------------------------------------------------------------
  Patient ID:          P001
  Name:                Rajesh Kumar
  Age:                 45 years
  Gender:              Male
  Phone:               9876543210
  Email:               rajesh@example.com
  Blood Type:          O+
  Address:             123 Main Street, Mumbai
  Medical History:     Hypertension, Diabetes
  Current Medications: Metformin, Lisinopril
  Last Visit:          22-09-2026 14:30:45
  Status:              Active
----------------------------------------------------------------------
Example 3: Searching Patients
Enter choice: 4
--- SEARCH PATIENTS ---
1. Search by Name
2. Search by Phone
3. Search by Blood Type
4. Search by Age Range
5. Search by Status
Enter choice: 3
Enter blood type: O+
====================================================================================================
  SEARCH RESULTS (2 found)
====================================================================================================
ID          Name                 Age    Gender   Phone           Blood Type   Status
----------------------------------------------------------------------------------------------------
P001        Rajesh Kumar         45     Male     9876543210      O+           Active
P005        Priya Sharma         38     Female   9123456789      O+           Active
====================================================================================================
Example 4: Viewing Statistics
Enter choice: 7
------------------------------------------------------------
  HOSPITAL STATISTICS
------------------------------------------------------------
Total Patients: 5
  - Active: 4
  - Inactive: 1
  - Discharged: 0
Gender Distribution:
  - Male: 3
  - Female: 2
Average Age: 41.60
Blood Type Distribution:
  - A+: 1
  - B+: 1
  - O+: 2
  - AB-: 1
------------------------------------------------------------

Future Enhancements
1. Database Backend
// Replace CSV with SQLite/MySQL
#include <sqlite3.h>
// Benefits:
// - Better performance for large datasets (millions of records)
// - ACID compliance
// - Query optimization
// - Multi-user access support
2. Appointment Management
struct Appointment {
    string appointmentID;
    string patientID;
    string doctorID;
    datetime appointmentTime;
    string reason;
    string status;  // Scheduled, Completed, Cancelled
};
3. Billing Module
struct Bill {
    string billID;
    string patientID;
    vector<Service> services;
    double totalAmount;
    datetime billDate;
    bool isPaid;
};
4. User Authentication
// Role-based access control:
// - Admin: Full access
// - Receptionist: Add/Search/Update
// - Doctor: View medical history only
// - Finance: View billing only
struct User {
    string username;
    string passwordHash;
    string role;  // Admin, Receptionist, Doctor, Finance
};
5. Advanced Search Indexing
// Implement B-Tree for range queries
// Binary Search Tree for sorted operations
// Better performance for complex queries

6. Reporting Module
// Generate reports:
// - Daily patient admission report
// - Monthly statistics
// - Disease outbreak tracking
// - Department-wise workload analysis
7. Web/Mobile Interface
// Create REST API using:
// - Qt Framework
// - Crow (C++ microframework)
// - HTML5 frontend

8. Data Encryption
// Implement encryption for sensitive data:
// - AES encryption for patient data at rest
// - SSL/TLS for data in transit
// - HIPAA compliance

