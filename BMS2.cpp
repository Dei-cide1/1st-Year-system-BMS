#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <random>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>

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
            return;
        }
        if (inputPin != pin)
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Invalid PIN. " << attempts << " attempts remaining." << endl;
            }
            else
            {
                cout << "Error: Maximum attempts reached. Please contact the administrator. " << endl;
                cout << " Automatically Logging out..." << endl
                     << endl;
                n = 0; // pag naging 0 to ibigsabihin maglologout kasi 0 ung condition ng logout dun sa user panel
                return;
            }
        }
        else
        {
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

        cout << "Enter your Full Name: ";
        getline(cin, fullName);

        do
        {
            if (checker)
            {
                cout << "Invalid phone number please try again" << endl;
            }

            cout << " Only digits " << endl;
            cout << " Ph number only (09) " << endl;
            cout << "No spaces, hyphens, or letters" << endl;
            cout << "Number :" << endl;
            cin >> number;
            checker = isValid.number(number);
        } while (!checker);

        cin.ignore();
        checker = false;

        do
        {
            if (checker)
            {
                cout << "Invalid username please try again" << endl;
            }
            cout << "Must contain: " << endl;
            cout << "At least one lowercase letter (a-z)" << endl;
            cout << "At least one uppercase letter (A-Z)" << endl;
            cout << "At least one digit (0-9)" << endl;
            cout << "Must not contain hyphens (-)" << endl;
            cout << "Minimum length: typically 6–12 characters" << endl;

            getUser(username);
            checker = isValid.username(username);
        } while (!checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "Invalid password please try again" << endl;
            }
            cout << "Must contain: " << endl;
            cout << "At least one lowercase letter (a-z)" << endl;
            cout << "At least one uppercase letter (A-Z)" << endl;
            cout << "At least one digit (0-9)" << endl;
            cout << "Minimum length: 8 characters" << endl;
            getPass(password);
            checker = isValid.password(password);
        } while (!checker);
        checker = false;
        do
        {
            string secondTry;
            if (checker)
            {
                cout << "Invalid pin please try again" << endl;
            }
            cout << "Must contain a 6 combination number" << endl;
            cout << "Enter your 6 digit PIN" << endl;
            cin >> pin;
            if (!isValid.pin(pin))
            {
                checker = !isValid.pin(pin);
            }
            else
            {
                cout << "Re-enter your 6 digit PIN" << endl;
                cin >> secondTry;
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

        cout << "Successfully created account!" << endl
             << endl;
        cout << "Full Name: " << fullName << endl;
        cout << "Username : " << username << endl;
        cout << "Account Number : " << accNumber << endl;

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
                cout << "Incorrect Username or Password" << endl;
            }
            cout << "(0 to return)" << endl;

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
                    cout << "Error: This account has been frozen." << endl;
                    cout << "Please contact the administrator at 09693381708 to unfreeze your account." << endl;
                    setAuditLog(c, "FAILED LOGIN - FROZEN ACCOUNT", 0.00, 0.00);
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
            }
        } while (!checker);
    }

    void user()
    {
        do
        {
            cout << "1. Check balance " << endl;
            cout << "2. Withdraw Cash " << endl;
            cout << "3. Deposit Cash " << endl;
            cout << "4. Transaction History " << endl;
            cout << "5. Change pin " << endl;
            cout << "0. Logout" << endl;
    cout << BOLD << "\nEnter your choice: " << RESET;
            cin >> n;
            cin.ignore();

            switch (n)
            {
            case 1:
                checkBalance();
                break;
            case 2:
                withdraw(accNumber);
                break;
            case 3:
                deposit(accNumber);
                break;
            case 4:
                viewTransaction();
                break;
            case 5:
                changePin(accNumber);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }

        } while (n != 0);
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
        cout << "\n==========================================" << endl;
        cout << "              TRANSACTION RECEIPT          " << endl;
        cout << "==========================================" << endl;
        cout << "Date & Time: " << getCurrentDateTime() << endl;
        cout << "Account Number: " << accNumber << endl;
        cout << "Full Name: " << getFromFile(accNumber, 4) << endl;
        cout << "Username: " << username << endl;
        cout << "Transaction Type: " << transactionType << endl;
        cout << "Amount: ₱" << fixed << setprecision(2) << amount << endl;
        cout << "Current Balance: ₱" << fixed << setprecision(2) << balance << endl;
        cout << "Transaction ID: " << transactionIdGenerate() << endl;
        cout << "==========================================" << endl;
        cout << "Thank you for banking with us!" << endl;
        cout << "==========================================\n"
             << endl;
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
            cout << "(Note that the bank only withdraw in denomination of 100, 200, 500 and 1000)" << endl;
            cout << "Choose amount to Withdraw(0 to return) : ";
            cin >> inputBalance;
            cin.ignore();
            if (inputBalance == 0)
            {
                return;
            }
            if (inputBalance < 0)
            {
                cout << "Error: Cannot withdraw negative amount!" << endl;
                repeater = false;
                continue;
            }
            if (0 != (static_cast<int>(inputBalance) % 100))
            {
                cout << "Error: Amount must be in denominations of 100, 200, 500, or 1000" << endl;
                repeater = false;
            }
            if (inputBalance > this->balance)
            {
                cout << "Error: Insufficient balance. Your current balance is ₱" << fixed << setprecision(2) << this->balance << endl;
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);
        if (n == 0)
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
                return;
            }
            if (inputBalance < 0)
            {
                cout << "Error: Cannot deposit negative amount!" << endl;
                repeater = false;
                continue;
            }
            if (inputBalance < 99)
            {
                cout << "Error : Minimum is 100" << endl;
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);

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
            if (!checker)
            {
                cout << "Incorrect Username or Password" << endl;
            }
            getUser(username);
            getPass(password);

            if (name == "0")
            {
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
        if (n == 0)
        {
            setAuditLog(ID, "FAILED PIN CHANGE - PIN VERIFICATION FAILED", 0.00, 0.00);
            return;
        }

        do
        {
            checker = false;

            cout << "Must contain a 6 combination number" << endl;
            cout << "Enter your New 6 digit PIN" << endl;
            getline(cin, newPin);

            if (!isValid.pin(newPin))
            {
                cout << "Invalid pin please try again" << endl;
                checker = !isValid.pin(newPin);
            }
            else
            {
                cout << "Re-enter your New 6 digit PIN" << endl;
                getline(cin, reEnter);
                if (newPin == reEnter)
                {
                    checker = !isValid.pin(newPin);
                }
                else
                {
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
            cout << "Invalid admin credentials!" << endl;
            return;
        }

        int choice;
        do
        {
            cout << "\n          ADMIN PANEL             " << endl;
            cout << "1. View All Customers             " << endl;
            cout << "2. Freeze/Unfreeze Account       " << endl;
            cout << "3. Audit Logs                    " << endl;
            cout << "0. Logout                        " << endl;
            cout << "\nEnter choice (0-3): ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                viewAllCustomers();
                break;
            case 2:
                freezeUnfreezeAccount();
                break;
            case 3:
                viewAuditLog();
                break;
            case 0:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
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
            return;

        cout << "1. Freeze Account" << endl;
        cout << "2. Unfreeze Account" << endl;
        cout << "Enter choice (1-2): ";
        cin >> action;
        cin.ignore();

        if (action != 1 && action != 2)
        {
            cout << "Invalid choice!" << endl;
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

                cout << "Account " << accNumber << " has been " << (action == 1 ? "frozen" : "unfrozen") << "." << endl;
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
             << setw(20) << "Action"
             << setw(15) << "Before"
             << setw(15) << "After" << endl;
        cout << string(130, '-') << endl;

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
                 << setw(20) << action
                 << setw(15) << beforeBalance
                 << setw(15) << afterBalance << endl;
        }
        cout << string(130, '-') << endl;
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
        switch (n)
        {
        case 1:
        {
            signup.getInfo();
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
        default:
        {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        }
    } while (n != 0);

    return 0;
}