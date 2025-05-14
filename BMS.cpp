#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <random>

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



struct Signup
{
    ofstream accounts;
    string accNumber, number, username, password, pin;
    bool checker = false;

    void userSignup()
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
            checker = isValidN(number);
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
            checker = isValidU(username);
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
            checker = isValidP(password);
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
            if (!isValidPin(pin))
            {
                checker = !isValidPin(pin);
            }else{
            cout << "Re-enter your 6 digit PIN" << endl;
            cin >> secondTry;
            if (pin == secondTry)
            {
                checker = !isValidPin(pin);
            }
            else
            {
                checker = true;
            }
        }
        } while (checker);

        accNumber = accountNum();

        accounts << accNumber << "," << number << "," << username << "," << password << "," << pin << endl;

        accounts.open(username);

        accounts.close();
    }

    bool isValidN(string number)
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

    bool isValidU(string name)
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

    bool isValidP(string pass)
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
    bool isValidPin(string pin)
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



    string accountNum()
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
    string lineAcc, firstC = "", secC = "";
    int location;
    bool isFound = true;

    bool authUser(string user)
    {
        ifstream iAccount("userAccounts.csv", ios ::in);

        while (iAccount >> lineAcc)
        {

            location = lineAcc.find(",");
            firstC = lineAcc.substr(0, location);
            lineAcc = lineAcc.substr(location + 1, lineAcc.length());

            location = lineAcc.find(",");
            secC = lineAcc.substr(0, location);

            if (user == secC)
            {
                isFound = false;
            }
        }
        iAccount.close();

        return isFound;
    }
    bool authPass(string user)
    {
        ifstream iAccount("userAccounts.csv", ios ::in);

        while (iAccount >> lineAcc)
        {

            location = lineAcc.find(",");
            firstC = lineAcc.substr(0, location);
            lineAcc = lineAcc.substr(location + 1, lineAcc.length());

            location = lineAcc.find(",");
            secC = lineAcc.substr(0, location);
            lineAcc = lineAcc.substr(location + 1, lineAcc.length());

            if (user == lineAcc)
            {
                isFound = false;
            }
        }
        iAccount.close();

        return isFound;
    }
};

int main()
{
    Signup signup;
    int n;

    displaySign();
    cout << "1. Create account " << endl;
    cout << "2. Check account " << endl;
    cout << "3. Login as Admin " << endl;
    cout << "4. exit " << endl;
    cout << "Enter choice : ";
    cin >> n;

    switch (n)
    {
    case 1:
    {

        signup.userSignup();
        cout << "Successfully created account!" << endl;

        break;
    }
    case 2:
    {
        Signin signin;
        bool checker = false;
        string user = {""}, pass = {""};

        displaySign();
        do
        {
            cout << checker;
            if (checker)
            {
                cout << "Incorrect Username please try again" << endl;
            }
            getUser(user);
            checker = signin.authUser(user);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "Incorrect Password please try again" << endl;
            }
            getPass(pass);
            checker = signin.authPass(pass);
        } while (checker);

        

        break;
    }
    case 3:
    {
        Signin signin;
        bool checker = false;
        string user = {""}, pass = {""};

        displaySign();
        do
        {
            cout << checker;
            if (checker)
            {
                cout << "Incorrect Username please try again" << endl;
            }
            getUser(user);
            checker = signin.authUser(user);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "Incorrect Password please try again" << endl;
            }
            getPass(pass);
            checker = signin.authPass(pass);
        } while (checker);

        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}