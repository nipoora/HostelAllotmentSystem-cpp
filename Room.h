/**
 * @file Room.h
 * @brief Room Class Definition for Hostel Allotment System
 * 
 * This class represents a room in the hostel management system.
 * It manages room details, capacity, occupancy, and student assignments.
 * 
 * Key Features:
 * - Room information management (type, capacity, rent, location)
 * - Occupancy tracking and availability management
 * - Student occupant management
 * - Room type validation
 * - File I/O operations for data persistence
 * 
 * @author Student Developer
 * @version 1.0
 * @date 2024
 */

#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Room {
private:
    string roomNumber;
    string roomType; // Single, Double, Triple, Quad
    int capacity;
    int currentOccupancy;
    vector<string> occupants; // Student IDs
    bool isAvailable;
    double rent;
    string floor;
    string block;

public:
    // Constructors
    Room();
    Room(const string& roomNo, const string& type, int cap, 
         double rent, const string& floor, const string& block);
    
    // Getters
    string getRoomNumber() const;
    string getRoomType() const;
    int getCapacity() const;
    int getCurrentOccupancy() const;
    vector<string> getOccupants() const;
    bool getIsAvailable() const;
    double getRent() const;
    string getFloor() const;
    string getBlock() const;
    int getAvailableSlots() const;
    
    // Setters
    void setRoomNumber(const string& roomNo);
    void setRoomType(const string& type);
    void setCapacity(int cap);
    void setRent(double rent);
    void setFloor(const string& floor);
    void setBlock(const string& block);
    
    // Room management functions
    bool addOccupant(const string& studentId);
    bool removeOccupant(const string& studentId);
    bool isFull() const;
    bool isEmpty() const;
    void updateAvailability();
    
    // Utility functions
    void displayInfo() const;
    void inputDetails();
    bool isValidRoomType(const string& type) const;
    
    // File operations
    void saveToFile(ostream& os) const;
    bool loadFromFile(istream& is);
    
    // Friend functions for file operations
    friend ostream& operator<<(ostream& os, const Room& room);
    friend istream& operator>>(istream& is, Room& room);
};

#endif // ROOM_H

