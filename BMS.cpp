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
    string line, accNum = {""}, num, name, pass, pin;
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
        pass = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        location = line.find(",");
        pin = line.substr(0, location);
        line = line.substr(location + 1, line.length());

        balance = stod(line);

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
                return pass;
                break;
            case 5:
                return pin;
                break;
            case 6:
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
             << beforeBalance << ","
             << afterBalance << endl;

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
        string line, accNum = {"0000-0000-0000"}, num, name, pass, pin;
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
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            balance = stod(line);

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
    string accNumber, number, username, password, pin;
    double balance = 0.00;
    bool checker = false;

    void getInfo()
    {

        accounts.open("SYSTEM_USERS.csv", ios::app);

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
                 << password << ","
                 << pin << ","
                 << balance << endl;
        accounts << toString.str();

        string newFile = accNumber + ".csv";
        ofstream newfile(newFile);

        newfile.close();

        cout << "Successfully created account!" << endl
             << endl;
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
// note pag nagwiwithdraw tas naubos na ung attemp naproprocess padin ung transaction
struct Signin
{
    Authenticate authenticate;

    string accNumber, number, username, password, pin, c;
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
                accNumber = getFromFile(c, 1);
                number = getFromFile(c, 2);
                username = getFromFile(c, 3);
                password = getFromFile(c, 4);
                pin = getFromFile(c, 5);
                balance = stod(getFromFile(c, 6));
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
            cout << "Enter choice : ";
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
        double balance = stod(getFromFile(accNumber, 6));
        cout << " CURRENT BALANCE : ₱" << fixed << setprecision(2) << balance << endl;
    }

    void withdraw(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, pass, pin;
        string inputPin;
        int attempts = 5;
        double balance;
        int location;
        double inputBalance;
        bool repeater;
        bool found = false;

        // dito mag rerepeat input pag mali ung ini input mo dito din nag tatake ng input
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
            return;

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
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            balance = stod(line);

            if (accNum == ID && !found)
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << pin << ","
                     << balance - inputBalance << endl;
                found = true;
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << pin << ","
                     << balance << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->balance = stod(getFromFile(c, 6));

        toHistory(inputBalance, "WITHDRAW");
    }

    void deposit(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, pass, pin;
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
            cout << "(Note that the bank only Deposit in denomination of 100, 200, 500 and 1000)" << endl;
            cout << "Choose amount to Deposit(0 to return) : ";
            cin >> inputBalance;
            cin.ignore();
            if (inputBalance == 0)
            {
                return;
            }
            if (inputBalance < 99)
            {
                cout << "Error : Minimum is 100" << endl;
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);
        if (n == 0)
            return;

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
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            balance = stod(line);

            if (accNum == ID && !found)
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << pin << ","
                     << balance + inputBalance << endl;
                found = true;
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << pin << ","
                     << balance << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->balance = stod(getFromFile(c, 6));

        toHistory(inputBalance, "DEPOSIT");
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
    }

    void changePin(string ID)
    {
        ifstream file("SYSTEM_USERS.csv");
        ofstream temp("temp.csv");
        Isvalid isValid;
        string line, accNum, num, name, pass, pin;
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
            return;

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
            pass = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            location = line.find(",");
            pin = line.substr(0, location);
            line = line.substr(location + 1, line.length());

            balance = stod(line);

            if (accNum == ID && !found)
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << reEnter << ","
                     << balance << endl;
                found = true;
            }
            else
            {
                temp << accNum << ","
                     << num << ","
                     << name << ","
                     << pass << ","
                     << pin << ","
                     << balance << endl;
            }
        }

        temp.close();
        file.close();

        remove("SYSTEM_USERS.csv");
        rename("temp.csv", "SYSTEM_USERS.csv");
        this->pin = newPin;

        toHistory(newBal, "CHANGE PIN");
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
int main()
{
    int n;
    do
    {
        Signup signup;
        Signin signin;

        displaySign();
        cout << "1. Create account " << endl;
        cout << "2. Check account " << endl;
        cout << "3. Login as Admin " << endl;
        cout << "4. exit " << endl;
        cout << "Enter choice : ";
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

            break;
        }
        default:
        {
            break;
        }
        }
    } while (n != 0);

    return 0;
}