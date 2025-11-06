#include "Admin.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

namespace {
    static inline string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
    string csvEscape(const string& value) {
        bool needsQuotes = value.find_first_of(",\"\n") != string::npos;
        string out = value;
        size_t pos = 0;
        while ((pos = out.find('"', pos)) != string::npos) {
            out.insert(pos, 1, '"');
            pos += 2;
        }
        if (needsQuotes) return string("\"") + out + string("\"");
        return out;
    }
    vector<string> csvParseLine(const string& line) {
        vector<string> result; string cur; bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < line.size() && line[i + 1] == '"') { cur.push_back('"'); ++i; }
                    else { inQuotes = false; }
                } else { cur.push_back(c); }
            } else {
                if (c == ',') { result.push_back(cur); cur.clear(); }
                else if (c == '"') { inQuotes = true; }
                else { cur.push_back(c); }
            }
        }
        result.push_back(cur);
        for (auto& t : result) t = trim(t);
        return result;
    }
}

// Default constructor
Admin::Admin() : adminId(""), username(""), password(""), name(""), email("") {}

// Parameterized constructor
Admin::Admin(const string& id, const string& username, const string& password,
             const string& name, const string& email)
    : adminId(id), username(username), password(password), name(name), email(email) {}

// Getters
string Admin::getAdminId() const { return adminId; }
string Admin::getUsername() const { return username; }
string Admin::getName() const { return name; }
string Admin::getEmail() const { return email; }

// Authentication
bool Admin::authenticate(const string& username, const string& password) const {
    return (this->username == username && this->password == password);
}

void Admin::changePassword(const string& newPassword) {
    password = newPassword;
}

// Student Management
void Admin::addStudent(vector<Student>& students) {
    Student newStudent;
    newStudent.inputDetails();
    // Enforce 13-digit student ID format
    if (!regex_match(newStudent.getStudentId(), regex(R"(\d{13})"))) {
        cout << "Error: Student ID must be exactly 13 digits (e.g., 2024010015138)." << endl;
        return;
    }
    
    // Check if student ID already exists
    for (const auto& student : students) {
        if (student.getStudentId() == newStudent.getStudentId()) {
            cout << "Error: Student ID already exists!" << endl;
            return;
        }
    }
    
    students.push_back(newStudent);
    cout << "Student added successfully!" << endl;
}

void Admin::removeStudent(vector<Student>& students, const string& studentId) {
    auto it = find_if(students.begin(), students.end(),
        [&studentId](const Student& s) { return s.getStudentId() == studentId; });
    
    if (it != students.end()) {
        if (it->getIsAllocated()) {
            cout << "Warning: Student is currently allocated to a room. Deallocate first." << endl;
            return;
        }
        students.erase(it);
        cout << "Student removed successfully!" << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

void Admin::updateStudent(vector<Student>& students, const string& studentId) {
    Student* student = findStudent(students, studentId);
    if (student) {
        cout << "Current student information:" << endl;
        student->displayInfo();
        cout << "\nEnter new details:" << endl;
        // Capture original ID to allow same-ID updates
        string originalId = student->getStudentId();
        Student temp = *student;
        temp.inputDetails();
        // Enforce unique ID (except if unchanged)
        const string& newId = temp.getStudentId();
        // Enforce 13-digit format
        if (!regex_match(newId, regex(R"(\d{13})"))) {
            cout << "Error: Student ID must be exactly 13 digits (e.g., 2024010015138). Update cancelled." << endl;
            return;
        }
        if (newId != originalId) {
            bool exists = any_of(students.begin(), students.end(), [&](const Student& s){
                return s.getStudentId() == newId;
            });
            if (exists) {
                cout << "Error: Student ID already exists! Update cancelled." << endl;
                return;
            }
        }
        *student = temp;
        cout << "Student updated successfully!" << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

void Admin::displayAllStudents(const vector<Student>& students) const {
    if (students.empty()) {
        cout << "No students found." << endl;
        return;
    }
    
    cout << "\n=== All Students ===" << endl;
    cout << left << setw(12) << "Student ID" 
              << setw(20) << "Name" 
              << setw(15) << "Course" 
              << setw(8) << "Year" 
              << setw(15) << "Status" 
              << setw(15) << "Room" << endl;
    cout << string(85, '-') << endl;
    
    for (const auto& student : students) {
        cout << left << setw(12) << student.getStudentId()
                  << setw(20) << student.getName()
                  << setw(15) << student.getCourse()
                  << setw(8) << student.getYear()
                  << setw(15) << (student.getIsAllocated() ? "Allocated" : "Not Allocated")
                  << setw(15) << (student.getAllocatedRoom().empty() ? "N/A" : student.getAllocatedRoom())
                  << endl;
    }
}

Student* Admin::findStudent(vector<Student>& students, const string& studentId) {
    auto it = find_if(students.begin(), students.end(),
        [&studentId](const Student& s) { return s.getStudentId() == studentId; });
    
    return (it != students.end()) ? &(*it) : nullptr;
}

// Room Management
void Admin::addRoom(vector<Room>& rooms) {
    Room newRoom;
    newRoom.inputDetails();
    
    // Check if room number already exists
    for (const auto& room : rooms) {
        if (room.getRoomNumber() == newRoom.getRoomNumber()) {
            cout << "Error: Room number already exists!" << endl;
            return;
        }
    }
    
    rooms.push_back(newRoom);
    cout << "Room added successfully!" << endl;
}

void Admin::removeRoom(vector<Room>& rooms, const string& roomNumber) {
    auto it = find_if(rooms.begin(), rooms.end(),
        [&roomNumber](const Room& r) { return r.getRoomNumber() == roomNumber; });
    
    if (it != rooms.end()) {
        if (!it->isEmpty()) {
            cout << "Warning: Room is not empty. Remove occupants first." << endl;
            return;
        }
        rooms.erase(it);
        cout << "Room removed successfully!" << endl;
    } else {
        cout << "Room not found!" << endl;
    }
}

void Admin::updateRoom(vector<Room>& rooms, const string& roomNumber) {
    Room* room = findRoom(rooms, roomNumber);
    if (room) {
        cout << "Current room information:" << endl;
        room->displayInfo();
        cout << "\nEnter new details:" << endl;
        room->inputDetails();
        cout << "Room updated successfully!" << endl;
    } else {
        cout << "Room not found!" << endl;
    }
}

void Admin::displayAllRooms(const vector<Room>& rooms) const {
    if (rooms.empty()) {
        cout << "No rooms found." << endl;
        return;
    }
    
    cout << "\n=== All Rooms ===" << endl;
    cout << left << setw(12) << "Room No" 
              << setw(12) << "Type" 
              << setw(10) << "Capacity" 
              << setw(10) << "Occupied" 
              << setw(10) << "Available" 
              << setw(10) << "Rent" 
              << setw(10) << "Status" << endl;
    cout << string(74, '-') << endl;
    
    for (const auto& room : rooms) {
        cout << left << setw(12) << room.getRoomNumber()
                  << setw(12) << room.getRoomType()
                  << setw(10) << room.getCapacity()
                  << setw(10) << room.getCurrentOccupancy()
                  << setw(10) << room.getAvailableSlots()
                  << setw(10) << room.getRent()
                  << setw(10) << (room.getIsAvailable() ? "Available" : "Full")
                  << endl;
    }
}

Room* Admin::findRoom(vector<Room>& rooms, const string& roomNumber) {
    auto it = find_if(rooms.begin(), rooms.end(),
        [&roomNumber](const Room& r) { return r.getRoomNumber() == roomNumber; });
    
    return (it != rooms.end()) ? &(*it) : nullptr;
}

// Room Allocation
bool Admin::allocateRoom(vector<Student>& students, vector<Room>& rooms, 
                         const string& studentId, const string& roomNumber) {
    Student* student = findStudent(students, studentId);
    Room* room = findRoom(rooms, roomNumber);
    
    if (!student) {
        cout << "Student not found!" << endl;
        return false;
    }
    
    if (!room) {
        cout << "Room not found!" << endl;
        return false;
    }
    
    if (student->getIsAllocated()) {
        cout << "Student is already allocated to a room!" << endl;
        return false;
    }
    
    if (room->isFull()) {
        cout << "Room is full!" << endl;
        return false;
    }
    
    // Allocate room
    if (room->addOccupant(studentId)) {
        student->setAllocatedRoom(roomNumber);
        cout << "Room allocated successfully!" << endl;
        return true;
    }
    
    return false;
}

bool Admin::deallocateRoom(vector<Student>& students, vector<Room>& rooms, 
                           const string& studentId) {
    Student* student = findStudent(students, studentId);
    
    if (!student) {
        cout << "Student not found!" << endl;
        return false;
    }
    
    if (!student->getIsAllocated()) {
        cout << "Student is not allocated to any room!" << endl;
        return false;
    }
    
    Room* room = findRoom(rooms, student->getAllocatedRoom());
    if (room) {
        room->removeOccupant(studentId);
    }
    
    student->setAllocatedRoom("");
    cout << "Room deallocated successfully!" << endl;
    return true;
}

void Admin::autoAllocateRooms(vector<Student>& students, vector<Room>& rooms) {
    cout << "Starting automatic room allocation..." << endl;
    
    // Sort students by year (seniors first)
    sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) { return a.getYear() > b.getYear(); });
    
    int allocated = 0;
    for (auto& student : students) {
        if (student.getIsAllocated()) continue;
        
        // Find available room
        for (auto& room : rooms) {
            if (room.getIsAvailable()) {
                if (allocateRoom(students, rooms, student.getStudentId(), room.getRoomNumber())) {
                    allocated++;
                    break;
                }
            }
        }
    }
    
    cout << "Automatic allocation completed. " << allocated << " students allocated." << endl;
}

void Admin::allocateWithRoommatePreference(vector<Student>& students, vector<Room>& rooms) {
    cout << "Starting roommate preference allocation..." << endl;
    
    int allocated = 0;
    for (auto& student : students) {
        if (student.getIsAllocated()) continue;
        
        // Check if student has roommate preference
        if (!student.getPreferredRoommate().empty()) {
            Student* preferredRoommate = findStudent(students, student.getPreferredRoommate());
            
            if (preferredRoommate && !preferredRoommate->getIsAllocated()) {
                // Find room that can accommodate both
                for (auto& room : rooms) {
                    if (room.getCapacity() >= 2 && room.getAvailableSlots() >= 2) {
                        // Allocate both students
                        if (allocateRoom(students, rooms, student.getStudentId(), room.getRoomNumber()) &&
                            allocateRoom(students, rooms, preferredRoommate->getStudentId(), room.getRoomNumber())) {
                            allocated += 2;
                            cout << "Allocated " << student.getName() << " and " 
                                      << preferredRoommate->getName() << " to room " 
                                      << room.getRoomNumber() << endl;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Allocate remaining students
    autoAllocateRooms(students, rooms);
    cout << "Roommate preference allocation completed." << endl;
}

// Reports and Statistics
void Admin::generateAllocationReport(const vector<Student>& students, 
                                     const vector<Room>& rooms) const {
    cout << "\n=== ALLOCATION REPORT ===" << endl;
    
    int totalStudents = students.size();
    int allocatedStudents = 0;
    int totalRooms = rooms.size();
    int occupiedRooms = 0;
    int totalCapacity = 0;
    int totalOccupancy = 0;
    
    for (const auto& student : students) {
        if (student.getIsAllocated()) allocatedStudents++;
    }
    
    for (const auto& room : rooms) {
        totalCapacity += room.getCapacity();
        totalOccupancy += room.getCurrentOccupancy();
        if (!room.isEmpty()) occupiedRooms++;
    }
    
    cout << "Total Students: " << totalStudents << endl;
    cout << "Allocated Students: " << allocatedStudents << endl;
    cout << "Unallocated Students: " << (totalStudents - allocatedStudents) << endl;
    cout << "Total Rooms: " << totalRooms << endl;
    cout << "Occupied Rooms: " << occupiedRooms << endl;
    cout << "Empty Rooms: " << (totalRooms - occupiedRooms) << endl;
    cout << "Total Capacity: " << totalCapacity << endl;
    cout << "Total Occupancy: " << totalOccupancy << endl;
    cout << "Occupancy Rate: " << fixed << setprecision(2) 
              << (totalCapacity > 0 ? (double)totalOccupancy / totalCapacity * 100 : 0) << "%" << endl;
}

void Admin::generateOccupancyReport(const vector<Room>& rooms) const {
    cout << "\n=== OCCUPANCY REPORT ===" << endl;
    
    map<string, int> typeCount;
    map<string, int> typeOccupancy;
    
    for (const auto& room : rooms) {
        typeCount[room.getRoomType()]++;
        typeOccupancy[room.getRoomType()] += room.getCurrentOccupancy();
    }
    
    cout << left << setw(12) << "Room Type" 
              << setw(10) << "Count" 
              << setw(12) << "Occupancy" << endl;
    cout << string(34, '-') << endl;
    
    for (const auto& pair : typeCount) {
        cout << left << setw(12) << pair.first
                  << setw(10) << pair.second
                  << setw(12) << typeOccupancy[pair.first] << endl;
    }
}

void Admin::generateStudentReport(const vector<Student>& students) const {
    cout << "\n=== STUDENT REPORT ===" << endl;
    
    map<string, int> courseCount;
    map<int, int> yearCount;
    
    for (const auto& student : students) {
        courseCount[student.getCourse()]++;
        yearCount[student.getYear()]++;
    }
    
    cout << "By Course:" << endl;
    cout << left << setw(20) << "Course" << setw(10) << "Count" << endl;
    cout << string(30, '-') << endl;
    for (const auto& pair : courseCount) {
        cout << left << setw(20) << pair.first << setw(10) << pair.second << endl;
    }
    
    cout << "\nBy Year:" << endl;
    cout << left << setw(10) << "Year" << setw(10) << "Count" << endl;
    cout << string(20, '-') << endl;
    for (const auto& pair : yearCount) {
        cout << left << setw(10) << pair.first << setw(10) << pair.second << endl;
    }
}

// Utility functions
void Admin::displayMenu() const {
    cout << "\n=== ADMIN MENU ===" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Room Management" << endl;
    cout << "3. Room Allocation" << endl;
    cout << "4. Reports" << endl;
    cout << "5. Change Password" << endl;
    cout << "6. Logout" << endl;
}

void Admin::inputDetails() {
    cout << "\n=== Enter Admin Details ===" << endl;
    cout << "Enter Admin ID: ";
    getline(cin, adminId);
    cout << "Enter Username: ";
    getline(cin, username);
    cout << "Enter Password: ";
    getline(cin, password);
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);
}

void Admin::displayInfo() const {
    cout << "\n=== Admin Information ===" << endl;
    cout << "Admin ID: " << adminId << endl;
    cout << "Username: " << username << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
}

// File operations (CSV)
void Admin::saveToFile(ostream& file) const {
    file
        << csvEscape(adminId) << ','
        << csvEscape(username) << ','
        << csvEscape(password) << ','
        << csvEscape(name) << ','
        << csvEscape(email)
        << '\n';
}

bool Admin::loadFromFile(istream& is) {
    string line;
    if (!getline(is, line)) return false;
    vector<string> tokens = csvParseLine(line);
    if (tokens.size() < 5) return false;
    adminId = tokens[0];
    username = tokens[1];
    password = tokens[2];
    name = tokens[3];
    email = tokens[4];
    return true;
}

// Output operator
ostream& operator<<(ostream& os, const Admin& admin) {
    admin.saveToFile(os);
    return os;
}

// Input operator
istream& operator>>(istream& is, Admin& admin) {
    admin.loadFromFile(is);
    return is;
}


