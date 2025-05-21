#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <random>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

#define RESET       "\033[0m"
#define RED         "\033[38;2;204;17;29m"
#define NAVY        "\033[38;2;24;43;58m"
#define TEAL        "\033[38;2;50;72;81m"
#define SLATE       "\033[38;2;75;129;140m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

random_device rd;
mt19937 gen(rd());

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void getUser(string &user)
{
    cout << " Username :" << endl;
    getline(cin, user);
}
void getPass(string &pass)
{
    cout << " Password :" << endl;
    getline(cin, pass);
}
void displaySign()
{
    cout << BOLD << NAVY;
    cout << "\n============================================\n";
    cout << "        WELCOME TO " << RED << "DAGGZ BANKING SYSTEM" << NAVY << "        \n";
    cout << "============================================\n";
    cout << RESET << endl;
    cout << "Welcome to DAGGZ!" << endl
         << endl;
}
string generateLogID()
{
    string logID = {"AUDIT"};
    for (int i = 0; i < 6; i++)
    {
        uniform_int_distribution<> distrib(0, 9);
        int randomNum = distrib(gen);
        char digit = '0' + randomNum;
        logID += digit;

        if (i == 2)
        {
            logID += "-";
        }
    }
    return logID;
}
string getFromFile(string accNumber, int n)
{
    ifstream file("SYSTEM_USERS.csv");
    ostringstream toString;
    string line, accNum = {""}, num, name, fullName, pass, pin;
    double balance;
    int location;

    while (getline(file, line))
    {
        location = line.find(",");
        accNum = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        num = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        name = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        fullName = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        pass = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        pin = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        if (location != string::npos)
        {
            balance = stod(line.substr(0, location));
        }
        else
        {
            balance = stod(line);
        }

        if (accNumber == accNum)
        {
            switch (n)
            {
            case 1:
                return accNum;
                break;
            case 2:
                return num;
                break;
            case 3:
                return name;
                break;
            case 4:
                return fullName;
                break;
            case 5:
                return pass;
                break;
            case 6:
                return pin;
                break;
            case 7:
                return to_string(balance);
                break;
            default:
                break;
            }
        }
    }
    file.close();
    return accNum;
}
string getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    ostringstream datetime;
    datetime << (1900 + ltm->tm_year) << "-"
             << setw(2) << setfill('0') << (1 + ltm->tm_mon) << "-"
             << setw(2) << setfill('0') << ltm->tm_mday << " "
             << setw(2) << setfill('0') << ltm->tm_hour << ":"
             << setw(2) << setfill('0') << ltm->tm_min << ":"
             << setw(2) << setfill('0') << ltm->tm_sec;

    return datetime.str();
}
void setAuditLog(string accNumber, string Action, double beforeBalance, double afterBalance)
{
    // logID,time,userid,username,action,beforeBalance,afterBalance
    ofstream file;
    ostringstream toString;
    string logID;
    file.open("SYSTEM_AUDIT_LOG.csv", ios::app);

    logID = generateLogID();
    toString << logID << ","
             << getCurrentDateTime() << ","
             << accNumber << ","
             << getFromFile(accNumber, 3) << ","
             << Action << ","
             << fixed << setprecision(2) << beforeBalance << ","
             << fixed << setprecision(2) << afterBalance << endl;

    file << toString.str();
    file.close();
}

void pinVerification(string &pin, int &n)
{
    string inputPin;
    int attempts = {5};

    do
    {
        cout << "Enter your 6-digit PIN(0 to return) : ";
        getline(cin, inputPin);
        if (inputPin == "0")
        {
            n = -1;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            return;
        }
        if (inputPin != pin)
        {
            attempts--;
            if (attempts > 0)
            {
                clearScreen();
                cout << RED << "Invalid PIN. " << attempts << " attempts remaining." << RESET << endl;
            }
            else
            {
                cout << RED << "Error: Maximum attempts reached. Please contact the administrator." << RESET << endl
                     << endl;
                cout << "Automatically Logging out..." << endl
                     << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                n = 0;
                return;
            }
        }
        else
        {
            clearScreen();
            break;
        }
    } while (attempts > 0);
}

struct Isvalid
{
    bool number(string number)
    {
        if (number.length() != 11)
        {
            return false;
        }

        for (int i = 0; i < number.length(); i++)
        {
            if (!isdigit(number[i]))
            {
                return false;
            }
        }

        if (number[0] != '0' || number[1] != '9')
        {
            return false;
        }
        return true;
    }

    bool usernameExists(string username)
    {
        ifstream file("SYSTEM_USERS.csv");
        string line;
        int location;

        while (getline(file, line))
        {
            // Skip account number
            location = line.find(",");
            line = line.substr(location + 1);

            // Skip phone number
            location = line.find(",");
            line = line.substr(location + 1);

            // Get username
            location = line.find(",");
            string existingUsername = line.substr(0, location);

            if (username == existingUsername)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    bool username(string name)
    {
        bool haslower = false, hasdigit = false, hasupper = false;
        if (name.length() < 6 || name.length() > 24)
        {
            return false;
        }
        for (int i = 0; i < name.length(); i++)
        {
            if (!isalnum(name[i]) || name[i] == '-')
            {
                return false;
            }
            if (islower(name[i]))
            {
                haslower = true;
            }

            if (isupper(name[i]))
            {
                hasupper = true;
            }
            if (isdigit(name[i]))
            {
                hasdigit = true;
            }
        }
        return (haslower && hasdigit && hasupper);
    }

    bool password(string pass)
    {
        bool haslower = false, hasdigit = false, hasupper = false;
        if (pass.length() < 6)
        {
            return false;
        }
        for (int i = 0; i < pass.length(); i++)
        {
            if (!isalnum(pass[i]))
            {
                return false;
            }
            if (islower(pass[i]))
            {
                haslower = true;
            }

            if (isupper(pass[i]))
            {
                hasupper = true;
            }
            if (isdigit(pass[i]))
            {
                hasdigit = true;
            }
        }
        return (haslower && hasdigit && hasupper);
    }
    bool pin(string pin)
    {
        if (pin.length() != 6)
        {
            return false;
        }
        for (int i = 0; i < pin.length(); i++)
        {
            if (!isdigit(pin[i]))
            {
                return false;
            }
        }
        return true;
    }
    bool fullName(string name)
    {
        // Check if name is empty
        if (name.empty()) {
            return false;
        }

        // Check each character
        for (char c : name) {
            // Allow letters and spaces
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }

        return true;
    }
};

struct Authenticate
{
    int location;

    string user(string user, string password)
    {
        ifstream file("SYSTEM_USERS.csv");
        ostringstream toString;
        string line, accNum = {"0000-0000-0000"}, num, name, fullName, pass, pin;
        double balance;
        int location;

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
            }
            else
            {
                balance = stod(line);
            }

            if (user == name && password == pass)
            {
                return accNum;
            }
        }
        file.close();
        accNum = {"0000-0000-0000"};
        return accNum;
    }
};

struct Signup
{
    ofstream accounts;
    ostringstream toString;
    Isvalid isValid;
    string accNumber, number, username, password, pin, fullName;
    double balance = 0.00;
    bool checker = false;

    void getInfo()
    {
        accounts.open("SYSTEM_USERS.csv", ios::app);

        do
        {
            if (checker)
            {
                cout << "\nError: Invalid fullname must be alphabetical letters only please try again" << "\033[0m" << endl;
            }
            cout << "\033[3m" << "\nmust be alphabetical letters only " << "\033[0m" << endl;
            cout << "\nEnter your Full Name (0 to return): ";
            getline(cin, fullName);
            if (fullName == "0")
            {
                return;
            }
            checker = !isValid.fullName(fullName);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "\nError: Invalid phone number please try again" << endl;
            }

            cout << "\033[3m" << " \nOnly digits " << "\033[0m" << endl;
            cout << "\033[3m" << " Ph number only (09) " << "\033[0m" << endl;
            cout << "\033[3m" << " No spaces, hyphens, or letters" << "\033[0m" << endl;
            cout << "\nNumber : " << endl;
            cin >> number;
            if (number == "0")
            {
                return;
            }
            checker = isValid.number(number);
        } while (!checker);

        cin.ignore();
        checker = false;

        do
        {
            if (checker)
            {
                cout << "\nError: Invalid username please try again" << endl;
            }

            cout << "\033[3m" << "\nMust contain: " << "\033[0m" << endl;
            cout << "\033[3m" << "At least one lowercase letter (a-z)" << "\033[0m" << endl;
            cout << "\033[3m" << "At least one uppercase letter (A-Z)" << "\033[0m" << endl;
            cout << "\033[3m" << "At least one digit (0-9)" << "\033[0m" << endl;
            cout << "\033[3m" << "Must not contain hyphens (-)" << "\033[0m" << endl;
            cout << "\033[3m" << "Minimum length: typically 6–12 characters" << "\033[0m" << endl;

            getUser(username);
            if (fullName == "0")
            {
                return;
            }
            if (isValid.usernameExists(username))
            {
                cout << "\nError: Username already exists. Please choose a different username." << endl;
                checker = true;
                continue;
            }
            checker = !isValid.username(username);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "\nError: Invalid password please try again" << endl;
            }
            cout << "\033[3m" << "\nMust contain: " << "\033[0m" << endl;
            cout << "\033[3m" << "At least one lowercase letter (a-z)" << "\033[0m" << endl;
            cout << "\033[3m" << "At least one uppercase letter (A-Z)" << "\033[0m" << endl;
            cout << "\033[3m" << "At least one digit (0-9)" << "\033[0m" << endl;
            cout << "\033[3m" << "Minimum length: 8 characters" << "\033[0m" << endl;
            getPass(password);
            if (fullName == "0")
            {
                return;
            }
            checker = isValid.password(password);
        } while (!checker);
        checker = false;
        do
        {
            string secondTry;
            if (checker)
            {
                cout << "\nError: Invalid pin please try again" << endl;
            }
            cout << "\033[3m" << "\nMust contain a 6 combination number" << "\033[0m" << endl;
            cout << "\nEnter your 6 digit PIN (0 to return) : " << endl;
            cin >> pin;
            cin.ignore();
            if (fullName == "0")
            {
                return;
            }
            if (!isValid.pin(pin))
            {
                checker = !isValid.pin(pin);
            }
            else
            {
                cout << "\nRe-enter your 6 digit PIN : " << endl;
                cin >> secondTry;
                cin.ignore();
                if (pin == secondTry)
                {
                    checker = !isValid.pin(pin);
                }
                else
                {
                    checker = true;
                }
            }
        } while (checker);

        accNumber = accountNumGenerate();

        toString << accNumber << ","
                 << number << ","
                 << username << ","
                 << fullName << ","
                 << password << ","
                 << pin << ","
                 << balance << endl;
        accounts << toString.str();

        string newFile = accNumber + ".csv";
        ofstream newfile(newFile);

        accounts.close();
        newfile.close();
        clearScreen();
        cout << "Successfully created account!" << endl
             << endl;
        cout << "Full Name: " << "\033[3m" << fullName << "\033[0m" << endl;
        cout << "Username : " << "\033[3m" << username << "\033[0m" << endl;
        cout << "Account Number : " << "\033[3m" << accNumber << "\033[0m" << endl;

        setAuditLog(accNumber, "CREATE ACCOUNT", 0.00, 0.00);
    }

    string accountNumGenerate()
    {
        string accNum = {""};
        for (int i = 0; i < 12; i++)
        {
            uniform_int_distribution<> distrib(0, 9);
            int randomNum = distrib(gen);
            char digit = '0' + randomNum;
            accNum += digit;

            if (i == 3 || i == 7)
            {
                accNum += "-";
            }
        }
        return accNum;
    }
};

struct Signin
{
    Authenticate authenticate;

    string accNumber, number, username, fullname, password, pin, c;
    int balance;
    int n;

    void signin()
    {
        bool checker = true;
        string name = {""}, pass = {""};

        displaySign();
        do
        {
            if (!checker)
            {
                cout << "\nIncorrect Username or Password" << endl;
            }

            getUser(name);
            getPass(pass);

            if (name == "0")
            {
                return;
            }

            if (authenticate.user(name, pass) == "0000-0000-0000")
            {
                checker = false;
            }
            else
            {
                c = authenticate.user(name, pass);
                // Check if account is frozen
                ifstream file("SYSTEM_USERS.csv");
                string line;
                bool isFrozen = false;

                while (getline(file, line))
                {
                    if (line.find(c) == 0)
                    {
                        if (line.find(",FROZEN") != string::npos)
                        {
                            isFrozen = true;
                        }
                        break;
                    }
                }
                file.close();

                if (isFrozen)
                {
                    cout << "\nError: This account has been frozen." << endl;
                    cout << "\nPlease contact the administrator at 09693381708 to unfreeze your account." << endl;
                    setAuditLog(c, "FAILED LOGIN - FROZEN ACCOUNT", 0.00, 0.00);

                    cout << "\nPress Enter to continue...";
                    cin.get();
                    clearScreen();

                    return;
                }

                accNumber = getFromFile(c, 1);
                number = getFromFile(c, 2);
                username = getFromFile(c, 3);
                fullname = getFromFile(c, 4);
                password = getFromFile(c, 5);
                pin = getFromFile(c, 6);
                balance = stod(getFromFile(c, 7));
                setAuditLog(accNumber, "LOGIN", 0.00, 0.00);
                user();
                checker = true;
                clearScreen();
            }
        } while (!checker);
    }

    void user()
    {
        do
        {
            cout << NAVY << BOLD;
            cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << "          USER MENU" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
            cout << TEAL << "[1] " << RESET << "Check Balance" << endl;
            cout << TEAL << "[2] " << RESET << "Withdraw Cash" << endl;
            cout << TEAL << "[3] " << RESET << "Deposit Cash" << endl;
            cout << TEAL << "[4] " << RESET << "Transaction History" << endl;
            cout << TEAL << "[5] " << RESET << "Change PIN" << endl;
            cout << TEAL << "[6] " << RESET << "View Information" << endl;
            cout << TEAL << "[7] " << RESET << "Change Account Information" << endl;
            cout << TEAL << "[0] " << RESET << "Logout" << endl;
            cout << BOLD << "\nEnter your choice: " << RESET;
            cin >> n;
            cin.ignore();
            clearScreen();

            switch (n)
            {
            case 1:
                checkBalance();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 2:
                withdraw(accNumber);
                break;
            case 3:
                deposit(accNumber);
                break;
            case 4:
                viewTransaction();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 5:
                changePin(accNumber);
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 6:
                viewInformation();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 7:
                changeAccountInfo();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 0:
                cout << "Logging out..." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            }

        } while (n != 0);
    }

    void viewInformation()
    {
        cout << NAVY << BOLD;
        cout << "\n==========================================" << endl;
        cout << "              ACCOUNT INFORMATION         " << endl;
        cout << "==========================================" << RESET << endl;
        cout << "Account Number: " << accNumber << endl;
        cout << "Full Name: " << getFromFile(accNumber, 4) << endl;
        cout << "Username: " << username << endl;
        cout << "Phone Number: " << getFromFile(accNumber, 2) << endl;
        cout << "Current Balance: ₱" << fixed << setprecision(2) << stod(getFromFile(accNumber, 7)) << endl;
        cout << NAVY << "==========================================" << RESET << endl;

        setAuditLog(accNumber, "VIEW INFORMATION", 0.00, 0.00);
    }

    void checkBalance()
    {
        double balance = stod(getFromFile(accNumber, 7));
        cout << NAVY << BOLD;
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "          BALANCE INQUIRY" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
        cout << " CURRENT BALANCE : ₱" << fixed << setprecision(2) << balance << endl;
        setAuditLog(accNumber, "CHECK BALANCE", balance, balance);
    }

    void generateReceipt(string transactionType, double amount, double balance)
    {
        cout << NAVY << BOLD;
        cout << "\n==========================================" << endl;
        cout << "              TRANSACTION RECEIPT          " << endl;
        cout << "==========================================" << RESET << endl;
        cout << "Date & Time: " << getCurrentDateTime() << endl;
        cout << "Account Number: " << accNumber << endl;
        cout << "Full Name: " << getFromFile(accNumber, 4) << endl;
        cout << "Username: " << username << endl;
        cout << "Transaction Type: " << transactionType << endl;
        cout << "Amount: ₱" << fixed << setprecision(2) << amount << endl;
        cout << "Current Balance: ₱" << fixed << setprecision(2) << balance << endl;
        cout << "Transaction ID: " << transactionIdGenerate() << endl;
        cout << NAVY << "==========================================" << endl;
        cout << "Thank you for banking with us!" << endl;
        cout << "==========================================\n" << RESET << endl;
    }

    void withdraw(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, fullName, pass, pin;
        string inputPin;
        int attempts = 5;
        double balance;
        int location;
        double inputBalance;
        bool repeater;
        bool found = false;

        do
        {
            repeater = true;
            cout << SLATE << "(Note that the bank only withdraw in denomination of 100, 200, 500 and 1000)" << RESET << endl;
            cout << "Choose amount to Withdraw(0 to return) : ";
            cin >> inputBalance;
            cin.ignore();
            if (inputBalance == 0)
            {
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                return;
            }
            if (inputBalance < 0)
            {
                cout << RED << "Error: Cannot withdraw negative amount!" << RESET << endl;
                repeater = false;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                continue;
            }
            if (0 != (static_cast<int>(inputBalance) % 100))
            {
                cout << RED << "Error: Amount must be in denominations of 100, 200, 500, or 1000" << RESET << endl;
                repeater = false;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
            }
            if (inputBalance > this->balance)
            {
                cout << RED << "Error: Insufficient balance. Your current balance is ₱" << fixed << setprecision(2) << this->balance << RESET << endl;
                repeater = false;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
            }
        } while (!repeater);

        pinVerification(this->pin, n);
        if (n == -1)
        {
            setAuditLog(ID, "CANCELLED WITHDRAW", this->balance, this->balance);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }
        if (n == 0) // Check for logout
        {
            setAuditLog(ID, "FAILED WITHDRAW - PIN VERIFICATION FAILED", this->balance, this->balance);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
            }
            else
            {
                balance = stod(line);
            }

            if (accNum == ID && !found)
            {
                double newBalance = balance - inputBalance;
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << newBalance << endl;
                found = true;
                setAuditLog(ID, "WITHDRAW", balance, newBalance);
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << balance;
                if (line.find(",") != string::npos)
                {
                    temp << "," << line.substr(line.find(",") + 1);
                }
                temp << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->balance = stod(getFromFile(c, 7));

        toHistory(inputBalance, "WITHDRAW");
        generateReceipt("WITHDRAW", inputBalance, this->balance);
    }

    void deposit(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, fullName, pass, pin;
        double balance;
        int location;
        double inputBalance;
        bool repeater;
        bool found = false;

        do
        {
            repeater = true;
            cout << "Enter amount to Deposit(0 to return) : ";
            cin >> inputBalance;
            cin.ignore();
            if (inputBalance == 0)
            {
                file.close();
                temp.close();
                remove("temp.csv");
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                return;
            }
            if (inputBalance < 0)
            {
                cout << RED << "Error: Cannot deposit negative amount!" << RESET << endl;
                repeater = false;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                continue;
            }
            if (inputBalance < 99)
            {
                cout << RED << "Error: Minimum deposit is ₱100" << RESET << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);
        if (n == -1)
        {
            setAuditLog(ID, "CANCELLED WITHDRAW", this->balance, this->balance);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }
        if (n == 0)
        {
            setAuditLog(ID, "FAILED DEPOSIT - PIN VERIFICATION FAILED", this->balance, this->balance);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
            }
            else
            {
                balance = stod(line);
            }

            if (accNum == ID && !found)
            {
                double newBalance = balance + inputBalance;
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << newBalance << endl;
                found = true;
                setAuditLog(ID, "DEPOSIT", balance, newBalance);
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << balance;
                if (line.find(",") != string::npos)
                {
                    temp << "," << line.substr(line.find(",") + 1);
                }
                temp << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->balance = stod(getFromFile(c, 7));

        toHistory(inputBalance, "DEPOSIT");
        generateReceipt("DEPOSIT", inputBalance, this->balance);
    }

    void viewTransaction()
    {
        ifstream file(accNumber + ".csv");
        string line;
        string transactionID, accountNumber, type, amount, amountAfter, time;
        int location;

        cout << "\nTransaction History:" << endl;
        cout << setw(15) << "TransactionID"
             << setw(25) << "Account Number"
             << setw(15) << "Type"
             << setw(15) << "Amount"
             << setw(15) << "Amount after"
             << setw(25) << "Time"
             << endl;

        while (getline(file, line))
        {
            location = line.find(",");
            transactionID = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            accountNumber = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            type = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            amount = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            amountAfter = line.substr(0, location);
            line = line.substr(location + 1);

            time = line;

            cout << setw(15) << transactionID
                 << setw(25) << accountNumber
                 << setw(15) << type
                 << setw(15) << amount
                 << setw(15) << amountAfter
                 << setw(25) << time
                 << endl;
        }

        file.close();
        setAuditLog(accNumber, "VIEW TRANSACTION HISTORY", 0.00, 0.00);
    }

    void changePin(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        Isvalid isValid;
        string line, accNum, num, name, fullName, pass, pin;
        string inputPin;
        int attempts = 5;
        double balance, newBal = 0;
        int location;
        string newPin, reEnter;
        bool repeater;
        bool found = false, checker = true;

        string username = {""}, password = {""};

        do
        {
            cout << " AUTHENTICATION " << endl
                 << endl;
            if (!checker)
            {
                cout << "Incorrect Username or Password" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
            }
            getUser(username);
            getPass(password);

            if (name == "0")
            {
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                return;
            }

            if (authenticate.user(username, password) == "0000-0000-0000")
            {
                checker = false;
            }
            else
            {
                checker = true;
            }
        } while (!checker);

        pinVerification(this->pin, n);
        if (n == -1)
        {
            setAuditLog(ID, "CANCELLED WITHDRAW", this->balance, this->balance);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }
        if (n == 0)
        {
            setAuditLog(ID, "FAILED PIN CHANGE - PIN VERIFICATION FAILED", 0.00, 0.00);
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }

        do
        {
            checker = false;

            cout << "\033[3m" << "Must contain a 6 combination number" << "\033[0m" << endl;
            cout << "\nEnter your New 6 digit PIN : " << endl;
            getline(cin, newPin);

            if (!isValid.pin(newPin))
            {
                clearScreen();
                cout << "Invalid pin please try again" << endl;
                checker = !isValid.pin(newPin);
            }
            else
            {
                cout << "\nRe-enter your New 6 digit PIN : " << endl;
                getline(cin, reEnter);
                if (newPin == reEnter)
                {
                    checker = !isValid.pin(newPin);
                }
                else
                {
                    clearScreen();
                    cout << "Error : PINs do not match" << endl;
                    repeater = true;
                }
            }
        } while (checker);

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
            }
            else
            {
                balance = stod(line);
            }

            if (accNum == ID && !found)
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << reEnter << ","
                     << balance;
                if (line.find(",") != string::npos)
                {
                    temp << "," << line.substr(line.find(",") + 1);
                }
                temp << endl;
                found = true;
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << balance;
                if (line.find(",") != string::npos)
                {
                    temp << "," << line.substr(line.find(",") + 1);
                }
                temp << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->pin = newPin;

        toHistory(newBal, "CHANGE PIN");
        setAuditLog(ID, "CHANGE PIN", 0.00, 0.00);
    }

    void toHistory(double &inputBalance, string action)
    {
        string transactionId = transactionIdGenerate();
        ostringstream toString;

        toString << transactionId << ","
                 << accNumber << ","
                 << action << ","
                 << inputBalance << ","
                 << balance << ","
                 << getCurrentDateTime() << endl;

        ofstream transactionFile(accNumber + ".csv", ios::app);
        transactionFile << toString.str();
        transactionFile.close();
    }

    string transactionIdGenerate()
    {
        string transId = {""};
        for (int i = 0; i < 9; i++)
        {
            uniform_int_distribution<> distrib(0, 9);
            int randomNum = distrib(gen);
            char digit = '0' + randomNum;
            transId += digit;

            if (i == 2 || i == 5)
            {
                transId += "-";
            }
        }
        return transId;
    }

    void changeAccountInfo()
    {
        int choice;
        string newValue;
        bool isValid = false;
        Isvalid validator;

        cout << "\n==========================================" << endl;
        cout << "          CHANGE ACCOUNT INFORMATION      " << endl;
        cout << "==========================================" << endl;
        cout << "1. Change Full Name" << endl;
        cout << "2. Change Username" << endl;
        cout << "3. Change Phone Number" << endl;
        cout << "4. Change Password" << endl;
        cout << "0. Return to Menu" << endl;
        cout << "==========================================" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();
        clearScreen();

        if (choice == 0)
        {
            return;
        }

        // Verify PIN before making changes
        pinVerification(this->pin, n);
        if (n == -1 || n == 0)
        {
            setAuditLog(accNumber, "FAILED ACCOUNT INFO CHANGE - PIN VERIFICATION FAILED", 0.00, 0.00);
            return;
        }

        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, fullName, pass, pin;
        double balance;
        int location;
        bool found = false;

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
            }
            else
            {
                balance = stod(line);
            }

            if (accNum == accNumber && !found)
            {
                found = true;
                switch (choice)
                {
                case 1: // Change Full Name
                    do
                    {
                        cout << "\nEnter new Full Name (0 to cancel): ";
                        getline(cin, newValue);
                        if (newValue == "0")
                        {
                            file.close();
                            temp.close();
                            remove("temp.csv");
                            return;
                        }
                        isValid = validator.fullName(newValue);
                        if (!isValid)
                        {
                            cout << "\nError: Invalid full name. Must contain only letters." << endl;
                        }
                    } while (!isValid);
                    fullName = newValue;
                    setAuditLog(accNumber, "CHANGE FULL NAME", 0.00, 0.00);
                    break;

                case 2: // Change Username
                    do
                    {
                        cout << "\nEnter new Username (0 to cancel): ";
                        getline(cin, newValue);
                        if (newValue == "0")
                        {
                            file.close();
                            temp.close();
                            remove("temp.csv");
                            return;
                        }
                        if (validator.usernameExists(newValue))
                        {
                            cout << "\nError: Username already exists. Please choose a different one." << endl;
                            isValid = false;
                        }
                        else
                        {
                            isValid = validator.username(newValue);
                            if (!isValid)
                            {
                                cout << "\nError: Invalid username format." << endl;
                            }
                        }
                    } while (!isValid);
                    name = newValue;
                    username = newValue;
                    setAuditLog(accNumber, "CHANGE USERNAME", 0.00, 0.00);
                    break;

                case 3: // Change Phone Number
                    do
                    {
                        cout << "\nEnter new Phone Number (0 to cancel): ";
                        getline(cin, newValue);
                        if (newValue == "0")
                        {
                            file.close();
                            temp.close();
                            remove("temp.csv");
                            return;
                        }
                        isValid = validator.number(newValue);
                        if (!isValid)
                        {
                            cout << "\nError: Invalid phone number format." << endl;
                        }
                    } while (!isValid);
                    num = newValue;
                    setAuditLog(accNumber, "CHANGE PHONE NUMBER", 0.00, 0.00);
                    break;

                case 4: // Change Password
                    do
                    {
                        cout << "\nEnter new Password (0 to cancel): ";
                        getline(cin, newValue);
                        if (newValue == "0")
                        {
                            file.close();
                            temp.close();
                            remove("temp.csv");
                            return;
                        }
                        isValid = validator.password(newValue);
                        if (!isValid)
                        {
                            cout << "\nError: Invalid password format." << endl;
                        }
                    } while (!isValid);
                    pass = newValue;
                    setAuditLog(accNumber, "CHANGE PASSWORD", 0.00, 0.00);
                    break;
                }

                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << balance;
                if (line.find(",") != string::npos)
                {
                    temp << "," << line.substr(line.find(",") + 1);
                }
                temp << endl;
            }
            else
            {
                temp << line << endl;
            }
        }

        file.close();
        temp.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");

        cout << "\nAccount information updated successfully!" << endl;
    }
};

struct Admin
{
    bool authenticateAdmin()
    {
        string username, password;
        cout << "\nEnter admin username: ";
        getline(cin, username);
        cout << "Enter admin password: ";
        getline(cin, password);

        return (username == "admin" && password == "admin");
    }

    void adminMenu()
    {
        if (!authenticateAdmin())
        {
            cout << RED << "Invalid admin credentials!" << RESET << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            return;
        }
        clearScreen();
        int choice;
        do
        {
            cout << NAVY << BOLD;
            cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << "          ADMIN PANEL" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
            cout << TEAL << "[1] " << RESET << "View All Customers" << endl;
            cout << TEAL << "[2] " << RESET << "Freeze/Unfreeze Account" << endl;
            cout << TEAL << "[3] " << RESET << "Audit Logs" << endl;
            cout << TEAL << "[0] " << RESET << "Logout" << endl;
            cout << BOLD << "\nEnter your choice: " << RESET;
            cin >> choice;
            cin.ignore();
            clearScreen();
            switch (choice)
            {
            case 1:
                viewAllCustomers();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 2:
                freezeUnfreezeAccount();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 3:
                viewAuditLog();
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            case 0:
                cout << "Logging out..." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                break;
            }
        } while (choice != 0);
    }

    void viewAllCustomers()
    {
        ifstream file("SYSTEM_USERS.csv");
        string line;
        string accNum, num, name, fullName, pass, pin, status;
        double balance;
        int location;

        cout << "\nAll Customer Accounts:" << endl;
        cout << setw(20) << "Account Number"
             << setw(15) << "Phone Number"
             << setw(20) << "Username"
             << setw(30) << "Full Name"
             << setw(15) << "Balance"
             << setw(10) << "Status" << endl;
        cout << string(110, '-') << endl;

        while (getline(file, line))
        {
            location = line.find(",");
            accNum = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            num = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            name = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            fullName = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            pass = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            if (location != string::npos)
            {
                balance = stod(line.substr(0, location));
                status = line.substr(location + 1);
            }
            else
            {
                balance = stod(line);
                status = "ACTIVE";
            }

            cout << setw(20) << accNum
                 << setw(15) << num
                 << setw(20) << name
                 << setw(30) << fullName
                 << setw(15) << fixed << setprecision(2) << balance
                 << setw(10) << status << endl;
        }
        cout << string(110, '-') << endl;
        file.close();
    }

    void freezeUnfreezeAccount()
    {
        // Show all users first
        viewAllCustomers();

        // Now proceed with freeze/unfreeze
        string accNumber;
        int action;
        cout << "\nEnter account number to modify (0 to return): ";
        getline(cin, accNumber);

        if (accNumber == "0")
        {
            return;
        }

        cout << "1. Freeze Account" << endl;
        cout << "2. Unfreeze Account" << endl;
        cout << "\nEnter choice (1-2): ";
        cin >> action;
        cin.ignore();

        if (action != 1 && action != 2)
        {
            cout << "Invalid choice!" << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, fullName, pass, pin;
        double balance;
        int location;
        bool found = false;
        string status = (action == 1) ? "FROZEN" : "ACTIVE";

        while (getline(file, line))
        {
            if (line.find(accNumber) == 0)
            {
                found = true;

                location = line.find(",");
                accNum = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                num = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                name = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                fullName = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                pass = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                pin = line.substr(0, location);
                line = line.substr(location + 1);

                location = line.find(",");
                if (location != string::npos)
                {
                    balance = stod(line.substr(0, location));
                }
                else
                {
                    balance = stod(line);
                }

                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << fullName << ","
                     << pass << ","
                     << pin << ","
                     << balance << ","
                     << status << endl;

                cout << "\nAccount " << accNumber << " has been " << (action == 1 ? "frozen" : "unfrozen") << "." << endl;
                setAuditLog(accNum, (action == 1 ? "ACCOUNT FROZEN" : "ACCOUNT UNFROZEN"), 0.00, 0.00);
            }
            else
            {
                temp << line << endl;
            }
        }

        if (!found)
        {
            cout << "Account not found!" << endl;
            file.close();
            temp.close();
            remove("temp.csv");
            return;
        }

        file.close();
        temp.close();
        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
    }

    void viewAuditLog()
    {
        ifstream file("SYSTEM_AUDIT_LOG.csv");
        string line;
        string logID, time, userID, username, action, beforeBalance, afterBalance;
        int location;

        cout << "\nAudit Log:" << endl;
        cout << setw(15) << "Log ID"
             << setw(25) << "Time"
             << setw(20) << "User ID"
             << setw(20) << "Username"
             << setw(30) << "Action"
             << setw(15) << "Before"
             << setw(15) << "After" << endl;
        cout << string(140, '-') << endl;

        while (getline(file, line))
        {
            location = line.find(",");
            logID = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            time = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            userID = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            username = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            action = line.substr(0, location);
            line = line.substr(location + 1);

            location = line.find(",");
            beforeBalance = line.substr(0, location);
            line = line.substr(location + 1);

            afterBalance = line;

            cout << setw(15) << logID
                 << setw(25) << time
                 << setw(20) << userID
                 << setw(20) << username
                 << setw(30) << action
                 << setw(15) << beforeBalance
                 << setw(15) << afterBalance << endl;
        }
        cout << string(140, '-') << endl;
        file.close();
    }
};

int main()
{
    int n;
    do
    {
        Signup signup;
        Signin signin;
        Admin admin;

        displaySign();
        cout << TEAL << "[1] " << RESET << "Create Account" << endl;
        cout << TEAL << "[2] " << RESET << "Login to Account" << endl;
        cout << TEAL << "[3] " << RESET << "Login as Admin" << endl;
        cout << "0. exit " << endl;
        cout << BOLD << "\nEnter your choice: " << RESET;
        cin >> n;
        cin.ignore();
        clearScreen();
        switch (n)
        {
        case 1:
        {
            signup.getInfo();
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;
        }
        case 2:
        {
            signin.signin();
            break;
        }
        case 3:
        {
            admin.adminMenu();
            break;
        }
        case 0:
        {
            cout << "Logging out..." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;
        }
        default:
        {
            cout << "Invalid choice. Please try again." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;
        }
        }
    } while (n != 0);

    return 0;
}