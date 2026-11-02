#include <iostream>
#include <string>
#include <cstddef>

using namespace std;

// Node Class
class Node {
private:
    string cloth;
    Node* next;

public:
    Node() : cloth(""), next(NULL) {}

    // Setter functions
    void setCloth(string c) {
        cloth = c;
    }

    void setNext(Node* ptr) {
        next = ptr;
    }

    // Getter functions
    string getCloth() const {
        return cloth;
    }

    Node* getNext() const {
        return next;
    }
};

// Stack Class using Linked List
class Stack {
private:
    Node* top;

public:
    // Constructor
    Stack() {
        top = NULL;
    }

    // TODO 1: Push a clothing item onto the stack
    void pushCloth(string c) {
        Node* newNode = new Node();

        newNode->setCloth(c);
        newNode->setNext(top);

        top = newNode;
    }

    // TODO 2: Pop a clothing item from the stack
    string popCloth() {
        if (top == NULL) {
            return "Stack Underflow";
        }

        Node* temp = top;
        string poppedValue = temp->getCloth();

        top = top->getNext();

        delete temp;

        return poppedValue;
    }

    // TODO 3: Display all clothing items
    void displayClothStack() const {
        Node* current = top;

        while (current != NULL) {
            cout << current->getCloth() << endl;
            current = current->getNext();
        }
    }

    // Destructor to release dynamic memory
    ~Stack() {
        while (top != NULL) {
            Node* temp = top;
            top = top->getNext();

            delete temp;
        }
    }
};

// Main Function
int main() {

    // TODO 4: Initialize Student VUID
    string studentID = "BC250443032";

    // Extract the last digit of VUID
    char lastDigitChar = studentID[studentID.length() - 1];
    int lastDigit = lastDigitChar - '0';

    Stack s;

    cout << "Clothes Pile Management System - ("
         << studentID << ")" << endl << endl;

    // Step 1: Push six initial clothing items
    cout << "Step 1: Six clothes added to the stack:" << endl;

    s.pushCloth("Jeans");
    s.pushCloth("Shirt");
    s.pushCloth("Trousers");
    s.pushCloth("Socks");
    s.pushCloth("Cap");
    s.pushCloth("Jacket");

    cout << "Stack (Top to Bottom):" << endl;
    s.displayClothStack();

    cout << endl;

    // Conditional addition based on VUID
    cout << "VUID last digit is " << lastDigit << "." << endl;

    if (lastDigit > 5) {

        s.pushCloth("Sweater");
        s.pushCloth("Pant");

        cout << "Last digit is greater than 5: "
             << "Sweater and Pant added." << endl << endl;

    }
    else {

        s.pushCloth("Sweater");

        cout << "Last digit is 5 or less: "
             << "Only Sweater added." << endl << endl;
    }

    // Step 2: Pop exactly three clothing items
    cout << "Step 2: Removing three clothes from the top:"
         << endl;

    cout << "Popped: " << s.popCloth() << endl;
    cout << "Popped: " << s.popCloth() << endl;
    cout << "Popped: " << s.popCloth() << endl;

    cout << endl;

    // Step 3: Display final stack
    cout << "Step 3: Final Stack (Top to Bottom):" << endl;

    s.displayClothStack();

    return 0;
}