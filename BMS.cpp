#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

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
    string username, password, number;
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

            cout << " Enter your 11 digit phone number starting at 09 " << endl;
            cout << "Number :" << endl;
            cin >> number;
            checker = isValidN(number);
        } while (checker);

        cin.ignore();
        checker = false;

        do
        {
            if (checker)
            {
                cout << "Invalid phone number please try again" << endl;
            }
            cout << "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen." << endl;
            getUser(username);
            checker = isValidU(username);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "Invalid phone number please try again" << endl;
            }
            cout << "Password should be at least 15 characters OR at least 8 characters including a number and a lowercase letter." << endl;
            getPass(password);
            checker = isValidP(password);
        } while (checker);

        accounts << number << "," << username << "," << password << endl;

        accounts.open(username);

        accounts.close();
    }

    bool isValidN(string number)
    {
        if (number.length() != 11)
        {
            return true;
        }

        for (int i = 0; i < number.length(); i++)
        {
            if (!isdigit(number[i]))
            {
                return true;
            }
        }

        if (number[0] != '0' || number[1] != '9')
        {
            return true;
        }
        return false;
    }

    bool isValidU(string name)
    {
        if (name.length() < 4 || name.length() > 24)
        {
            return true;
        }
        if (name[0] == '-' || name.back() == '-')
        {
            return true;
        }
        for (int i = 0; i < name.length(); i++)
        {
            if (!isalnum(name[i]) || name[i] == '-')
            {
                return true;
            }
        }
        return false;
    }

    bool isValidP(string pass)
    {
        if (password.length() < 8)
            return true;
        bool hasLower = true, hasNumber = true;
        for (int i = 0; i < pass.length(); i++)
        {
            if (islower(pass[i]))
            {
                hasLower = false;
            }
            if (isdigit(pass[i]))
            {
                hasNumber = false;
            }
        }
        return (hasLower || hasNumber);
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
    cout << "1. Sign in " << endl;
    cout << "2. Sign up " << endl;
    cout << "3. Sign in as admin " << endl;
    cout << "4. exit " << endl;
    cout << "Enter choice : ";
    cin >> n;

    switch (n)
    {
    case 1:
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
    case 2:
    {
        signup.userSignup();
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