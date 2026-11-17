#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Supply {
    string name;
    int quantity;
};

struct Request {
    string area;
    string item;
    int quantity;
    int priority;
};

void showSupplies(const vector<Supply>& supplies) {
    cout << "\nAvailable supplies\n";
    if (supplies.empty()) {
        cout << "No supplies added yet.\n";
        return;
    }
    for (int i = 0; i < (int)supplies.size(); i++) {
        cout << i + 1 << ". " << supplies[i].name
             << " - " << supplies[i].quantity << '\n';
    }
}

void addSupply(vector<Supply>& supplies) {
    string name;
    int quantity;
    cout << "Enter supply name: ";
    getline(cin >> ws, name);
    cout << "Enter quantity: ";
    cin >> quantity;
    if (quantity <= 0) {
        cout << "Quantity must be greater than zero.\n";
        return;
    }

    for (auto& supply : supplies) {
        if (supply.name == name) {
            supply.quantity += quantity;
            cout << "Stock updated.\n";
            return;
        }
    }

    supplies.push_back({name, quantity});
    cout << "Supply added.\n";
}

void addRequest(vector<Request>& requests) {
    Request request;
    cout << "Enter affected area: ";
    getline(cin >> ws, request.area);
    cout << "Enter required item: ";
    getline(cin, request.item);
    cout << "Enter required quantity: ";
    cin >> request.quantity;
    cout << "Priority (1 = normal, 2 = urgent, 3 = critical): ";
    cin >> request.priority;

    if (request.quantity <= 0 || request.priority < 1 || request.priority > 3) {
        cout << "Please enter valid quantity and priority.\n";
        return;
    }

    requests.push_back(request);
    cout << "Request registered.\n";
}

void processRequests(vector<Supply>& supplies, vector<Request>& requests) {
    if (requests.empty()) {
        cout << "There are no pending requests.\n";
        return;
    }

    stable_sort(requests.begin(), requests.end(), [](const Request& a, const Request& b) {
        return a.priority > b.priority;
    });

    for (int i = 0; i < (int)requests.size();) {
        Request request = requests[i];
        auto it = find_if(supplies.begin(), supplies.end(), [&](const Supply& supply) {
            return supply.name == request.item;
        });

        cout << "\nArea: " << request.area << "\nItem: " << request.item
             << "\nQuantity: " << request.quantity
             << "\nPriority: " << request.priority << '\n';

        if (it == supplies.end()) {
            cout << "This item is not in stock. Request kept pending.\n";
            i++;
        } else if (it->quantity < request.quantity) {
            cout << "Not enough stock. Request kept pending.\n";
            i++;
        } else {
            it->quantity -= request.quantity;
            cout << "Supplies allocated successfully.\n";
            requests.erase(requests.begin() + i);
        }
    }
}

void showRequests(const vector<Request>& requests) {
    cout << "\nPending requests\n";
    if (requests.empty()) {
        cout << "No pending requests.\n";
        return;
    }
    for (const auto& request : requests) {
        cout << request.area << " needs " << request.quantity << " "
             << request.item << " | Priority: " << request.priority << '\n';
    }
}

int main() {
    vector<Supply> supplies;
    vector<Request> requests;
    int choice;

    do {
        cout << "\nDisaster Relief Resource Allocation\n";
        cout << "1. Add or update supply\n2. View supplies\n3. Register area request\n";
        cout << "4. Process requests\n5. View pending requests\n0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: addSupply(supplies); break;
            case 2: showSupplies(supplies); break;
            case 3: addRequest(requests); break;
            case 4: processRequests(supplies, requests); break;
            case 5: showRequests(requests); break;
            case 0: cout << "Goodbye.\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 0);

    return 0;
}
