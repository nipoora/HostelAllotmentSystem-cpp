# Hostel Allotment System (C++)

Simple, menu-driven console app to manage students, rooms, and allocations. Data is stored in CSV files.

## Quick Start

1) Build

```bash
# Windows
build.bat

# Linux/macOS
g++ -std=c++11 -Wall -Wextra -O2 -o hostel_system main.cpp Student.cpp Room.cpp Admin.cpp HostelManager.cpp
```

2) Run

```bash
# Windows
run.bat

# Linux/macOS
./hostel_system
```

3) Admin Login

- Username: admin
- Password: admin123

## Files

- students.csv
- rooms.csv
- admins.csv

CSV is the single source of truth. Legacy .txt files were removed.

## Data Formats

- Student: `ID,Name,Email,Phone,Course,Year,PreferredRoommate,AllocatedRoom,IsAllocated,Preferences(; separated)`
- Room: `Number,Type,Capacity,Occupancy,Rent,Floor,Block,IsAvailable,Occupants(; separated)`
- Admin: `ID,Username,Password,Name,Email`

Notes:
- Student ID must be exactly 13 digits (e.g., 2024010015138) and unique.
- Room numbers use a numeric series (e.g., 4001, 4002, ...).
- `IsAllocated`/`IsAvailable`: use 1 for true, 0 for false.

## Main Features

- Student CRUD, search by ID
- Room CRUD, availability tracking
- Manual and auto allocation
- Reports: allocation, occupancy, student distribution

## Shortcuts (Admin Panel)

- Add/Remove/Update/Display/Search students
- Add/Remove/Update/Display/Search rooms
- Allocate/Deallocate/Auto-allocate rooms
- Change password, add admins

## Implementation Notes

- OOP: classes `Student`, `Room`, `Admin`, `HostelManager`
- CSV I/O: robust parsing with quoting; stream operators `<<` and `>>`
- Integrity: 13-digit ID validation, uniqueness check, dedupe on load

## Troubleshooting

- If data doesn’t load, check CSV formatting and line endings
- To reset data, clear CSVs (keep headers/format) or replace with samples

## License

Educational/demo use.