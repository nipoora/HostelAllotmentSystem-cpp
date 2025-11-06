/**
 * @file Admin.h
 * @brief Admin Class Definition for Hostel Allotment System
 * 
 * This class represents an administrator in the hostel management system.
 * It provides administrative functions for managing students, rooms, and allocations.
 * 
 * Key Features:
 * - Administrator authentication and session management
 * - Student management (CRUD operations)
 * - Room management (CRUD operations)
 * - Room allocation algorithms (manual, auto, preference-based)
 * - Comprehensive reporting and analytics
 * - Admin account management
 * 
 * @author Student Developer
 * @version 1.0
 * @date 2024
 */

#ifndef ADMIN_H
#define ADMIN_H

#include "Student.h"
#include "Room.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class Admin {
private:
    string adminId;
    string username;
    string password;
    string name;
    string email;

public:
    // Constructors
    Admin();
    Admin(const string& id, const string& username, const string& password,
          const string& name, const string& email);
    
    // Getters
    string getAdminId() const;
    string getUsername() const;
    string getName() const;
    string getEmail() const;
    
    // Authentication
    bool authenticate(const string& username, const string& password) const;
    void changePassword(const string& newPassword);
    
    // Student Management
    void addStudent(vector<Student>& students);
    void removeStudent(vector<Student>& students, const string& studentId);
    void updateStudent(vector<Student>& students, const string& studentId);
    void displayAllStudents(const vector<Student>& students) const;
    Student* findStudent(vector<Student>& students, const string& studentId);
    
    // Room Management
    void addRoom(vector<Room>& rooms);
    void removeRoom(vector<Room>& rooms, const string& roomNumber);
    void updateRoom(vector<Room>& rooms, const string& roomNumber);
    void displayAllRooms(const vector<Room>& rooms) const;
    Room* findRoom(vector<Room>& rooms, const string& roomNumber);
    
    // Room Allocation
    bool allocateRoom(vector<Student>& students, vector<Room>& rooms, 
                      const string& studentId, const string& roomNumber);
    bool deallocateRoom(vector<Student>& students, vector<Room>& rooms, 
                        const string& studentId);
    void autoAllocateRooms(vector<Student>& students, vector<Room>& rooms);
    void allocateWithRoommatePreference(vector<Student>& students, vector<Room>& rooms);
    
    // Reports and Statistics
    void generateAllocationReport(const vector<Student>& students, 
                                  const vector<Room>& rooms) const;
    void generateOccupancyReport(const vector<Room>& rooms) const;
    void generateStudentReport(const vector<Student>& students) const;
    
    // Utility functions
    void displayMenu() const;
    void inputDetails();
    void displayInfo() const;
    
    // File operations
    void saveToFile(ostream& os) const;
    bool loadFromFile(istream& is);
    
    // Friend functions for file operations
    friend ostream& operator<<(ostream& os, const Admin& admin);
    friend istream& operator>>(istream& is, Admin& admin);
};

#endif // ADMIN_H


