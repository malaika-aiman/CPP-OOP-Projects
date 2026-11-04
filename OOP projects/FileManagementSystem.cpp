#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct FileEntry {
    int id;
    string name;
    string category;
    long long sizeKB;
};

class FileCatalog {
private:
    vector<FileEntry> files;
    const string dataFile = "file_catalog.txt";
    int nextId = 1;

    void loadFiles() {
        ifstream file(dataFile);
        string line;

        while (getline(file, line)) {
            stringstream stream(line);
            string idText, name, category, sizeText;

            if (getline(stream, idText, '|') &&
                getline(stream, name, '|') &&
                getline(stream, category, '|') &&
                getline(stream, sizeText)) {
                try {
                    int id = stoi(idText);
                    long long size = stoll(sizeText);
                    files.push_back({id, name, category, size});
                    if (id >= nextId) {
                        nextId = id + 1;
                    }
                } catch (const invalid_argument&) {
                } catch (const out_of_range&) {
                }
            }
        }
    }

    void saveFiles() const {
        ofstream file(dataFile);
        for (const FileEntry& entry : files) {
            file << entry.id << '|' << entry.name << '|'
                 << entry.category << '|' << entry.sizeKB << '\n';
        }
    }

    bool hasName(const string& name) const {
        for (const FileEntry& entry : files) {
            if (entry.name == name) {
                return true;
            }
        }
        return false;
    }

public:
    FileCatalog() {
        loadFiles();
    }

    void addFile() {
        string name, category, sizeText;

        cout << "File name: ";
        getline(cin, name);
        cout << "Category: ";
        getline(cin, category);
        cout << "Size (KB): ";
        getline(cin, sizeText);

        long long size;
        try {
            size = stoll(sizeText);
        } catch (...) {
            cout << "Invalid file size.\n";
            return;
        }

        if (name.empty() || category.empty() || size <= 0 ||
            name.find('|') != string::npos ||
            category.find('|') != string::npos) {
            cout << "Please enter valid file details.\n";
            return;
        }

        if (hasName(name)) {
            cout << "A file with this name already exists in the catalog.\n";
            return;
        }

        files.push_back({nextId++, name, category, size});
        saveFiles();
        cout << "File added to catalog.\n";
    }

    void displayFiles() const {
        if (files.empty()) {
            cout << "The catalog is empty.\n";
            return;
        }

        cout << "\nFILE CATALOG\n";
        cout << "--------------------------------------------------\n";
        cout << left << setw(6) << "ID"
             << setw(24) << "Name"
             << setw(16) << "Category"
             << right << setw(10) << "Size KB\n";
        cout << "--------------------------------------------------\n";

        for (const FileEntry& entry : files) {
            cout << left << setw(6) << entry.id
                 << setw(24) << entry.name.substr(0, 23)
                 << setw(16) << entry.category.substr(0, 15)
                 << right << setw(10) << entry.sizeKB << '\n';
        }
    }

    void searchFiles() const {
        string keyword;
        cout << "Enter a name or category keyword: ";
        getline(cin, keyword);

        transform(keyword.begin(), keyword.end(), keyword.begin(),
                  [](unsigned char character) {
                      return static_cast<char>(tolower(character));
                  });

        bool found = false;

        for (const FileEntry& entry : files) {
            string name = entry.name;
            string category = entry.category;

            transform(name.begin(), name.end(), name.begin(),
                      [](unsigned char character) {
                          return static_cast<char>(tolower(character));
                      });

            transform(category.begin(), category.end(), category.begin(),
                      [](unsigned char character) {
                          return static_cast<char>(tolower(character));
                      });

            if (name.find(keyword) != string::npos ||
                category.find(keyword) != string::npos) {
                cout << "ID: " << entry.id
                     << " | Name: " << entry.name
                     << " | Category: " << entry.category
                     << " | Size: " << entry.sizeKB << " KB\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No matching entries found.\n";
        }
    }

    void sortBySize() {
        sort(files.begin(), files.end(),
             [](const FileEntry& first, const FileEntry& second) {
                 return first.sizeKB < second.sizeKB;
             });

        saveFiles();
        cout << "Catalog sorted by file size.\n";
        displayFiles();
    }

    void removeEntry() {
        string idText;
        cout << "Enter file ID to remove from catalog: ";
        getline(cin, idText);

        int id;
        try {
            id = stoi(idText);
        } catch (...) {
            cout << "Invalid ID.\n";
            return;
        }

        auto position = remove_if(files.begin(), files.end(),
                                  [id](const FileEntry& entry) {
                                      return entry.id == id;
                                  });

        if (position == files.end()) {
            cout << "File ID not found.\n";
            return;
        }

        files.erase(position, files.end());
        saveFiles();
        cout << "Catalog entry removed. No computer files were deleted.\n";
    }
};

int main() {
    FileCatalog catalog;

    while (true) {
        cout << "\nSMART FILE CATALOG\n";
        cout << "1. Add file entry\n2. View catalog\n";
        cout << "3. Search catalog\n4. Sort by size\n";
        cout << "5. Remove catalog entry\n6. Exit\n";
        cout << "Select an option: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            catalog.addFile();
        } else if (choice == "2") {
            catalog.displayFiles();
        } else if (choice == "3") {
            catalog.searchFiles();
        } else if (choice == "4") {
            catalog.sortBySize();
        } else if (choice == "5") {
            catalog.removeEntry();
        } else if (choice == "6") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
