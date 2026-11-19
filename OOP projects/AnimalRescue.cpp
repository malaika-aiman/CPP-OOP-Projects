#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

class Animal {
protected:
    int id;
    string name;
    int age;
    bool adopted;

public:
    Animal(int animalId, string animalName, int animalAge)
        : id(animalId), name(animalName), age(animalAge), adopted(false) {}

    virtual string type() const = 0;

    virtual void showDetails() const {
        cout << "ID: " << id << " | Name: " << name
             << " | Type: " << type() << " | Age: " << age
             << " | Status: " << (adopted ? "Adopted" : "Available") << '\n';
    }

    int getId() const {
        return id;
    }

    bool isAvailable() const {
        return !adopted;
    }

    void adopt() {
        adopted = true;
    }

    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    Dog(int id, string name, int age) : Animal(id, name, age) {}
    string type() const override {
        return "Dog";
    }
};

class Cat : public Animal {
public:
    Cat(int id, string name, int age) : Animal(id, name, age) {}
    string type() const override {
        return "Cat";
    }
};

class Bird : public Animal {
public:
    Bird(int id, string name, int age) : Animal(id, name, age) {}
    string type() const override {
        return "Bird";
    }
};

class RescueCenter {
    vector<unique_ptr<Animal>> animals;
    int nextId = 1;

public:
    void addAnimal() {
        int type, age;
        string name;

        cout << "Animal name: ";
        getline(cin >> ws, name);
        cout << "Age: ";
        cin >> age;
        cout << "Type (1. Dog, 2. Cat, 3. Bird): ";
        cin >> type;

        if (age < 0 || (type < 1 || type > 3)) {
            cout << "Please enter valid details.\n";
            return;
        }

        if (type == 1) animals.push_back(make_unique<Dog>(nextId, name, age));
        else if (type == 2) animals.push_back(make_unique<Cat>(nextId, name, age));
        else animals.push_back(make_unique<Bird>(nextId, name, age));

        cout << "Animal registered with ID " << nextId << ".\n";
        nextId++;
    }

    void showAnimals() const {
        if (animals.empty()) {
            cout << "No animal records found.\n";
            return;
        }
        for (const auto& animal : animals)
            animal->showDetails();
    }

    void adoptAnimal() {
        int id;
        cout << "Enter animal ID: ";
        cin >> id;

        for (auto& animal : animals) {
            if (animal->getId() == id) {
                if (!animal->isAvailable()) {
                    cout << "This animal has already been adopted.\n";
                    return;
                }
                animal->adopt();
                cout << "Adoption completed. Thank you for helping an animal.\n";
                return;
            }
        }
        cout << "Animal ID not found.\n";
    }
};

int main() {
    RescueCenter center;
    int choice;

    do {
        cout << "\nAnimal Rescue and Adoption Center\n";
        cout << "1. Register rescued animal\n2. View animal records\n";
        cout << "3. Adopt an animal\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) center.addAnimal();
        else if (choice == 2) center.showAnimals();
        else if (choice == 3) center.adoptAnimal();
        else if (choice == 0) cout << "Goodbye.\n";
        else cout << "Invalid option.\n";
    } while (choice != 0);

    return 0;
}
