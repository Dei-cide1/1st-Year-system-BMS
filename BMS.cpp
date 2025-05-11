#include <iostream>
#include <fstream>
#include <string>

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

struct Signup
{
    ofstream accounts;
    string username, password;
    long int number;

    void userSignup()
    {

        accounts.open("userAccounts.csv", ios::app);

        cout << "Number :" << endl;
        cin >> number;
        cin.ignore();

        cout << "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen." << endl;
        getUser(username);

        cout << "Password should be at least 15 characters OR at least 8 characters including a number and a lowercase letter." << endl;
        getPass(password);

        accounts << number << "," << username << "," << password << endl;

        accounts.open(username);

        accounts.close();
    }
};

struct Signin
{

    void userSignin()
    {
        cout << "Welcome to DAGGZ!" << endl
             << endl;
    }

    bool authUser(string user)
    {
        ifstream iAccount("userAccounts.csv", ios ::in);
        string lineAcc, first = "", sec = "";
        int location;
        bool isFound = true;

        while (iAccount >> lineAcc)
        {

            location = lineAcc.find(",");
            first = lineAcc.substr(0, location);
            lineAcc = lineAcc.substr(location + 1, lineAcc.length());

            location = lineAcc.find(",");
            sec = lineAcc.substr(0, location);

            if (user == sec)
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
        string lineAcc, first = "", sec = "";
        int location;
        bool isFound = true;

        while (iAccount >> lineAcc)
        {

            location = lineAcc.find(",");
            first = lineAcc.substr(0, location);
            lineAcc = lineAcc.substr(location + 1, lineAcc.length());

            location = lineAcc.find(",");
            sec = lineAcc.substr(0, location);
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

    cout << "WELCOME TO DAGGZ!" << endl
         << endl;
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

        do
        {
            cout << checker;
            if (checker)
            {
                cout << "Invalid Username please try again" << endl;
            }
            getUser(user);
            checker = signin.authUser(user);
        } while (checker);
        checker = false;
        do
        {
            if (checker)
            {
                cout << "Invalid Password please try again" << endl;
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
    default:
    {
        break;
    }
    }

    return 0;
}