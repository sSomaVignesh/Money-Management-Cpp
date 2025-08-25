#include <iostream>
#include <string>
#include <map>

using namespace std;

struct User {
    string username;
    string password;
    double totalFunds = 0;
    double expenses = 0, emergency = 0, savings = 0;
};

// Simple in-memory storage (can be expanded to file/database later)
map<string, User> users;

// Sign up function
void signUp() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    if (users.find(username) != users.end()) {
        cout << "Username already exists!\n";
        return;
    }

    User newUser;
    newUser.username = username;
    newUser.password = password;
    users[username] = newUser;

    cout << "Account created successfully!\n";
}

// Login function
User* login() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end() && users[username].password == password) {
        cout << "Login successful!\n";
        return &users[username];
    } else {
        cout << "Invalid credentials!\n";
        return nullptr;
    }
}

// Add income and split into 5:3:2
void addIncome(User &u) {
    double amount;
    cout << "Enter income amount: ";
    cin >> amount;

    u.totalFunds += amount;
    u.expenses += amount * 0.5;
    u.emergency += amount * 0.3;
    u.savings += amount * 0.2;

    cout << "Income added and budget allocated (50:30:20)!\n";
}

// Record expenses with optional savings usage
void recordExpense(User &u) {
    double amount;
    cout << "Enter expense amount: ";
    cin >> amount;

    double available = u.expenses + u.emergency;

    // Case 1: Covered by expenses + emergency
    if (available >= amount) {
        double remaining = amount;

        if (u.expenses >= remaining) {
            u.expenses -= remaining;
            cout << "Expense of " << amount << " recorded from Monthly Expenses.\n";
        } else {
            cout << "Used " << u.expenses << " from Monthly Expenses.\n";
            remaining -= u.expenses;
            u.expenses = 0;

            u.emergency -= remaining;
            cout << "Used " << remaining << " from Emergency Funds.\n";
        }
        return;
    }

    // Case 2: Not enough in expenses + emergency, ask about savings
    cout << "Not enough funds in Monthly Expenses and Emergency Funds.\n";
    cout << "Do you want to use Savings? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        double totalAvailable = u.expenses + u.emergency + u.savings;

        if (totalAvailable >= amount) {
            double remaining = amount;

            // Deduct from expenses
            if (u.expenses >= remaining) {
                u.expenses -= remaining;
                cout << "Expense of " << amount << " recorded from Monthly Expenses.\n";
                return;
            } else {
                cout << "Used " << u.expenses << " from Monthly Expenses.\n";
                remaining -= u.expenses;
                u.expenses = 0;
            }

            // Deduct from emergency
            if (u.emergency >= remaining) {
                u.emergency -= remaining;
                cout << "Used " << remaining << " from Emergency Funds.\n";
                return;
            } else {
                cout << "Used " << u.emergency << " from Emergency Funds.\n";
                remaining -= u.emergency;
                u.emergency = 0;
            }

            // Deduct remaining from savings
            u.savings -= remaining;
            cout << "Used " << remaining << " from Savings.\n";
        } else {
            cout << "Not enough funds even with Savings included!\n";
        }
    } else {
        cout << "Transaction cancelled. Savings untouched.\n";
    }
}



// Show balance
void showBalance(User &u) {
    cout << "\n---- Current Budget Status ----\n";
    cout << "Total Funds: " << u.totalFunds << endl;
    cout << "Monthly Expenses (50%): " << u.expenses << endl;
    cout << "Emergency & Luxury (30%): " << u.emergency << endl;
    cout << "Savings (20%): " << u.savings << endl;
    cout << "--------------------------------\n";
}

int main() {
    int choice;
    User *currentUser = nullptr;

    while (true) {
        cout << "\n--- Money Management System ---\n";
        if (!currentUser) {
            cout << "1. Sign Up\n2. Login\n3. Exit\n";
            cin >> choice;

            if (choice == 1) signUp();
            else if (choice == 2) currentUser = login();
            else break;
        } else {
            cout << "1. Add Income\n2. Record Expense\n3. Show Balance\n4. Logout\n";
            cin >> choice;

            if (choice == 1) addIncome(*currentUser);
            else if (choice == 2) recordExpense(*currentUser);
            else if (choice == 3) showBalance(*currentUser);
            else if (choice == 4) currentUser = nullptr;
        }
    }

    return 0;
}
