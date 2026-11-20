#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Customer {
    string name;
    string phone;

public:
    Customer(string customerName, string customerPhone)
        : name(customerName), phone(customerPhone) {}

    void show() const {
        cout << name << " (" << phone << ")";
    }
};

class Employee {
protected:
    string name;
    int employeeId;

public:
    Employee(string employeeName, int id) : name(employeeName), employeeId(id) {}

    virtual string role() const = 0;

    void show() const {
        cout << name << " - " << role() << " (ID " << employeeId << ")";
    }

    virtual ~Employee() {}
};

class Rider : public Employee {
public:
    Rider(string name, int id) : Employee(name, id) {}

    string role() const override {
        return "Delivery Rider";
    }
};

class Parcel {
    int trackingId;
    Customer sender;
    Customer receiver;
    double weight;
    string destination;
    string status;

public:
    Parcel(int id, Customer from, Customer to, double parcelWeight, string place)
        : trackingId(id), sender(from), receiver(to), weight(parcelWeight),
          destination(place), status("Booked") {}

    int getTrackingId() const {
        return trackingId;
    }

    double calculateCost() const {
        return 150 + weight * 80;
    }

    void updateStatus(string newStatus) {
        status = newStatus;
    }

    void showDetails() const {
        cout << "\nTracking ID: " << trackingId << '\n';
        cout << "Sender: ";
        sender.show();
        cout << "\nReceiver: ";
        receiver.show();
        cout << "\nDestination: " << destination;
        cout << "\nWeight: " << weight << " kg";
        cout << "\nDelivery cost: Rs. " << fixed << setprecision(2) << calculateCost();
        cout << "\nStatus: " << status << "\n";
    }
};

class CourierService {
    vector<Parcel> parcels;
    int nextTrackingId = 1001;

public:
    void bookParcel() {
        string senderName, senderPhone, receiverName, receiverPhone, destination;
        double weight;

        cout << "Sender name: ";
        getline(cin >> ws, senderName);
        cout << "Sender phone: ";
        cin >> senderPhone;
        cout << "Receiver name: ";
        getline(cin >> ws, receiverName);
        cout << "Receiver phone: ";
        cin >> receiverPhone;
        cout << "Destination city: ";
        getline(cin >> ws, destination);
        cout << "Parcel weight in kg: ";
        cin >> weight;

        if (weight <= 0) {
            cout << "Weight must be greater than zero.\n";
            return;
        }

        Customer sender(senderName, senderPhone);
        Customer receiver(receiverName, receiverPhone);
        parcels.emplace_back(nextTrackingId, sender, receiver, weight, destination);

        cout << "Parcel booked. Tracking ID: " << nextTrackingId << '\n';
        cout << "Delivery cost: Rs. " << fixed << setprecision(2)
             << parcels.back().calculateCost() << '\n';
        nextTrackingId++;
    }

    void updateParcel() {
        int id, choice;
        cout << "Enter tracking ID: ";
        cin >> id;

        for (auto& parcel : parcels) {
            if (parcel.getTrackingId() == id) {
                cout << "1. In transit\n2. Out for delivery\n3. Delivered\nChoose status: ";
                cin >> choice;

                if (choice == 1) parcel.updateStatus("In Transit");
                else if (choice == 2) parcel.updateStatus("Out for Delivery");
                else if (choice == 3) parcel.updateStatus("Delivered");
                else {
                    cout << "Invalid status.\n";
                    return;
                }

                cout << "Parcel status updated.\n";
                return;
            }
        }
        cout << "Tracking ID not found.\n";
    }

    void trackParcel() const {
        int id;
        cout << "Enter tracking ID: ";
        cin >> id;

        for (const auto& parcel : parcels) {
            if (parcel.getTrackingId() == id) {
                parcel.showDetails();
                return;
            }
        }
        cout << "No parcel found with this tracking ID.\n";
    }

    void showAllParcels() const {
        if (parcels.empty()) {
            cout << "No parcels have been booked.\n";
            return;
        }
        for (const auto& parcel : parcels)
            parcel.showDetails();
    }
};

int main() {
    CourierService service;
    int choice;

    do {
        cout << "\nCourier and Parcel Tracking System\n";
        cout << "1. Book parcel\n2. Update delivery status\n";
        cout << "3. Track parcel\n4. View all parcels\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) service.bookParcel();
        else if (choice == 2) service.updateParcel();
        else if (choice == 3) service.trackParcel();
        else if (choice == 4) service.showAllParcels();
        else if (choice == 0) cout << "Goodbye.\n";
        else cout << "Invalid option.\n";
    } while (choice != 0);

    return 0;
}
