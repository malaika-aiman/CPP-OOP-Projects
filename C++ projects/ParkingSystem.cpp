#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Vehicle {
private:
    string registrationNumber;
    string ownerName;
    string vehicleType;

public:
    Vehicle(const string& registration, const string& owner, const string& type)
        : registrationNumber(registration), ownerName(owner), vehicleType(type) {}

    string getRegistrationNumber() const { return registrationNumber; }
    string getOwnerName() const { return ownerName; }
    string getVehicleType() const { return vehicleType; }
};

class ParkingSlot {
private:
    int slotNumber;
    bool occupied;
    Vehicle* vehicle;
    int entryHour;

public:
    explicit ParkingSlot(int number)
        : slotNumber(number), occupied(false), vehicle(nullptr), entryHour(0) {}

    ~ParkingSlot() {
        delete vehicle;
    }

    ParkingSlot(const ParkingSlot&) = delete;
    ParkingSlot& operator=(const ParkingSlot&) = delete;

    ParkingSlot(ParkingSlot&& other) noexcept
        : slotNumber(other.slotNumber),
          occupied(other.occupied),
          vehicle(other.vehicle),
          entryHour(other.entryHour) {
        other.vehicle = nullptr;
        other.occupied = false;
    }

    ParkingSlot& operator=(ParkingSlot&& other) noexcept {
        if (this != &other) {
            delete vehicle;
            slotNumber = other.slotNumber;
            occupied = other.occupied;
            vehicle = other.vehicle;
            entryHour = other.entryHour;
            other.vehicle = nullptr;
            other.occupied = false;
        }
        return *this;
    }

    int getSlotNumber() const { return slotNumber; }
    bool isOccupied() const { return occupied; }

    string getRegistrationNumber() const {
        return vehicle ? vehicle->getRegistrationNumber() : "";
    }

    void parkVehicle(const Vehicle& newVehicle, int hour) {
        if (occupied) {
            return;
        }

        vehicle = new Vehicle(newVehicle);
        entryHour = hour;
        occupied = true;
    }

    double removeVehicle(int exitHour) {
        if (!occupied || vehicle == nullptr) {
            return -1.0;
        }

        int duration = exitHour - entryHour;
        if (duration <= 0) {
            duration += 24;
        }

        double fee = duration * 100.0;
        delete vehicle;
        vehicle = nullptr;
        occupied = false;
        entryHour = 0;
        return fee;
    }

    void display() const {
        cout << "Slot " << setw(2) << slotNumber << " : ";
        if (!occupied) {
            cout << "Available\n";
        } else {
            cout << "Occupied | " << vehicle->getRegistrationNumber()
                 << " | " << vehicle->getOwnerName()
                 << " | " << vehicle->getVehicleType() << '\n';
        }
    }
};

class ParkingLot {
private:
    vector<ParkingSlot> slots;
    const string recordFile = "parking_records.txt";

    bool isValidHour(int hour) const {
        return hour >= 0 && hour <= 23;
    }

    bool registrationExists(const string& registration) const {
        for (const ParkingSlot& slot : slots) {
            if (slot.isOccupied() && slot.getRegistrationNumber() == registration) {
                return true;
            }
        }
        return false;
    }

    ParkingSlot* findAvailableSlot() {
        for (ParkingSlot& slot : slots) {
            if (!slot.isOccupied()) {
                return &slot;
            }
        }
        return nullptr;
    }

    ParkingSlot* findVehicle(const string& registration) {
        for (ParkingSlot& slot : slots) {
            if (slot.isOccupied() && slot.getRegistrationNumber() == registration) {
                return &slot;
            }
        }
        return nullptr;
    }

    void saveRecord(const string& registration, int slotNumber,
                    int duration, double fee) const {
        ofstream file(recordFile, ios::app);
        if (file) {
            file << registration << "|" << slotNumber << "|"
                 << duration << "|" << fixed << setprecision(2) << fee << '\n';
        }
    }

public:
    explicit ParkingLot(int capacity) {
        for (int i = 1; i <= capacity; ++i) {
            slots.emplace_back(i);
        }
    }

    void displaySlots() const {
        cout << "\nPARKING SLOT STATUS\n";
        cout << "-------------------\n";
        for (const ParkingSlot& slot : slots) {
            slot.display();
        }
    }

    void park() {
        string registration, owner, type, hourText;
        cout << "Vehicle registration: ";
        getline(cin, registration);
        cout << "Owner name: ";
        getline(cin, owner);
        cout << "Vehicle type: ";
        getline(cin, type);
        cout << "Entry hour (0-23): ";
        getline(cin, hourText);

        int hour;
        try {
            hour = stoi(hourText);
        } catch (...) {
            cout << "Invalid hour.\n";
            return;
        }

        if (registration.empty() || owner.empty() || type.empty() ||
            !isValidHour(hour)) {
            cout << "Please enter valid vehicle details and time.\n";
            return;
        }

        if (registrationExists(registration)) {
            cout << "This vehicle is already parked.\n";
            return;
        }

        ParkingSlot* slot = findAvailableSlot();
        if (slot == nullptr) {
            cout << "The parking lot is full.\n";
            return;
        }

        Vehicle vehicle(registration, owner, type);
        slot->parkVehicle(vehicle, hour);
        cout << "Vehicle parked in slot " << slot->getSlotNumber() << ".\n";
    }

    void release() {
        string registration, hourText;
        cout << "Vehicle registration: ";
        getline(cin, registration);
        cout << "Exit hour (0-23): ";
        getline(cin, hourText);

        int exitHour;
        try {
            exitHour = stoi(hourText);
        } catch (...) {
            cout << "Invalid hour.\n";
            return;
        }

        if (!isValidHour(exitHour)) {
            cout << "Hour must be between 0 and 23.\n";
            return;
        }

        ParkingSlot* slot = findVehicle(registration);
        if (slot == nullptr) {
            cout << "Vehicle not found.\n";
            return;
        }

        int slotNumber = slot->getSlotNumber();
        double fee = slot->removeVehicle(exitHour);
        cout << fixed << setprecision(2);
        cout << "Vehicle released from slot " << slotNumber << ".\n";
        cout << "Parking fee: Rs. " << fee << '\n';
        saveRecord(registration, slotNumber, 0, fee);
    }

    void showRecords() const {
        ifstream file(recordFile);
        string line;

        cout << "\nPARKING HISTORY\n";
        cout << "----------------\n";

        if (!file) {
            cout << "No completed parking records found.\n";
            return;
        }

        while (getline(file, line)) {
            stringstream stream(line);
            string registration, slot, duration, fee;

            if (getline(stream, registration, '|') &&
                getline(stream, slot, '|') &&
                getline(stream, duration, '|') &&
                getline(stream, fee)) {
                cout << "Vehicle: " << registration
                     << " | Slot: " << slot
                     << " | Fee: Rs. " << fee << '\n';
            }
        }
    }
};

int main() {
    ParkingLot parkingLot(8);

    while (true) {
        cout << "\nSMART PARKING MANAGEMENT SYSTEM\n";
        cout << "1. View parking slots\n2. Park a vehicle\n";
        cout << "3. Release a vehicle\n4. View parking history\n5. Exit\n";
        cout << "Select an option: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            parkingLot.displaySlots();
        } else if (choice == "2") {
            parkingLot.park();
        } else if (choice == "3") {
            parkingLot.release();
        } else if (choice == "4") {
            parkingLot.showRecords();
        } else if (choice == "5") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
