#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Class to manage Account Records (Object-Oriented Approach)
class Account {
private:
    int accountNumber;
    char name[50];
    double balance;

public:
    // Function to take user inputs
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore(); // Clear input buffer
        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);
        cout << "Enter Initial Deposit Amount: $";
        cin >> balance;
        cout << "\nAccount Created Successfully!";
    }

    // Function to display account details
    void showAccount() const {
        cout << left << setw(15) << accountNumber 
             << setw(25) << name 
             << "$ " << fixed << setprecision(2) << balance << endl;
    }

    // Function to accept deposits
    void deposit(double amount) {
        balance += amount;
    }

    // Function to accept withdrawals
    bool withdraw(double amount) {
        if (amount > balance) {
            return false; // Insufficient funds
        }
        balance -= amount;
        return true;
    }

    // Getter functions
    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
};

// Function Declarations
void createNewAccount();
void displayAllAccounts();
void checkBalance();
void depositMoney();
void withdrawMoney();

int main() {
    int choice;

    while (true) {
        cout << "\n====================================";
        cout << "\n       BANK MANAGEMENT SYSTEM       ";
        cout << "\n====================================";
        cout << "\n1. Open New Account";
        cout << "\n2. Display All Accounts";
        cout << "\n3. Balance Inquiry";
        cout << "\n4. Deposit Money";
        cout << "\n5. Withdraw Money";
        cout << "\n6. Exit";
        cout << "\n------------------------------------";
        cout << "\nEnter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: createNewAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: checkBalance(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: 
                cout << "\nThank you for banking with us. Goodbye!\n"; 
                return 0;
            default: 
                cout << "\nInvalid choice! Please try again.\n";
        }
    }
    return 0;
}

// 1. Function to Write a New Account Record to File
void createNewAccount() {
    Account acc;
    ofstream outFile("accounts.dat", ios::binary | ios::app);

    if (!outFile) {
        cout << "\nFile Error: Could not open database.";
        return;
    }

    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

// 2. Function to View All Existing Accounts
void displayAllAccounts() {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);

    if (!inFile) {
        cout << "\nNo accounts found. Please open an account first!\n";
        return;
    }

    cout << "\n=========================================================\n";
    cout << left << setw(15) << "Account No." << setw(25) << "Holder Name" << "Balance" << endl;
    cout << "=========================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
    }
    cout << "---------------------------------------------------------\n";
    inFile.close();
}

// 3. Function to Check Balance of a Specific Account
void checkBalance() {
    int targetAccNum;
    Account acc;
    bool found = false;

    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile) {
        cout << "\nDatabase Error.\n";
        return;
    }

    cout << "\nEnter Account Number: ";
    cin >> targetAccNum;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == targetAccNum) {
            cout << "\n====================================\n";
            cout << "Account Holder: " << acc.getAccountNumber() << "\n";
            cout << "Current Balance: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
            cout << "====================================\n";
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) cout << "\nAccount number not found.\n";
}

// 4. Function to Update Balance on Deposit
void depositMoney() {
    int targetAccNum;
    double amount;
    Account acc;
    bool found = false;

    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nDatabase Error.\n";
        return;
    }

    cout << "\nEnter Account Number: ";
    cin >> targetAccNum;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == targetAccNum) {
            found = true;
            cout << "\nEnter Amount to Deposit: $";
            cin >> amount;
            
            if(amount <= 0) {
                cout << "\nInvalid deposit amount.\n";
                break;
            }

            acc.deposit(amount);

            int pos = -1 * static_cast<int>(sizeof(Account));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            
            cout << "\nDeposit successful! New Balance: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
            break;
        }
    }
    file.close();
    if (!found) cout << "\nAccount number not found.\n";
}

// 5. Function to Update Balance on Withdrawal
void withdrawMoney() {
    int targetAccNum;
    double amount;
    Account acc;
    bool found = false;

    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nDatabase Error.\n";
        return;
    }

    cout << "\nEnter Account Number: ";
    cin >> targetAccNum;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == targetAccNum) {
            found = true;
            cout << "\nEnter Amount to Withdraw: $";
            cin >> amount;

            if(amount <= 0) {
                cout << "\nInvalid withdrawal amount.\n";
                break;
            }

            if (acc.withdraw(amount)) {
                int pos = -1 * static_cast<int>(sizeof(Account));
                file.seekp(pos, ios::cur);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                cout << "\nWithdrawal successful! New Balance: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
            } else {
                cout << "\nTransaction Declined: Insufficient Funds!\n";
            }
            break;
        }
    }
    file.close();
    if (!found) cout << "\nAccount number not found.\n";
}