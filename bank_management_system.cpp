#include <iostream>   // For input and output (cin, cout)
#include <fstream>    // For file handling (ifstream, ofstream, fstream)
#include <cstring>    // For string handling (like getline for char arrays)
using namespace std;

// ---------------- ACCOUNT CLASS ----------------
class Account {
private:
    int accountNumber;     // Unique account number for each user
    char name[50];         // Account holder's name (stored as char array)
    double balance;        // Balance in account

public:
    // Function to create a new account
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;     // Take account number from user
        cin.ignore();             // Ignore newline so getline works properly
        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);    // Read full name including spaces
        cout << "Enter Initial Balance: ";
        cin >> balance;           // Take starting balance
    }

    // Function to show account details
    void showAccount() const {
        cout << "\nAccount No.: " << accountNumber
             << "\nName: " << name
             << "\nBalance: " << balance << "\n";
    }

    // Deposit money into account
    void deposit(double amount) {
        balance += amount;  // Add amount to balance
    }

    // Withdraw money from account
    void withdraw(double amount) {
        if (amount <= balance)   // Only withdraw if enough balance
            balance -= amount;
        else
            cout << "Insufficient balance!\n";
    }

    // Return account number (getter function)
    int getAccountNumber() const {
        return accountNumber;
    }
};

// ---------------- FILE HANDLING FUNCTIONS ----------------

// Function to create a new account and save it in file
void writeAccount() {
    Account acc;                   // Create an Account object
    acc.createAccount();           // Fill account details using user input

    // Open file in binary append mode
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();

    cout << "Account created successfully!\n";
}

// Function to display all accounts from file
void displayAll() {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);

    if (!inFile) {  // If file doesn’t exist
        cout << "No accounts found.\n";
        return;
    }

    cout << "\n--- All Accounts ---\n";
    // Read each account object from file and display it
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
        cout << "--------------------------------\n";
    }
    inFile.close();
}

// Function to view details of a specific account
void viewAccount(int accNo) {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    bool found = false;

    // Search in file for the account
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            acc.showAccount();
            found = true;
        }
    }

    if (!found)
        cout << "Account not found.\n";

    inFile.close();
}

// Function to update account (deposit/withdraw)
void updateAccount(int accNo, bool isDeposit) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account acc;
    bool found = false;

    // Read each account until found
    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account)) && !found) {
        if (acc.getAccountNumber() == accNo) {
            acc.showAccount(); // Show current details
            double amount;

            // If user wants deposit
            if (isDeposit) {
                cout << "Enter amount to deposit: ";
                cin >> amount;
                acc.deposit(amount);
            } 
            // If user wants withdraw
            else {
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                acc.withdraw(amount);
            }

            // Move file pointer back to overwrite this account
            int pos = -1 * static_cast<int>(sizeof(Account));
            file.seekp(pos, ios::cur);

            // Write updated account back into file
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            found = true;

            cout << "Account updated.\n";
        }
    }
    if (!found)
        cout << "Account not found.\n";

    file.close();
}

// ---------------- MAIN FUNCTION ----------------
int main() {
    int choice, accNo;
    do {
        // Menu-driven program
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n";
        cout << "2. View All Accounts\n";
        cout << "3. View Specific Account\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:  // Create new account
                writeAccount();
                break;
            case 2:  // View all accounts
                displayAll();
                break;
            case 3:  // View one account
                cout << "Enter Account Number: ";
                cin >> accNo;
                viewAccount(accNo);
                break;
            case 4:  // Deposit money
                cout << "Enter Account Number: ";
                cin >> accNo;
                updateAccount(accNo, true);
                break;
            case 5:  // Withdraw money
                cout << "Enter Account Number: ";
                cin >> accNo;
                updateAccount(accNo, false);
                break;
            case 6:  // Exit program
                cout << "Exiting program...\n";
                break;
            default: // Invalid input
                cout << "Invalid choice.\n";
        }
    } while (choice != 6); // Loop until Exit option is chosen

    return 0;
}
