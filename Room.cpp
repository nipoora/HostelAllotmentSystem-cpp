#include "Room.h"
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
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        cur.push_back('"'); ++i;
                    } else { inQuotes = false; }
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
Room::Room() : roomNumber(""), roomType(""), capacity(0), currentOccupancy(0), 
               isAvailable(true), rent(0.0), floor(""), block("") {}

// Parameterized constructor
Room::Room(const string& roomNo, const string& type, int cap, 
           double rent, const string& floor, const string& block)
    : roomNumber(roomNo), roomType(type), capacity(cap), currentOccupancy(0), 
      isAvailable(true), rent(rent), floor(floor), block(block) {}

// Getters
string Room::getRoomNumber() const { return roomNumber; }
string Room::getRoomType() const { return roomType; }
int Room::getCapacity() const { return capacity; }
int Room::getCurrentOccupancy() const { return currentOccupancy; }
vector<string> Room::getOccupants() const { return occupants; }
bool Room::getIsAvailable() const { return isAvailable; }
double Room::getRent() const { return rent; }
string Room::getFloor() const { return floor; }
string Room::getBlock() const { return block; }

int Room::getAvailableSlots() const {
    return capacity - currentOccupancy;
}

// Setters
void Room::setRoomNumber(const string& roomNo) { roomNumber = roomNo; }
void Room::setRoomType(const string& type) { roomType = type; }
void Room::setCapacity(int cap) { 
    capacity = cap; 
    updateAvailability();
}
void Room::setRent(double rent) { this->rent = rent; }
void Room::setFloor(const string& floor) { this->floor = floor; }
void Room::setBlock(const string& block) { this->block = block; }

// Add occupant to room
bool Room::addOccupant(const string& studentId) {
    if (isFull()) {
        return false;
    }
    
    // Check if student is already in the room
    if (find(occupants.begin(), occupants.end(), studentId) != occupants.end()) {
        return false;
    }
    
    occupants.push_back(studentId);
    currentOccupancy++;
    updateAvailability();
    return true;
}

// Remove occupant from room
bool Room::removeOccupant(const string& studentId) {
    auto it = find(occupants.begin(), occupants.end(), studentId);
    if (it != occupants.end()) {
        occupants.erase(it);
        currentOccupancy--;
        updateAvailability();
        return true;
    }
    return false;
}

// Check if room is full
bool Room::isFull() const {
    return currentOccupancy >= capacity;
}

// Check if room is empty
bool Room::isEmpty() const {
    return currentOccupancy == 0;
}

// Update room availability
void Room::updateAvailability() {
    isAvailable = (currentOccupancy < capacity);
}

// Display room information
void Room::displayInfo() const {
    cout << "\n=== Room Information ===" << endl;
    cout << "Room Number: " << roomNumber << endl;
    cout << "Room Type: " << roomType << endl;
    cout << "Capacity: " << capacity << endl;
    cout << "Current Occupancy: " << currentOccupancy << endl;
    cout << "Available Slots: " << getAvailableSlots() << endl;
    cout << "Rent: $" << rent << endl;
    cout << "Floor: " << floor << endl;
    cout << "Block: " << block << endl;
    cout << "Status: " << (isAvailable ? "Available" : "Full") << endl;
    
    if (!occupants.empty()) {
        cout << "Occupants: ";
        for (size_t i = 0; i < occupants.size(); ++i) {
            cout << occupants[i];
            if (i < occupants.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// Input room details
void Room::inputDetails() {
    cout << "\n=== Enter Room Details ===" << endl;
    
    cout << "Enter Room Number: ";
    getline(cin, roomNumber);
    
    do {
        cout << "Enter Room Type (Single/Double/Triple/Quad): ";
        getline(cin, roomType);
        if (!isValidRoomType(roomType)) {
            cout << "Invalid room type. Please enter Single, Double, Triple, or Quad." << endl;
        }
    } while (!isValidRoomType(roomType));
    
    do {
        cout << "Enter Capacity (1-4): ";
        cin >> capacity;
        cin.ignore();
        if (capacity < 1 || capacity > 4) {
            cout << "Invalid capacity. Please enter 1-4." << endl;
        }
    } while (capacity < 1 || capacity > 4);
    
    cout << "Enter Monthly Rent: $";
    cin >> rent;
    cin.ignore();
    
    cout << "Enter Floor: ";
    getline(cin, floor);
    
    cout << "Enter Block: ";
    getline(cin, block);
    
    currentOccupancy = 0;
    updateAvailability();
}

// Validate room type
bool Room::isValidRoomType(const string& type) const {
    string lowerType = type;
    transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
    
    return (lowerType == "single" || lowerType == "double" || 
            lowerType == "triple" || lowerType == "quad");
}

// Save to CSV file
void Room::saveToFile(ostream& file) const {
    // Join occupants with ';' inside one CSV field
    string occ;
    for (size_t i = 0; i < occupants.size(); ++i) {
        if (i) occ += ';';
        occ += occupants[i];
    }
    file
        << csvEscape(roomNumber) << ','
        << csvEscape(roomType) << ','
        << capacity << ','
        << currentOccupancy << ','
        << rent << ','
        << csvEscape(floor) << ','
        << csvEscape(block) << ','
        << (isAvailable ? 1 : 0) << ','
        << csvEscape(occ)
        << '\n';
}

// Load from CSV (generic istream); returns success
bool Room::loadFromFile(istream& is) {
    string line;
    if (!getline(is, line)) return false;
    vector<string> tokens = csvParseLine(line);
    if (tokens.size() < 8) return false;
    roomNumber = tokens[0];
    roomType = tokens[1];
    capacity = tokens[2].empty() ? 0 : stoi(tokens[2]);
    currentOccupancy = tokens[3].empty() ? 0 : stoi(tokens[3]);
    rent = tokens[4].empty() ? 0.0 : stod(tokens[4]);
    floor = tokens[5];
    block = tokens[6];
    isAvailable = (!tokens[7].empty() && tokens[7] != "0");
    occupants.clear();
    if (tokens.size() > 8 && !tokens[8].empty()) {
        stringstream occStream(tokens[8]);
        string occupant;
        while (getline(occStream, occupant, ';')) {
            occupant = trim(occupant);
            if (!occupant.empty()) occupants.push_back(occupant);
        }
    }
    return true;
}

// Output operator
ostream& operator<<(ostream& os, const Room& room) {
    string occ;
    for (size_t i = 0; i < room.occupants.size(); ++i) {
        if (i) occ += ';';
        occ += room.occupants[i];
    }
    os
        << csvEscape(room.roomNumber) << ','
        << csvEscape(room.roomType) << ','
        << room.capacity << ','
        << room.currentOccupancy << ','
        << room.rent << ','
        << csvEscape(room.floor) << ','
        << csvEscape(room.block) << ','
        << (room.isAvailable ? 1 : 0) << ','
        << csvEscape(occ)
        << '\n';
    return os;
}

// Input operator
istream& operator>>(istream& is, Room& room) {
    room.loadFromFile(is);
    return is;
}

