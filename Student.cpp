#include "Student.h"
#include <regex>
#include <algorithm>
#include <sstream>

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
        // Escape quotes by doubling them
        size_t pos = 0;
        while ((pos = out.find('"', pos)) != string::npos) {
            out.insert(pos, 1, '"');
            pos += 2;
        }
        if (needsQuotes) {
            return string("\"") + out + string("\"");
        }
        return out;
    }

    vector<string> csvParseLine(const string& line) {
        vector<string> result;
        string cur;
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        cur.push_back('"');
                        ++i; // skip escaped quote
                    } else {
                        inQuotes = false;
                    }
                } else {
                    cur.push_back(c);
                }
            } else {
                if (c == ',') {
                    result.push_back(cur);
                    cur.clear();
                } else if (c == '"') {
                    inQuotes = true;
                } else {
                    cur.push_back(c);
                }
            }
        }
        result.push_back(cur);
        for (auto& t : result) t = trim(t);
        return result;
    }
}

// Default constructor
Student::Student() : studentId(""), name(""), email(""), phone(""), 
                     course(""), year(0), preferredRoommate(""), 
                     allocatedRoom(""), isAllocated(false) {}

// Parameterized constructor
Student::Student(const string& id, const string& name, const string& email, 
                 const string& phone, const string& course, int year)
    : studentId(id), name(name), email(email), phone(phone), course(course), 
      year(year), preferredRoommate(""), allocatedRoom(""), isAllocated(false) {}

// Getters
string Student::getStudentId() const { return studentId; }
string Student::getName() const { return name; }
string Student::getEmail() const { return email; }
string Student::getPhone() const { return phone; }
string Student::getCourse() const { return course; }
int Student::getYear() const { return year; }
string Student::getPreferredRoommate() const { return preferredRoommate; }
string Student::getAllocatedRoom() const { return allocatedRoom; }
bool Student::getIsAllocated() const { return isAllocated; }
vector<string> Student::getPreferences() const { return preferences; }

// Setters
void Student::setStudentId(const string& id) { studentId = id; }
void Student::setName(const string& name) { this->name = name; }
void Student::setEmail(const string& email) { this->email = email; }
void Student::setPhone(const string& phone) { this->phone = phone; }
void Student::setCourse(const string& course) { this->course = course; }
void Student::setYear(int year) { this->year = year; }
void Student::setPreferredRoommate(const string& roommate) { preferredRoommate = roommate; }
void Student::setAllocatedRoom(const string& room) { 
    allocatedRoom = room; 
    isAllocated = (room != "");
}
void Student::setIsAllocated(bool allocated) { isAllocated = allocated; }

void Student::addPreference(const string& room) {
    if (find(preferences.begin(), preferences.end(), room) == preferences.end()) {
        preferences.push_back(room);
    }
}

// Display student information
void Student::displayInfo() const {
    cout << "\n=== Student Information ===" << endl;
    cout << "Student ID: " << studentId << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Course: " << course << endl;
    cout << "Year: " << year << endl;
    cout << "Preferred Roommate: " << (preferredRoommate.empty() ? "None" : preferredRoommate) << endl;
    cout << "Allocated Room: " << (allocatedRoom.empty() ? "Not Allocated" : allocatedRoom) << endl;
    cout << "Status: " << (isAllocated ? "Allocated" : "Not Allocated") << endl;
    
    if (!preferences.empty()) {
        cout << "Room Preferences: ";
        for (size_t i = 0; i < preferences.size(); ++i) {
            cout << preferences[i];
            if (i < preferences.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// Input student details
void Student::inputDetails() {
    cout << "\n=== Enter Student Details ===" << endl;
    
    cout << "Enter Student ID: ";
    getline(cin, studentId);
    
    cout << "Enter Name: ";
    getline(cin, name);
    
    do {
        cout << "Enter Email: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please try again." << endl;
        }
    } while (!isValidEmail(email));
    
    do {
        cout << "Enter Phone: ";
        getline(cin, phone);
        if (!isValidPhone(phone)) {
            cout << "Invalid phone format. Please try again." << endl;
        }
    } while (!isValidPhone(phone));
    
    cout << "Enter Course: ";
    getline(cin, course);
    
    do {
        cout << "Enter Year (1-4): ";
        cin >> year;
        cin.ignore();
        if (year < 1 || year > 4) {
            cout << "Invalid year. Please enter 1-4." << endl;
        }
    } while (year < 1 || year > 4);
    
    cout << "Enter Preferred Roommate ID (or press Enter for none): ";
    getline(cin, preferredRoommate);
}

// Email validation
bool Student::isValidEmail(const string& email) const {
    regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailRegex);
}

// Phone validation
bool Student::isValidPhone(const string& phone) const {
    regex phoneRegex(R"(\d{10})");
    return regex_match(phone, phoneRegex);
}

// Save to CSV file
void Student::saveToFile(ofstream& file) const {
    // Join preferences with ';' to keep single CSV cell
    string prefs;
    for (size_t i = 0; i < preferences.size(); ++i) {
        if (i) prefs += ';';
        prefs += preferences[i];
    }
    file
        << csvEscape(studentId) << ','
        << csvEscape(name) << ','
        << csvEscape(email) << ','
        << csvEscape(phone) << ','
        << csvEscape(course) << ','
        << year << ','
        << csvEscape(preferredRoommate) << ','
        << csvEscape(allocatedRoom) << ','
        << (isAllocated ? 1 : 0) << ','
        << csvEscape(prefs)
        << '\n';
}

// Load from CSV (generic istream); returns success
bool Student::loadFromFile(istream& is) {
    string line;
    if (!getline(is, line)) return false;
    vector<string> tokens = csvParseLine(line);
    if (tokens.size() < 9) return false;
    studentId = tokens[0];
    name = tokens[1];
    email = tokens[2];
    phone = tokens[3];
    course = tokens[4];
    year = tokens[5].empty() ? 0 : stoi(tokens[5]);
    preferredRoommate = tokens[6];
    allocatedRoom = tokens[7];
    isAllocated = (!tokens[8].empty() && tokens[8] != "0");
    preferences.clear();
    if (tokens.size() > 9 && !tokens[9].empty()) {
        stringstream prefStream(tokens[9]);
        string pref;
        while (getline(prefStream, pref, ';')) {
            pref = trim(pref);
            if (!pref.empty()) preferences.push_back(pref);
        }
    }
    return true;
}

void Student::saveToFile(ostream& os) const {
    string prefs;
    for (size_t i = 0; i < preferences.size(); ++i) {
        if (i) prefs += ';';
        prefs += preferences[i];
    }
    os
        << csvEscape(studentId) << ','
        << csvEscape(name) << ','
        << csvEscape(email) << ','
        << csvEscape(phone) << ','
        << csvEscape(course) << ','
        << year << ','
        << csvEscape(preferredRoommate) << ','
        << csvEscape(allocatedRoom) << ','
        << (isAllocated ? 1 : 0) << ','
        << csvEscape(prefs)
        << '\n';
}

// Output operator
ostream& operator<<(ostream& os, const Student& student) {
    student.saveToFile(os);
    return os;
}

// Input operator
istream& operator>>(istream& is, Student& student) {
    student.loadFromFile(is);
    return is;
}
