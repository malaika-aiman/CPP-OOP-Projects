#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

class Device {
protected:
    string name;
    bool isOn;

public:
    Device(string deviceName) : name(deviceName), isOn(false) {}

    virtual void turnOn() {
        isOn = true;
        cout << name << " is now ON.\n";
    }

    virtual void turnOff() {
        isOn = false;
        cout << name << " is now OFF.\n";
    }

    virtual void showStatus() const {
        cout << name << ": " << (isOn ? "ON" : "OFF") << '\n';
    }

    virtual double dailyUnits() const {
        return 0;
    }

    virtual ~Device() {}
};

class Light : public Device {
    int brightness;

public:
    Light() : Device("Light"), brightness(70) {}

    void setBrightness(int value) {
        if (value < 0 || value > 100) {
            cout << "Brightness must be from 0 to 100.\n";
            return;
        }
        brightness = value;
        cout << "Brightness set to " << brightness << "%.\n";
    }

    double dailyUnits() const override {
        return isOn ? 0.8 : 0;
    }

    void showStatus() const override {
        Device::showStatus();
        cout << "Brightness: " << brightness << "%\n";
    }
};

class AirConditioner : public Device {
    int temperature;

public:
    AirConditioner() : Device("Air Conditioner"), temperature(24) {}

    void setTemperature(int value) {
        if (value < 16 || value > 30) {
            cout << "Temperature must be between 16 and 30.\n";
            return;
        }
        temperature = value;
        cout << "Temperature set to " << temperature << " C.\n";
    }

    double dailyUnits() const override {
        return isOn ? 8.0 : 0;
    }

    void showStatus() const override {
        Device::showStatus();
        cout << "Temperature: " << temperature << " C\n";
    }
};

class Door : public Device {
public:
    Door() : Device("Smart Door") {}

    void turnOn() override {
        isOn = true;
        cout << "Door is locked.\n";
    }

    void turnOff() override {
        isOn = false;
        cout << "Door is unlocked.\n";
    }

    double dailyUnits() const override {
        return 0.1;
    }
};

class SmartHome {
    Light light;
    AirConditioner ac;
    Door door;

public:
    void controlDevice() {
        int choice, action;
        cout << "1. Light\n2. Air conditioner\n3. Smart door\nChoose device: ";
        cin >> choice;

        cout << "1. Turn on / lock\n2. Turn off / unlock\nChoose action: ";
        cin >> action;

        Device* selected = nullptr;
        if (choice == 1) selected = &light;
        else if (choice == 2) selected = &ac;
        else if (choice == 3) selected = &door;

        if (selected == nullptr || (action != 1 && action != 2)) {
            cout << "Invalid selection.\n";
            return;
        }

        if (action == 1) selected->turnOn();
        else selected->turnOff();

        if (choice == 1) {
            int value;
            cout << "Enter brightness (0-100): ";
            cin >> value;
            light.setBrightness(value);
        } else if (choice == 2) {
            int value;
            cout << "Enter temperature (16-30): ";
            cin >> value;
            ac.setTemperature(value);
        }
    }

    void showAllStatus() const {
        cout << '\n';
        light.showStatus();
        ac.showStatus();
        door.showStatus();
        cout << "Estimated daily electricity use: "
             << light.dailyUnits() + ac.dailyUnits() + door.dailyUnits()
             << " units\n";
    }
};

int main() {
    SmartHome home;
    int choice;

    do {
        cout << "\nSmart Home Automation\n";
        cout << "1. Control a device\n2. View home status\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) home.controlDevice();
        else if (choice == 2) home.showAllStatus();
        else if (choice == 0) cout << "Goodbye.\n";
        else cout << "Invalid option.\n";
    } while (choice != 0);

    return 0;
}
