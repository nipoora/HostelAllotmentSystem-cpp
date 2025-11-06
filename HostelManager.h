/**
 * @file HostelManager.h
 * @brief Main Controller Class for Hostel Allotment System
 * 
 * This class serves as the main controller for the entire hostel management system.
 * It coordinates all operations including student management, room management,
 * allocation algorithms, and file operations.
 * 
 * Key Responsibilities:
 * - File I/O operations (load/save data)
 * - User authentication and session management
 * - Student CRUD operations
 * - Room CRUD operations
 * - Room allocation algorithms (manual, auto, preference-based)
 * - Report generation and analytics
 * - User interface management
 * 
 * @author Student Developer
 * @version 1.0
 * @date 2024
 */

#ifndef HOSTELMANAGER_H
#define HOSTELMANAGER_H

#include "Student.h"
#include "Room.h"
#include "Admin.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class HostelManager {
private:
    vector<Student> students;
    vector<Room> rooms;
    vector<Admin> admins;
    Admin* currentAdmin;
    string studentsFile;
    string roomsFile;
    string adminsFile;

public:
    // Constructor
    HostelManager();
    
    // File Operations
    void loadData();
    void saveData();
    void loadStudents();
    void saveStudents();
    void loadRooms();
    void saveRooms();
    void loadAdmins();
    void saveAdmins();
    
    // Authentication
    bool adminLogin();
    void adminLogout();
    bool isAdminLoggedIn() const;
    
    // Student Operations
    void addStudent();
    void removeStudent();
    void updateStudent();
    void displayAllStudents();
    void searchStudent();
    void displayStudentDetails();
    
    // Room Operations
    void addRoom();
    void removeRoom();
    void updateRoom();
    void displayAllRooms();
    void searchRoom();
    void displayRoomDetails();
    
    // Allocation Operations
    void allocateRoom();
    void deallocateRoom();
    void autoAllocateRooms();
    void allocateWithRoommatePreference();
    
    // Report Operations
    void generateAllocationReport();
    void generateOccupancyReport();
    void generateStudentReport();
    
    // Admin Operations
    void changeAdminPassword();
    void addAdmin();
    
    // User Interface
    void displayMainMenu();
    void displayAdminMenu();
    void displayStudentMenu();
    void run();
    void adminPanel();
    void studentPanel();
    void studentManagement();
    void roomManagement();
    void allocationManagement();
    void reportsManagement();
    void adminSettings();
    
    // Utility Functions
    void clearScreen();
    void pause();
    bool isValidChoice(const string& choice, int maxChoice) const;
    
    // Error Handling
    void handleFileError(const string& operation, const string& filename) const;
    void handleInputError(const string& message) const;
    void handleSystemError(const string& message) const;
};

#endif // HOSTELMANAGER_H
