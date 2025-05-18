#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void updateCustomerBalance(const string& filename, const string& username, double newBalance) {
    ifstream inFile(filename);
    ofstream tempFile("temp.csv");
    string line;

    bool found = false;

    while (getline(inFile, line)) {
        size_t location;
        string firstC, secC, thirdC;

        location = line.find(",");
        firstC = line.substr(0, location);
        line = line.substr(location + 1);

        location = line.find(",");
        secC = line.substr(0, location);
        line = line.substr(location + 1);

        location = line.find(",");
        thirdC = line.substr(0, location);

        if (secC == username) {
            found = true;
            tempFile << firstC << "," << secC << "," << newBalance << "," << line.substr(location + 1) << "\n";
        } else {
            tempFile << firstC << "," << secC << "," << thirdC << "," << line.substr(location + 1) << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.csv", filename.c_str());

    if (!found) {
        cout << "User not found!" << endl;
    } else {
        cout << "Balance updated successfully!" << endl;
    }
}

int main() {
    updateCustomerBalance("userAccounts.csv", "john_doe", 750.00);
    return 0;
}