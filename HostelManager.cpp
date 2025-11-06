#include "HostelManager.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <unordered_set>

using namespace std;

// Constructor
HostelManager::HostelManager() : currentAdmin(nullptr), 
                                 studentsFile("students.csv"), 
                                 roomsFile("rooms.csv"), 
                                 adminsFile("admins.csv") {
    loadData();
}

// File Operations
void HostelManager::loadData() {
    try {
        loadAdmins();
        loadStudents();
        loadRooms();
        cout << "Data loaded successfully!" << endl;
    } catch (const exception& e) {
        cout << "Error loading data: " << e.what() << endl;
    }
}

void HostelManager::saveData() {
    try {
        saveAdmins();
        saveStudents();
        saveRooms();
        cout << "Data saved successfully!" << endl;
    } catch (const exception& e) {
        cout << "Error saving data: " << e.what() << endl;
    }
}

void HostelManager::loadStudents() {
    ifstream file(studentsFile);
    if (!file.is_open()) {
        // File doesn't exist, create empty file
        ofstream newFile(studentsFile);
        newFile.close();
        return;
    }
    
    students.clear();
    Student student;
    unordered_set<string> seenIds;
    while (student.loadFromFile(file)) {
        if (!student.getStudentId().empty()) {
            if (seenIds.insert(student.getStudentId()).second) {
                students.push_back(student);
            } else {
                // Duplicate ID encountered in file; skip it
            }
        }
    }
    file.close();
}

void HostelManager::saveStudents() {
    ofstream file(studentsFile);
    if (!file.is_open()) {
        handleFileError("save", studentsFile);
        return;
    }
    
    for (const auto& student : students) {
        student.saveToFile(file);
    }
    file.close();
}

void HostelManager::loadRooms() {
    ifstream file(roomsFile);
    if (!file.is_open()) {
        // File doesn't exist, create empty file
        ofstream newFile(roomsFile);
        newFile.close();
        return;
    }
    
    rooms.clear();
    Room room;
    while (room.loadFromFile(file)) {
        if (!room.getRoomNumber().empty()) {
            rooms.push_back(room);
        }
    }
    file.close();
}

void HostelManager::saveRooms() {
    ofstream file(roomsFile);
    if (!file.is_open()) {
        handleFileError("save", roomsFile);
        return;
    }
    
    for (const auto& room : rooms) {
        room.saveToFile(file);
    }
    file.close();
}

void HostelManager::loadAdmins() {
    ifstream file(adminsFile);
    if (!file.is_open()) {
        // Create default admin if file doesn't exist
        Admin defaultAdmin("ADM001", "admin", "admin123", "System Administrator", "admin@hostel.com");
        admins.push_back(defaultAdmin);
        saveAdmins();
        return;
    }
    
    admins.clear();
    Admin admin;
    while (admin.loadFromFile(file)) {
        if (!admin.getAdminId().empty()) {
            admins.push_back(admin);
        }
    }
    file.close();
}

void HostelManager::saveAdmins() {
    ofstream file(adminsFile);
    if (!file.is_open()) {
        handleFileError("save", adminsFile);
        return;
    }
    
    for (const auto& admin : admins) {
        admin.saveToFile(file);
    }
    file.close();
}

// Authentication
bool HostelManager::adminLogin() {
    string username, password;
    
    cout << "\n=== ADMIN LOGIN ===" << endl;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    
    for (auto& admin : admins) {
        if (admin.authenticate(username, password)) {
            currentAdmin = &admin;
            cout << "Login successful! Welcome, " << admin.getName() << endl;
            return true;
        }
    }
    
    cout << "Invalid username or password!" << endl;
    return false;
}

void HostelManager::adminLogout() {
    if (currentAdmin) {
        cout << "Logging out " << currentAdmin->getName() << endl;
        currentAdmin = nullptr;
    }
}

bool HostelManager::isAdminLoggedIn() const {
    return currentAdmin != nullptr;
}

// Student Operations
void HostelManager::addStudent() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->addStudent(students);
    saveStudents();
}

void HostelManager::removeStudent() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string studentId;
    cout << "Enter Student ID to remove: ";
    getline(cin, studentId);
    
    currentAdmin->removeStudent(students, studentId);
    saveStudents();
}

void HostelManager::updateStudent() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string studentId;
    cout << "Enter Student ID to update: ";
    getline(cin, studentId);
    
    currentAdmin->updateStudent(students, studentId);
    saveStudents();
}

void HostelManager::displayAllStudents() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->displayAllStudents(students);
}

void HostelManager::searchStudent() {
    string studentId;
    cout << "Enter Student ID to search: ";
    getline(cin, studentId);
    
    Student* student = currentAdmin->findStudent(students, studentId);
    if (student) {
        student->displayInfo();
    } else {
        cout << "Student not found!" << endl;
    }
}

void HostelManager::displayStudentDetails() {
    searchStudent();
}

// Room Operations
void HostelManager::addRoom() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->addRoom(rooms);
    saveRooms();
}

void HostelManager::removeRoom() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string roomNumber;
    cout << "Enter Room Number to remove: ";
    getline(cin, roomNumber);
    
    currentAdmin->removeRoom(rooms, roomNumber);
    saveRooms();
}

void HostelManager::updateRoom() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string roomNumber;
    cout << "Enter Room Number to update: ";
    getline(cin, roomNumber);
    
    currentAdmin->updateRoom(rooms, roomNumber);
    saveRooms();
}

void HostelManager::displayAllRooms() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->displayAllRooms(rooms);
}

void HostelManager::searchRoom() {
    string roomNumber;
    cout << "Enter Room Number to search: ";
    getline(cin, roomNumber);
    
    Room* room = currentAdmin->findRoom(rooms, roomNumber);
    if (room) {
        room->displayInfo();
    } else {
        cout << "Room not found!" << endl;
    }
}

void HostelManager::displayRoomDetails() {
    searchRoom();
}

// Allocation Operations
void HostelManager::allocateRoom() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string studentId, roomNumber;
    cout << "Enter Student ID: ";
    getline(cin, studentId);
    cout << "Enter Room Number: ";
    getline(cin, roomNumber);
    
    if (currentAdmin->allocateRoom(students, rooms, studentId, roomNumber)) {
        saveStudents();
        saveRooms();
    }
}

void HostelManager::deallocateRoom() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string studentId;
    cout << "Enter Student ID: ";
    getline(cin, studentId);
    
    if (currentAdmin->deallocateRoom(students, rooms, studentId)) {
        saveStudents();
        saveRooms();
    }
}

void HostelManager::autoAllocateRooms() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->autoAllocateRooms(students, rooms);
    saveStudents();
    saveRooms();
}

void HostelManager::allocateWithRoommatePreference() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->allocateWithRoommatePreference(students, rooms);
    saveStudents();
    saveRooms();
}

// Report Operations
void HostelManager::generateAllocationReport() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->generateAllocationReport(students, rooms);
}

void HostelManager::generateOccupancyReport() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->generateOccupancyReport(rooms);
}

void HostelManager::generateStudentReport() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    currentAdmin->generateStudentReport(students);
}

// Admin Operations
void HostelManager::changeAdminPassword() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    string newPassword;
    cout << "Enter new password: ";
    getline(cin, newPassword);
    
    currentAdmin->changePassword(newPassword);
    saveAdmins();
    cout << "Password changed successfully!" << endl;
}

void HostelManager::addAdmin() {
    if (!isAdminLoggedIn()) {
        cout << "Please login as admin first!" << endl;
        return;
    }
    
    Admin newAdmin;
    newAdmin.inputDetails();
    admins.push_back(newAdmin);
    saveAdmins();
    cout << "Admin added successfully!" << endl;
}

// User Interface
void HostelManager::displayMainMenu() {
    cout << "\n=== HOSTEL ALLOTMENT SYSTEM ===" << endl;
    cout << "1. Admin Login" << endl;
    cout << "2. Student Portal" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

void HostelManager::displayAdminMenu() {
    cout << "\n=== ADMIN PANEL ===" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Room Management" << endl;
    cout << "3. Room Allocation" << endl;
    cout << "4. Reports" << endl;
    cout << "5. Admin Settings" << endl;
    cout << "6. Logout" << endl;
    cout << "Enter your choice: ";
}

void HostelManager::displayStudentMenu() {
    cout << "\n=== STUDENT PORTAL ===" << endl;
    cout << "1. View My Details" << endl;
    cout << "2. View Available Rooms" << endl;
    cout << "3. View Room Details" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
}

void HostelManager::run() {
    string choice;
    
    while (true) {
        displayMainMenu();
        getline(cin, choice);
        
        if (choice == "1") {
            if (adminLogin()) {
                adminPanel();
            }
        } else if (choice == "2") {
            studentPanel();
        } else if (choice == "3") {
            cout << "Saving data..." << endl;
            saveData();
            cout << "Thank you for using Hostel Allotment System!" << endl;
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void HostelManager::adminPanel() {
    string choice;
    
    while (isAdminLoggedIn()) {
        displayAdminMenu();
        getline(cin, choice);
        
        if (choice == "1") {
            studentManagement();
        } else if (choice == "2") {
            roomManagement();
        } else if (choice == "3") {
            allocationManagement();
        } else if (choice == "4") {
            reportsManagement();
        } else if (choice == "5") {
            adminSettings();
        } else if (choice == "6") {
            adminLogout();
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void HostelManager::studentPanel() {
    string choice;
    
    while (true) {
        displayStudentMenu();
        getline(cin, choice);
        
        if (choice == "1") {
            displayStudentDetails();
        } else if (choice == "2") {
            displayAllRooms();
        } else if (choice == "3") {
            displayRoomDetails();
        } else if (choice == "4") {
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void HostelManager::studentManagement() {
    string choice;
    
    while (true) {
        cout << "\n=== STUDENT MANAGEMENT ===" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Remove Student" << endl;
        cout << "3. Update Student" << endl;
        cout << "4. Display All Students" << endl;
        cout << "5. Search Student" << endl;
        cout << "6. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);
        
        if (choice == "1") addStudent();
        else if (choice == "2") removeStudent();
        else if (choice == "3") updateStudent();
        else if (choice == "4") displayAllStudents();
        else if (choice == "5") searchStudent();
        else if (choice == "6") break;
        else cout << "Invalid choice! Please try again." << endl;
        
        pause();
    }
}

void HostelManager::roomManagement() {
    string choice;
    
    while (true) {
        cout << "\n=== ROOM MANAGEMENT ===" << endl;
        cout << "1. Add Room" << endl;
        cout << "2. Remove Room" << endl;
        cout << "3. Update Room" << endl;
        cout << "4. Display All Rooms" << endl;
        cout << "5. Search Room" << endl;
        cout << "6. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);
        
        if (choice == "1") addRoom();
        else if (choice == "2") removeRoom();
        else if (choice == "3") updateRoom();
        else if (choice == "4") displayAllRooms();
        else if (choice == "5") searchRoom();
        else if (choice == "6") break;
        else cout << "Invalid choice! Please try again." << endl;
        
        pause();
    }
}

void HostelManager::allocationManagement() {
    string choice;
    
    while (true) {
        cout << "\n=== ALLOCATION MANAGEMENT ===" << endl;
        cout << "1. Allocate Room" << endl;
        cout << "2. Deallocate Room" << endl;
        cout << "3. Auto Allocate Rooms" << endl;
        cout << "4. Allocate with Roommate Preference" << endl;
        cout << "5. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);
        
        if (choice == "1") allocateRoom();
        else if (choice == "2") deallocateRoom();
        else if (choice == "3") autoAllocateRooms();
        else if (choice == "4") allocateWithRoommatePreference();
        else if (choice == "5") break;
        else cout << "Invalid choice! Please try again." << endl;
        
        pause();
    }
}

void HostelManager::reportsManagement() {
    string choice;
    
    while (true) {
        cout << "\n=== REPORTS ===" << endl;
        cout << "1. Allocation Report" << endl;
        cout << "2. Occupancy Report" << endl;
        cout << "3. Student Report" << endl;
        cout << "4. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);
        
        if (choice == "1") generateAllocationReport();
        else if (choice == "2") generateOccupancyReport();
        else if (choice == "3") generateStudentReport();
        else if (choice == "4") break;
        else cout << "Invalid choice! Please try again." << endl;
        
        pause();
    }
}

void HostelManager::adminSettings() {
    string choice;
    
    while (true) {
        cout << "\n=== ADMIN SETTINGS ===" << endl;
        cout << "1. Change Password" << endl;
        cout << "2. Add New Admin" << endl;
        cout << "3. Back to Admin Menu" << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);
        
        if (choice == "1") changeAdminPassword();
        else if (choice == "2") addAdmin();
        else if (choice == "3") break;
        else cout << "Invalid choice! Please try again." << endl;
        
        pause();
    }
}

// Utility Functions
void HostelManager::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void HostelManager::pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
}

bool HostelManager::isValidChoice(const string& choice, int maxChoice) const {
    if (choice.length() != 1) return false;
    int choiceNum = choice[0] - '0';
    return (choiceNum >= 1 && choiceNum <= maxChoice);
}

// Error Handling
void HostelManager::handleFileError(const string& operation, const string& filename) const {
    cout << "Error: Could not " << operation << " file: " << filename << endl;
}

void HostelManager::handleInputError(const string& message) const {
    cout << "Input Error: " << message << endl;
}

void HostelManager::handleSystemError(const string& message) const {
    cout << "System Error: " << message << endl;
}
