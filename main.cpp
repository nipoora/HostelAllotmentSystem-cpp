/**
 * @file main.cpp
 * @brief Hostel Allotment System - Main Application Entry Point
 * 
 * This is the main entry point for the Hostel Allotment System.
 * The system manages student room allocations with advanced features
 * including roommate preferences and automated allocation.
 * 
 * @author Student Developer
 * @version 1.0
 * @date 2024
 */

#include "HostelManager.h"
#include <iostream>
#include <exception>

using namespace std;

/**
 * @brief Main function - Application entry point
 * 
 * This function:
 * 1. Displays welcome message
 * 2. Creates HostelManager instance
 * 3. Runs the main application loop
 * 4. Handles exceptions gracefully
 * 
 * @return int Exit status (0 for success, 1 for error)
 */
int main() {
    try {
        // Display welcome message
        cout << "================================================" << endl;
        cout << "    WELCOME TO HOSTEL ALLOTMENT SYSTEM" << endl;
        cout << "================================================" << endl;
        cout << "A comprehensive system for managing hostel room" << endl;
        cout << "allocations with roommate preferences and" << endl;
        cout << "automated assignment capabilities." << endl;
        cout << "================================================" << endl;
        
        // Create and run the hostel management system
        HostelManager hostelSystem;
        hostelSystem.run();
        
    } catch (const exception& e) {
        // Handle standard exceptions
        cerr << "Fatal Error: " << e.what() << endl;
        cerr << "The application will now exit." << endl;
        return 1;
    } catch (...) {
        // Handle any other unexpected exceptions
        cerr << "Unknown error occurred. The application will now exit." << endl;
        return 1;
    }
    
    return 0;
}


