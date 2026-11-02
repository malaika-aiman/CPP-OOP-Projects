#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

class User {
private:
    int id;
    string username;
    string password;
    string role;

public:
    User(int userId, const string& userName, const string& userPassword,
         const string& userRole)
        : id(userId), username(userName), password(userPassword), role(userRole) {}

    int getId() const { return id; }
    string getUsername() const { return username; }
    string getRole() const { return role; }

    bool matchesCredentials(const string& userName, const string& userPassword) const {
        return username == userName && password == userPassword;
    }

    void display() const {
        cout << "ID: " << id << " | Username: " << username
             << " | Role: " << role << '\n';
    }

    string toRecord() const {
        return to_string(id) + "|" + username + "|" + password + "|" + role;
    }
};

class AccessManager {
private:
    vector<User> users;
    int nextId = 1;
    const string dataFile = "users.txt";
    const string logFile = "access_log.txt";

    void writeLog(const string& message) const {
        ofstream file(logFile, ios::app);
        if (file) {
            file << message << '\n';
        }
    }

    void saveUsers() const {
        ofstream file(dataFile);
        for (const User& user : users) {
            file << user.toRecord() << '\n';
        }
    }

    void loadUsers() {
        ifstream file(dataFile);
        string line;

        while (getline(file, line)) {
            stringstream stream(line);
            string idText, username, password, role;

            if (getline(stream, idText, '|') &&
                getline(stream, username, '|') &&
                getline(stream, password, '|') &&
                getline(stream, role)) {
                try {
                    int id = stoi(idText);
                    users.emplace_back(id, username, password, role);
                    if (id >= nextId) {
                        nextId = id + 1;
                    }
                } catch (const invalid_argument&) {
                } catch (const out_of_range&) {
                }
            }
        }
    }

    bool usernameExists(const string& username) const {
        for (const User& user : users) {
            if (user.getUsername() == username) {
                return true;
            }
        }
        return false;
    }

    bool hasPermission(const string& role, const string& action) const {
        if (role == "Admin") {
            return true;
        }

        if (role == "Editor") {
            return action == "view" || action == "edit";
        }

        return action == "view";
    }

public:
    AccessManager() {
        loadUsers();

        if (users.empty()) {
            users.emplace_back(nextId++, "admin", "admin123", "Admin");
            saveUsers();
        }
    }

    void registerUser() {
        string username, password, role;

        cout << "Choose a username: ";
        getline(cin, username);

        if (username.empty() || username.find('|') != string::npos ||
            usernameExists(username)) {
            cout << "Username is empty, invalid, or already in use.\n";
            return;
        }

        cout << "Choose a password: ";
        getline(cin, password);

        if (password.size() < 4 || password.find('|') != string::npos) {
            cout << "Password must contain at least 4 characters.\n";
            return;
        }

        cout << "Select role (1. Viewer  2. Editor): ";
        getline(cin, role);

        if (role == "1") {
            role = "Viewer";
        } else if (role == "2") {
            role = "Editor";
        } else {
            cout << "Invalid role.\n";
            return;
        }

        users.emplace_back(nextId++, username, password, role);
        saveUsers();
        cout << "User account created.\n";
    }

    void login() {
        string username, password;

        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        for (const User& user : users) {
            if (user.matchesCredentials(username, password)) {
                cout << "\nWelcome, " << user.getUsername()
                     << " (" << user.getRole() << ").\n";
                writeLog("Successful login: " + username);
                userMenu(user);
                return;
            }
        }

        cout << "Login failed. Check your credentials.\n";
        writeLog("Failed login attempt: " + username);
    }

    void userMenu(const User& user) {
        while (true) {
            cout << "\n1. View dashboard\n2. Edit profile area\n3. View access log\n4. Logout\n";
            cout << "Select an option: ";

            string choice;
            getline(cin, choice);

            if (choice == "1") {
                if (hasPermission(user.getRole(), "view")) {
                    cout << "Dashboard access granted.\n";
                } else {
                    cout << "Access denied.\n";
                }
            } else if (choice == "2") {
                if (hasPermission(user.getRole(), "edit")) {
                    cout << "Edit access granted.\n";
                } else {
                    cout << "Access denied. Your role is read-only.\n";
                }
            } else if (choice == "3") {
                if (user.getRole() != "Viewer") {
                    showAccessLog();
                } else {
                    cout << "Access denied.\n";
                }
            } else if (choice == "4") {
                writeLog("Logout: " + user.getUsername());
                cout << "Logged out.\n";
                return;
            } else {
                cout << "Invalid option.\n";
            }
        }
    }

    void showAccessLog() const {
        ifstream file(logFile);
        string line;

        cout << "\nAccess Log\n";
        cout << "------------------------------\n";

        if (!file) {
            cout << "No log entries found.\n";
            return;
        }

        while (getline(file, line)) {
            cout << line << '\n';
        }
    }
};

int main() {
    AccessManager manager;

    while (true) {
        cout << "\nDIGITAL IDENTITY & ACCESS MANAGER\n";
        cout << "1. Login\n2. Register user\n3. Exit\n";
        cout << "Select an option: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            manager.login();
        } else if (choice == "2") {
            manager.registerUser();
        } else if (choice == "3") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
