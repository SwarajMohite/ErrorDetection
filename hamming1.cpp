#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>

using namespace std;

class Welcome {
    fstream temp;
    char name[80], ph_no[10], email[20];
    char username[20], password[20];
    char log_username[20], log_password[20];

public:
    void login();
    void registration();
    void welcome_msg() {
        cout << endl << endl << " Hey! " << username << endl;
        cout << endl << "Welcome again in Error Detection and Correction\n\n" << endl;
    }
};

void Welcome::registration() {
    cout << endl << "Registration Page" << endl;
    cout << endl << "Enter your name: ";
    cin >> name;
    cout << endl << "Enter your phone no: ";
    cin >> ph_no;
    cout << endl << "Enter your valid email ID: ";
    cin >> email;

    cout << endl << "Enter a username: ";
    cin >> username;
    cout << endl << "Enter a password: ";
    cin >> password;

    temp.open("users.txt", ios::out | ios::app);
    if (!temp) {
        cout << "Error! Unable to open file";
        exit(1);
    }
    temp << username << " " << password << endl;
    cout << "Successfully Created Account!" << endl;
    temp.close();
}

void Welcome::login() {
    cout << endl << "Login Page" << endl << endl;
    cout << endl << "Username: ";
    cin >> log_username;
    cout << endl << "Password: ";
    cin >> log_password;

    char captcha[6];
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < 5; i++) {
        captcha[i] = chars[rand() % (sizeof(chars) - 1)];
    }
    captcha[5] = '\0';

    cout << endl << "Captcha: " << captcha << endl;
    char user_input[6];
    cout << "Enter captcha: ";
    cin >> user_input;

    if (strcmp(captcha, user_input) != 0) {
        cout << "Incorrect Captcha!" << endl;
        return;
    }

    temp.open("users.txt", ios::in);
    if (!temp) {
        cout << endl << "Error! Unable to open file";
        exit(1);
    }

    while (temp >> username >> password) {
        if (strcmp(log_username, username) == 0) {
            if (strcmp(log_password, password) == 0) {
                cout << endl << "Login Successfully!!!" << endl;
                temp.close();
                welcome_msg();
                return;
            } else {
                cout << endl << "Incorrect Password!" << endl;
                temp.close();
                return;
            }
        }
    }
    cout << endl << "Username not found!" << endl;
    temp.close();
}

int calculateParityBits(int dataBits) {
    int parityBits = 0;
    while (pow(2, parityBits) < (dataBits + parityBits + 1)) {
        parityBits++;
    }
    return parityBits;
}

vector<int> insertParityBits(vector<int>& data, int parityBits) {
    int totalBits = data.size() + parityBits;
    vector<int> hammingCode(totalBits, 0);

    int j = 0;
    for (int i = 1; i <= totalBits; i++) {
        if ((i & (i - 1)) != 0) {
            hammingCode[i - 1] = data[j];
            j++;
        }
    }
    return hammingCode;
}

void calculateParityBits(vector<int>& hammingCode, int parityBits) {
    int totalBits = hammingCode.size();

    for (int i = 0; i < parityBits; i++) {
        int parityPos = pow(2, i) - 1;
        int parity = 0;

        for (int j = parityPos; j < totalBits; j++) {
            if ((j + 1) & (parityPos + 1)) {
                parity ^= hammingCode[j];
            }
        }

        hammingCode[parityPos] = parity;
    }
}

int detectError(vector<int>& hammingCode, int parityBits) {
    int totalBits = hammingCode.size();
    int errorPosition = 0;

    for (int i = 0; i < parityBits; i++) {
        int parityPos = pow(2, i) - 1;
        int parity = 0;

        for (int j = parityPos; j < totalBits; j++) {
            if ((j + 1) & (parityPos + 1)) {
                parity ^= hammingCode[j];
            }
        }

        if (parity != 0) {
            errorPosition += parityPos + 1;
        }
    }
    return errorPosition;
}

void printHammingCode(vector<int>& hammingCode) {
    for (int i = 0; i < hammingCode.size(); i++) {
        cout << hammingCode[i] << "  ";
    }
    cout << endl;
}

void introduceRandomErrors(vector<int>& hammingCode, int errorCount) {
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < errorCount; i++) {
        int pos = rand() % hammingCode.size();
        hammingCode[pos] ^= 1;
    }
}

int main() {
    Welcome user;
    int choice;

    cout << endl << "Error Detection and Correction" << endl;

welcome:
    cout << endl << "Welcome" << endl;
    cout << endl << "1. Login" << endl;
    cout << endl << "2. Register" << endl;
    cout << endl << "Enter your choice: " << endl;
    cin >> choice;

    switch (choice) {
        case 1:
            user.login();
            break;
        case 2:
            user.registration();
            goto welcome;
        default:
            cout << endl<<"Invalid Option! Please try again." << endl;
            goto welcome;
    }

    int dataBits;
    cout << endl << "Enter the number of data bits: ";
    cin >> dataBits;

    vector<int> data(dataBits);
    cout << endl <<"Enter the data bits separated by space: ";
    for (int i = 0; i < dataBits; i++) {
        cin >> data[i];
    }

    int parityBits = calculateParityBits(dataBits);
    cout <<endl << "Number of Parity bits needed: " << parityBits << endl;

    vector<int> hammingCode = insertParityBits(data, parityBits);
    calculateParityBits(hammingCode, parityBits);

    cout << endl <<"Hamming code with parity bits: ";
    printHammingCode(hammingCode);

    int errorPosition = detectError(hammingCode, parityBits);
    if (errorPosition == 0) {
        cout << endl <<"No errors detected in the Hamming code." << endl;
    } else {
        cout << endl <<"Error detected at position: " << errorPosition << endl;
        hammingCode[errorPosition - 1] ^= 1;  // Correct the error
        cout << endl <<"Corrected Hamming Code: ";
        printHammingCode(hammingCode);
    }

    int option;
    cout << endl <<endl <<"Do you want to introduce random errors \nor\n Do you have data from the receiver's side? \n(1 for random errors, 2 for receiver's data, 0 for No option): ";
    cin >> option;

    if (option == 1) {
        int errorCount;
        cout <<endl << "Enter the number of random errors to introduce: ";
        cin >> errorCount;
        introduceRandomErrors(hammingCode, errorCount);
        cout << endl <<"Hamming code after introducing random errors: ";
        printHammingCode(hammingCode);

        errorPosition = detectError(hammingCode, parityBits);
        if (errorPosition == 0) {
            cout << endl <<"No errors detected in the Hamming code." << endl;
        } else {
            cout << endl <<"Error detected at position: " << errorPosition << endl;
            hammingCode[errorPosition - 1] ^= 1;  // Correct the error
            cout << "Corrected Hamming Code: ";
            printHammingCode(hammingCode);
        }
    } else if (option == 2) {
        vector<int> receivedHammingCode(hammingCode.size());
        cout << endl <<"Enter the received Hamming Code: ";
        for (int i = 0; i < receivedHammingCode.size(); i++) {
            cin >> receivedHammingCode[i];
        }

        errorPosition = detectError(receivedHammingCode, parityBits);
        if (errorPosition == 0) {
            cout <<endl << "No errors detected in the Hamming code." << endl;
        } else {
            cout <<endl <<endl << "Error detected at position: " << errorPosition << endl;
            receivedHammingCode[errorPosition - 1] ^= 1;  // Correct the error
            cout << endl <<"Corrected Hamming Code: ";
            printHammingCode(receivedHammingCode);
        }
    } else if (option == 0) {
        exit(0);
    }

    return 0;
}
