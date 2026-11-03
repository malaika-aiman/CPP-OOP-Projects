#include <iostream>
#include <string>
#include <cstddef>

using namespace std;

// Node Class
class Node {
private:
    int data;
    Node* next;

public:
    Node() {
        data = 0;
        next = NULL;
    }

    Node(int val) {
        data = val;
        next = NULL;
    }

    // Setter functions
    void setData(int val) {
        data = val;
    }

    void setNext(Node* ptr) {
        next = ptr;
    }

    // Getter functions
    int getData() const {
        return data;
    }

    Node* getNext() const {
        return next;
    }
};

// NumberStack Class using Singly Linked List
class NumberStack {
private:
    Node* top;

public:
    // Constructor
    NumberStack() {
        top = NULL;
    }

    // Destructor to release dynamic memory
    ~NumberStack() {
        while (top != NULL) {
            Node* temp = top;
            top = top->getNext();
            delete temp;
        }
    }

    // Push operation
    void push(int val) {
        Node* newNode = new Node(val);
        newNode->setNext(top);
        top = newNode;
    }

    // Pop operation
    int pop() {
        if (top == NULL) {
            cout << "Stack Underflow! No elements to pop."
                 << endl;
            return -1;
        }

        Node* temp = top;
        int poppedValue = temp->getData();

        top = top->getNext();

        delete temp;

        return poppedValue;
    }

    // Check whether stack is empty
    bool isEmpty() const {
        return top == NULL;
    }

    // Check whether a number is prime
    bool isPrime(int n) const {
        if (n <= 1) {
            return false;
        }

        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                return false;
            }
        }

        return true;
    }

    // Count total prime numbers
    int countPrimes() const {
        int count = 0;
        Node* current = top;

        while (current != NULL) {
            if (isPrime(current->getData())) {
                count++;
            }

            current = current->getNext();
        }

        return count;
    }

    // Calculate sum of prime numbers
    int sumPrimes() const {
        int sum = 0;
        Node* current = top;

        while (current != NULL) {
            if (isPrime(current->getData())) {
                sum += current->getData();
            }

            current = current->getNext();
        }

        return sum;
    }

    // Convert decimal number into binary using stack
    void toBinary(int decimalNum) const {
        if (decimalNum == 0) {
            cout << "0";
            return;
        }

        NumberStack binaryStack;
        int tempNum = decimalNum;

        // Push remainders into another stack
        while (tempNum > 0) {
            int remainder = tempNum % 2;

            binaryStack.push(remainder);

            tempNum = tempNum / 2;
        }

        // Pop remainders to display binary number
        while (!binaryStack.isEmpty()) {
            cout << binaryStack.pop();
        }
    }
};

// Main Function
int main() {

    // Step 1: Display Student VU ID
    string studentID = "BC250443032";

    cout << "==========================================="
         << endl;

    cout << "  Prime Number & Binary Conversion System"
         << endl;

    cout << "  Student VU ID: " << studentID << endl;

    cout << "==========================================="
         << endl << endl;

    // Extract last digit of VU ID
    char lastDigitChar =
        studentID[studentID.length() - 1];

    int lastDigit = lastDigitChar - '0';

    // Create NumberStack object
    NumberStack myStack;

    // Step 2: Push six numbers into the stack
    myStack.push(lastDigit);
    myStack.push(18);
    myStack.push(23);
    myStack.push(29);
    myStack.push(41);
    myStack.push(45);

    cout << "Six numbers inserted into the stack successfully."
         << endl;

    cout << "Last digit extracted from VU ID: "
         << lastDigit << endl << endl;

    // Step 3: Count prime numbers
    int primeCount = myStack.countPrimes();

    cout << "Total Prime Numbers in the stack: "
         << primeCount << endl;

    // Step 4: Calculate sum of prime numbers
    int primeSum = myStack.sumPrimes();

    cout << "Sum of all Prime Numbers in the stack: "
         << primeSum << endl << endl;

    // Step 5: Pop one number from the stack
    cout << "--- Pop & Binary Conversion ---"
         << endl;

    int poppedNumber = myStack.pop();

    cout << "Popped Number from Stack: "
         << poppedNumber << endl;

    // Step 6: Convert popped number into binary
    cout << "Binary equivalent of "
         << poppedNumber << " is: ";

    myStack.toBinary(poppedNumber);

    cout << endl;

    return 0;
}