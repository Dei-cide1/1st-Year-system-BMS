#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <random>
#include <vector>
#include <iomanip>
#include <sstream>

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

void pinVerification(string &pin, int &n)
{
    string inputPin;
    int attempts = {5};

    do
    {
        cout << "Enter your 6-digit PIN: ";
        getline(cin, inputPin);

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
            }
        }
        else
        {
            break;
        }
    } while (attempts > 0);
}
string getFromFile(string accNumber, int n)
{
    ifstream file("userAccounts.csv");
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
    return accNum;
}
void editBalance(string &filename, string accNumber)
{
    ifstream file(filename);
    ofstream temp("temp.csv");
    ostringstream toString;
    vector<string> fLine;
    string line, accNum, num, name, pass, pin;
    int balance;
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

        toString << fixed << setprecision(2);
        toString << line;
        balance = stod(line);

        if (accNumber == accNum)
        {
            toString << fixed << setprecision(2);
            toString << accNum << "," << num << "," << name << "," << pass << "," << pin << "," << balance << endl;
            temp << toString.str();
        }
    }

    file.close();
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
        ifstream file("userAccounts.csv");
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

        accounts.open("userAccounts.csv", ios::app);

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

        toString << accNumber << "," << number << "," << username << "," << password << "," << pin << "," << balance << endl;
        accounts << toString.str();

        string newFile = accNumber + ".csv";
        ofstream newfile(newFile);

        newfile.close();

        cout << "Successfully created account!" << endl
             << endl;
        cout << "Username : " << username << endl;
        cout << "Account Number : " << accNumber << endl;
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
            getUser(name);
            getPass(pass);

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
            cout << "4.Transaction History " << endl;
            cout << "5. Change pin " << endl;
            cout << "Enter choice : ";
            cin >> n;

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
            default:
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
        ifstream file("userAccounts.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, pass, pin;
        string inputPin;
        int attempts = 5;
        double balance;
        int location;
        double newBalance;
        bool repeater;
        bool found = false;

        // dito mag rerepeat input pag mali ung ini input mo dito din nag tatake ng input
        do
        {
            repeater = true;
            cout << "(Note that the bank only withdraw in denomination of 100, 200, 500 and 1000)" << endl;
            cout << "Choose amount to Withdraw : ";
            cin >> newBalance;
            cin.ignore();
            if (0 != (static_cast<int>(newBalance) % 100))
            {
                cout << "Error: Amount must be in denominations of 100, 200, 500, or 1000" << endl;
                repeater = false;
            }
            if (newBalance > this->balance)
            {
                cout << "Error: Insufficient balance. Your current balance is ₱" << fixed << setprecision(2) << this->balance << endl;
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);

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
                temp << accNum << "," << num << "," << name << "," << pass << "," << pin << "," << balance - newBalance << endl;
                found = true;
            }
            else
            {
                temp << accNum << "," << num << "," << name << "," << pass << "," << pin << "," << balance << endl;
            }
        }

        temp.close();
        file.close();

        remove("userAccounts.csv");
        rename("temp.csv", "userAccounts.csv");
    }

    void deposit(string ID)
    {
        ifstream file("userAccounts.csv");
        ofstream temp("temp.csv");
        string line, accNum, num, name, pass, pin;
        string inputPin;
        int attempts = 5;
        double balance;
        int location;
        double newBalance;
        bool repeater;
        bool found = false;

        do
        {
            repeater = true;
            cout << "(Note that the bank only Deposit in denomination of 100, 200, 500 and 1000)" << endl;
            cout << "Choose amount to Deposit : ";
            cin >> newBalance;
            cin.ignore();
            if (newBalance < 99)
            {
                cout << "Error : Minimum is 100" << endl;
                repeater = false;
            }
        } while (!repeater);

        pinVerification(this->pin, n);

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
                temp << accNum << "," << num << "," << name << "," << pass << "," << pin << "," << balance + newBalance << endl;
                found = true;
            }
            else
            {
                temp << accNum << "," << num << "," << name << "," << pass << "," << pin << "," << balance << endl;
            }
        }

        temp.close();
        file.close();

        remove("userAccounts.csv");
        rename("temp.csv", "userAccounts.csv");
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