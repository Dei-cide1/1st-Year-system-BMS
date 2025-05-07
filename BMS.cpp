#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Signup
{
    ofstream accounts;
    string username,password;
    long int number;

    void userSignup(){

    accounts.open("userAccounts.csv", ios::app);

    cout<<"Number :"<<endl;
    cin>>number;
    cin.ignore();

    cout<<"Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen."<<endl;
    cout<<" Username :"<<endl;
    getline(cin, username);

    cout<<"Password should be at least 15 characters OR at least 8 characters including a number and a lowercase letter."<<endl;
    cout<<" Password :"<<endl;
    getline(cin, password);

    accounts<<number<<","<<username<<","<<password<<endl;

    accounts.close();
    
    username = username + "TH.csv";

    accounts.open(username);


    accounts.close();
    }

};

struct Signin
{
    

    
};


int main(){
    Signup signup;
    int n;

    cout<<"WELCOME TO DAGGZ!"<<endl<<endl;
    cout<<"1. Sign in "<<endl;
    cout<<"2. Sign up "<<endl;
    cout<<"3. Sign in as admin "<<endl;
    cout<<"4. exit "<<endl;
    cout<<"Enter choice : ";
    cin>>n;

    switch (n)
    {
    case 1:
        
        break;
    
    case 2:
        signup.userSignup();
        break;

    default:
        break;
    }

    return 0;
}