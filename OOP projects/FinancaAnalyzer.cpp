#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

struct Transaction {
    string type;
    string category;
    string description;
    double amount;
};

class FinanceAnalyzer {
private:
    vector<Transaction> transactions;
    const string dataFile = "transactions.txt";

    bool isValidAmount(double amount) const {
        return amount > 0.0;
    }

    void loadTransactions() {
        ifstream file(dataFile);
        string line;

        while (getline(file, line)) {
            stringstream stream(line);
            string type, category, description, amountText;

            if (getline(stream, type, '|') &&
                getline(stream, category, '|') &&
                getline(stream, description, '|') &&
                getline(stream, amountText)) {
                try {
                    transactions.push_back(
                        {type, category, description, stod(amountText)}
                    );
                } catch (const invalid_argument&) {
                } catch (const out_of_range&) {
                }
            }
        }
    }

    void saveTransactions() const {
        ofstream file(dataFile);
        for (const Transaction& transaction : transactions) {
            file << transaction.type << '|'
                 << transaction.category << '|'
                 << transaction.description << '|'
                 << fixed << setprecision(2) << transaction.amount << '\n';
        }
    }

    double totalFor(const string& type) const {
        double total = 0.0;
        for (const Transaction& transaction : transactions) {
            if (transaction.type == type) {
                total += transaction.amount;
            }
        }
        return total;
    }

public:
    FinanceAnalyzer() {
        loadTransactions();
    }

    void addTransaction() {
        string typeChoice, category, description, amountText;

        cout << "1. Income\n2. Expense\nSelect type: ";
        getline(cin, typeChoice);

        string type;
        if (typeChoice == "1") {
            type = "Income";
        } else if (typeChoice == "2") {
            type = "Expense";
        } else {
            cout << "Invalid transaction type.\n";
            return;
        }

        cout << "Category: ";
        getline(cin, category);
        cout << "Description: ";
        getline(cin, description);
        cout << "Amount: Rs. ";
        getline(cin, amountText);

        double amount;
        try {
            amount = stod(amountText);
        } catch (...) {
            cout << "Please enter a valid amount.\n";
            return;
        }

        if (category.empty() || description.empty() || !isValidAmount(amount) ||
            category.find('|') != string::npos ||
            description.find('|') != string::npos) {
            cout << "Transaction details are invalid.\n";
            return;
        }

        transactions.push_back({type, category, description, amount});
        saveTransactions();
        cout << "Transaction saved.\n";
    }

    void displayTransactions() const {
        if (transactions.empty()) {
            cout << "No transactions recorded yet.\n";
            return;
        }

        cout << "\nTRANSACTION HISTORY\n";
        cout << "-------------------------------------------------------------\n";
        cout << left << setw(10) << "Type"
             << setw(16) << "Category"
             << setw(24) << "Description"
             << right << setw(12) << "Amount\n";

        cout << "-------------------------------------------------------------\n";
        for (const Transaction& transaction : transactions) {
            cout << left << setw(10) << transaction.type
                 << setw(16) << transaction.category.substr(0, 15)
                 << setw(24) << transaction.description.substr(0, 23)
                 << right << setw(12) << fixed << setprecision(2)
                 << transaction.amount << '\n';
        }
    }

    void displaySummary() const {
        double income = totalFor("Income");
        double expenses = totalFor("Expense");
        double balance = income - expenses;

        cout << fixed << setprecision(2);
        cout << "\nFINANCIAL SUMMARY\n";
        cout << "-----------------\n";
        cout << "Total income:   Rs. " << income << '\n';
        cout << "Total expenses: Rs. " << expenses << '\n';
        cout << "Net balance:    Rs. " << balance << '\n';

        if (balance < 0) {
            cout << "Status: Expenses are higher than income.\n";
        } else {
            cout << "Status: Your balance is non-negative.\n";
        }
    }

    void categoryReport() const {
        string category;
        cout << "Enter category: ";
        getline(cin, category);

        double total = 0.0;
        bool found = false;

        for (const Transaction& transaction : transactions) {
            if (transaction.category == category) {
                total += transaction.amount;
                found = true;
            }
        }

        if (!found) {
            cout << "No transactions found for this category.\n";
        } else {
            cout << fixed << setprecision(2);
            cout << "Total for " << category << ": Rs. " << total << '\n';
        }
    }
};

int main() {
    FinanceAnalyzer analyzer;

    while (true) {
        cout << "\nPERSONAL FINANCE ANALYZER\n";
        cout << "1. Add transaction\n2. View transactions\n";
        cout << "3. Financial summary\n4. Category report\n5. Exit\n";
        cout << "Select an option: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            analyzer.addTransaction();
        } else if (choice == "2") {
            analyzer.displayTransactions();
        } else if (choice == "3") {
            analyzer.displaySummary();
        } else if (choice == "4") {
            analyzer.categoryReport();
        } else if (choice == "5") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
