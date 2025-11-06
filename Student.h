/**
 * @file Student.h
 * @brief Student Class Definition for Hostel Allotment System
 * 
 * This class represents a student in the hostel management system.
 * It manages student information, preferences, and room allocation status.
 * 
 * Key Features:
 * - Complete student information management
 * - Email and phone number validation
 * - Roommate preference handling
 * - Room allocation tracking
 * - File I/O operations for data persistence
 * 
 * @author Student Developer
 * @version 1.0
 * @date 2024
 */

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Student {
private:
    string studentId;
    string name;
    string email;
    string phone;
    string course;
    int year;
    string preferredRoommate;
    string allocatedRoom;
    bool isAllocated;
    vector<string> preferences; // Room preferences

public:
    // Constructors
    Student();
    Student(const string& id, const string& name, const string& email, 
            const string& phone, const string& course, int year);
    
    // Getters
    string getStudentId() const;
    string getName() const;
    string getEmail() const;
    string getPhone() const;
    string getCourse() const;
    int getYear() const;
    string getPreferredRoommate() const;
    string getAllocatedRoom() const;
    bool getIsAllocated() const;
    vector<string> getPreferences() const;
    
    // Setters
    void setStudentId(const string& id);
    void setName(const string& name);
    void setEmail(const string& email);
    void setPhone(const string& phone);
    void setCourse(const string& course);
    void setYear(int year);
    void setPreferredRoommate(const string& roommate);
    void setAllocatedRoom(const string& room);
    void setIsAllocated(bool allocated);
    void addPreference(const string& room);
    
    // Utility functions
    void displayInfo() const;
    void inputDetails();
    bool isValidEmail(const string& email) const;
    bool isValidPhone(const string& phone) const;
    
    // File operations
    void saveToFile(ostream& os) const;
    bool loadFromFile(istream& is);
    
    // Friend functions for file operations
    friend ostream& operator<<(ostream& os, const Student& student);
    friend istream& operator>>(istream& is, Student& student);
};

#endif // STUDENT_H

