#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "code.h"

using namespace std;

class ErrorDetectionAndCorrection {
public:
    virtual void inputData(const vector<int>& inputData) = 0;
    virtual void introduceErrors() = 0;
    virtual void displayResults() const = 0;
    virtual void detectAndCorrectErrors() = 0; // Pure virtual function
};


class User {
private:
    string username;
    string password;
    string email;
    string phoneNumber;

public:
    User(const string& user, const string& pass, const string& email, const string& phone)
        : username(user), password(pass), email(email), phoneNumber(phone) {}

    const string& getUsername() const { return username; }
    const string& getPassword() const { return password; }
    const string& getEmail() const { return email; }
    const string& getPhoneNumber() const { return phoneNumber; }
};

class UserManager {
private:
    vector<User> users;

public:
    void registerUser(const string& user, const string& pass, const string& email, const string& phone) {
        users.emplace_back(user, pass, email, phone);
        cout << BGRN << "Successfully Created Account!" << reset << endl;

        // Save user data to a file
        ofstream userFile("users.txt", ios::app);
        userFile << user << "," << pass << "," << email << "," << phone << endl;
        userFile.close();
    }

    bool loginUser(const string& user, const string& pass) {
        for (const auto& u : users) {
            if (u.getUsername() == user && u.getPassword() == pass) {
                return true;  // Login successful
            }
        }
        return false;  // Login failed
    }

    void displayAllUsers() const {
        if (users.empty()) {
            cout << BRED << "No registered users found." << reset<< endl;
            return;
        }

        cout << "Registered Users:" << endl;
        for (const auto& u : users) {
            cout << "Username: " << u.getUsername() << ", Email: " << u.getEmail() << ", Phone: " << u.getPhoneNumber() << endl;
        }
    }

    void loadUsersFromFile() {
        ifstream userFile("users.txt");
        string line;
        while (getline(userFile, line)) {
            stringstream ss(line);
            string user, pass, email, phone;
            getline(ss, user, ',');
            getline(ss, pass, ',');
            getline(ss, email, ',');
            getline(ss, phone, ',');
            users.emplace_back(user, pass, email, phone);
        }
        userFile.close();
    }

    const vector<User>& getUsers() const {  // Method to retrieve users
        return users;
    }
};

class Welcome : public UserManager {
private:
    string name, ph_no, email;
    UserManager userManager;
    bool isLoggedIn = false;  // Add a flag to track login status

public:
    Welcome() {
        userManager.loadUsersFromFile();  // Load users from file at startup
    }

    void login();
    void registration();
    void displayAllUsers() { userManager.displayAllUsers(); }  // Display users
    void setLoggedIn(bool status) { isLoggedIn = status; }  // Setter for login status
    bool getLoggedIn() const { return isLoggedIn; }  // Getter for login status
    void welcome_msg(const string& username) {
        cout << endl << endl << CYNB << "\t\t\tHey! " << username << endl;
        cout << "\t\t\tWelcome again to Error Detection and Correction\n" << reset << endl;
    }

    static int calculateParityBits(int dataBits);
};

void Welcome::registration() {
    while(true){
    try {  // Start of try block
        cout << BHYEL << "\t\t\t---Registration Page---" << reset << endl;
        cout << "\t\t\t*Enter your name: ";
        cin.ignore();  // Clear the buffer
        getline(cin, name);
        cout << "\t\t\t*Enter your phone no: ";
        cin >> ph_no;
        if (ph_no.size() != 10)
            throw runtime_error("Enter 10 digits Phone Number!");
        cout << "\t\t\t*Enter your valid email ID: ";
        cin >> email;
        if (email.find('@') == string::npos)  // Fixed email validation
            throw runtime_error("Enter valid Email ID!");

        string username, password;
        cout << "\t\t\t*Enter a username: ";
        cin >> username;
        cout << "\t\t\t*Enter a password: ";
        cin >> password;

        userManager.registerUser(username, password, email, ph_no);
        cout << "\t\t\tRedirecting to Login Page..." << endl;  // Notify user
        sleep(2);
        login();  // Automatically redirect to login
        break;
        
    } catch (const exception& e) {
        cout << BRED << "Exception: " << e.what() << reset << endl;
        
    }
    }
}


void Welcome::login() {
    string log_username, log_password;

    while (true) {  // Loop until successful login or user decides to exit
        cout << endl << endl << BHYEL << "\t\t\t---Login Page---" << reset << endl;
        cout << "\t\t\t*Username: ";
        cin >> log_username;
        cout << "\t\t\t*Password: ";
        cin >> log_password;

        char captcha[6];
        const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        static bool isSeeded = false; // Ensure srand is only called once
        if (!isSeeded) {
            srand(static_cast<unsigned int>(time(0)));
            isSeeded = true;
        }

        for (int i = 0; i < 5; i++) {
            captcha[i] = chars[rand() % (sizeof(chars) - 1)];
        }
        captcha[5] = '\0';

        cout << "\t\t\t*Captcha: " << captcha << endl;
        string user_input;
        cout << "\t\t\t*Enter captcha: ";
        cin >> user_input;

        if (user_input != captcha) {
            cout << BRED << "\t\t\tIncorrect Captcha! Please try again." << reset << endl;
            continue;  
        }

        if (userManager.loginUser(log_username, log_password)) {
            cout << BGRN << "\n\t\t\tLogin Successfully!!!" << reset << endl;
            welcome_msg(log_username);
            setLoggedIn(true);  // Set login status
            return;  // Exit the login loop
        } else {
            cout << BRED << "\t\t\tInvalid username or password! Please try again." << reset << endl;
        }

        int choice;
        cout << "\t\t\tWould you like to:\n";
        cout << "\t\t\t1. Try Login Again\n";
        cout << "\t\t\t2. Register\n";
        cout << "\t\t\t3. Exit\n";
        cout << "\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: continue;  // Restart the loop to try logging in again
            case 2: registration(); return;  // Register a new account
            case 3: cout << "\t\t\tExiting the program. Goodbye!" << endl; exit(0);
            default: cout << RED << "\t\t\tInvalid Option! Please try again." << reset << endl; break;
        }
    }
}

int Welcome::calculateParityBits(int dataBits) { 
    int parityBits = 0; 
    while ((1 << parityBits) < (dataBits + parityBits + 1)) { // Using bit shifting instead of pow 
     parityBits++; 
 
    } 
    return parityBits; 
} 
 
vector<int> insertParityBits(vector<int>& data, int parityBits) { 
    int totalBits = data.size() + parityBits; 
    vector<int> hammingCode(totalBits, 0); 
    int j = 0; 
    for (int i = 1; i <= totalBits; i++) { 
        if ((i & (i - 1)) != 0) {  // Non-power of 2 
            hammingCode[i - 1] = data[j++]; 
        } 
    } 
    return hammingCode; 
} 
 
void calculateParityBits(vector<int>& hammingCode, int parityBits) { 
    int totalBits = hammingCode.size(); 
    for (int i = 0; i < parityBits; i++) { 
        int parityPos = (1 << i) - 1;  // Position of parity bit 
        int parity = 0; 
        for (int j = 0; j < totalBits; j++) { 
            if ((j + 1) & (parityPos + 1)) { 
                parity ^= hammingCode[j]; 
            } 
        } 
        hammingCode[parityPos] = parity; 
    } 
} 
 
int detectError(const vector<int>& hammingCode, int parityBits) { 
    int totalBits = hammingCode.size(); 
    int errorPosition = 0; 
    for (int i = 0; i < parityBits; i++) { 
        int parityPos = (1 << i) - 1;  // Position of parity bit 
        int parity = 0; 
        for (int j = 0; j < totalBits; j++) { 
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
 
void printHammingCode(const vector<int>& hammingCode) { 
    for (int i : hammingCode) { 
        cout << i << "  "; 
    } 
    cout << endl; 
}
 
void introduceRandomError(vector<int>& hammingCode) { 
 
    srand(static_cast<unsigned int>(time(0))); 
    int pos = rand() % hammingCode.size(); 
    hammingCode[pos] ^= 1;  // Introduce error by flipping the bit 
} 

void displayMenu() {
    cout << MAGB << "\n\t\t\t---Error Detection and Correction---" << reset<< endl;
    sleep(1);
    cout << "\t\t\t1. Login" << endl;
    cout << "\t\t\t2. Register" << endl;
    cout << "\t\t\t3. Display All Users" << endl;
    cout << "\t\t\t4. Exit" << endl;
}


class detect {
public:
    virtual void crc(const vector<int>& data) = 0;
    virtual void checksum(const vector<int>& data) = 0;
};

class CRC : public detect {
public:
    
    vector<int> generateRandomGenerator(int length = 4) {
        vector<int> generator(length);
        srand(time(0));  // Initialize random number generator

        // Generate random values for the generator polynomial (1 or 0)
        for (int i = 0; i < length; i++) {
            generator[i] = rand() % 2;
        }

        return generator;
    }

    
    static vector<int> computeCRC(const vector<int>& data, const vector<int>& generator) {
        vector<int> crc(data);
        crc.resize(data.size() + generator.size() - 1, 0);

        for (size_t i = 0; i < data.size(); i++) {
            if (crc[i] == 1) {
                for (size_t j = 0; j < generator.size(); j++) {
                    crc[i + j] ^= generator[j];
                }
            }
        }

        vector<int> remainder(crc.end() - generator.size() + 1, crc.end());
        return remainder;
    }

    static bool verifyCRC(const vector<int>& data, const vector<int>& generator) {
        vector<int> remainder = computeCRC(data, generator);
        for (int bit : remainder) {
            if (bit != 0) return false;
        }
        return true;
    }

    void crc(const vector<int>& data) override {
        cout << " This will treat above input data as Receiver Side Data" <<endl << endl;
        vector<int> generator(4) ;// generateRandomGenerator();
        cout << "Enter Polynomial (at most 4 bits) eg. 1101 for 100100: ";
        for (int i=0; i<4; i++){
        cin >> generator[i];
        }
        
        sleep(1);
        cout << "Polynomial: ";
        for (int bit : generator) {
            cout << bit;
        }
        cout << endl;


        vector<int> remainder = computeCRC(data, generator);
        cout << "CRC Remainder: ";
        for (int bit : remainder) {
            cout << bit;
        }
        cout << endl;
        
        sleep(2);
        bool valid = verifyCRC(data, generator);
        cout << "CRC verification: " << (valid ? "Valid" : "Invalid") << endl;
    }

    void checksum(const vector<int>& data) override {
        // Not implemented for CRC class
    }
};

class Checksum : public detect {
public:
    static int calculateChecksum(const vector<int>& data) {
        int sum = 0;
        for (int num : data) {
            sum += num;
        }
        return ~sum + 1; // One's complement
    }

    static bool verifyChecksum(const vector<int>& data, int receivedChecksum) {
        return calculateChecksum(data) == receivedChecksum;
    }

    void checksum(const vector<int>& data) override {
        int checksum = calculateChecksum(data);
        cout << "Calculated Checksum: " << checksum << endl;
        
        sleep(2);
        // Verify checksum using the calculated value
        bool valid = verifyChecksum(data, checksum);
        cout << "Checksum verification: " << (valid ? "Valid" : "Invalid") << endl;
    }

    void crc(const vector<int>& data) override {
        // Not implemented for Checksum class
    }
};



void dataInput(){
    int dataBits; 
        cout << "\t\t\tEnter the number of data bits: "; 
        cin >> dataBits; 
 
        vector<int> data(dataBits); 
        cout << "\t\t\tEnter the data bits separated by space: "; 
        for (int i = 0; i < dataBits; i++) { 
            cin >> data[i]; 
        } 
 
        int parityBits = Welcome::calculateParityBits(dataBits); 
        cout << "\t\t\tNumber of Parity bits needed: " << parityBits << endl; 
 
        vector<int> hammingCode = insertParityBits(data, parityBits); 
        calculateParityBits(hammingCode, parityBits); 
        
        sleep(1);
        cout << "\t\t\tHamming code with parity bits: "; 
        printHammingCode(hammingCode); 
        
        // fstream inputFile;
        // inputFile.open("input.txt", ios::app);
        // inputFile << hammingCode <<endl;
        // inputFile.close();
 
        int errorPosition = detectError(hammingCode, parityBits); 
        if (errorPosition == 0) { 
            cout << "\t\t\tNo errors detected in the Hamming code." << endl; 
            
            sleep(2);
            int choice;
            cout << "\t\t\tPress 1 to detect Error using other methods\n\t\t\t0 to nothing: ";
            cin >> choice;
            if(choice == 1){
                detect* crcObj = new CRC();       // Pointer to CRC object
                detect* checksumObj = new Checksum(); // Pointer to Checksum object
                
                cout << "Performing CRC operations:" << endl;
                sleep(2);
                crcObj->crc(data); // Pass data to CRC method
                
                cout << "Performing Checksum operations:" << endl;
                sleep(2);
                checksumObj->checksum(data); // Pass data to checksum method
                
                delete crcObj;
                delete checksumObj;
            }
            // else if( choice == 0 ){
            //   continue;
            // }
            
        } else { 
            cout << "\t\t\tError detected at position: " << errorPosition << endl; 
            hammingCode[errorPosition - 1] ^= 1;  // Correct the error 
            sleep(2);
            cout << "\t\t\tCorrected Hamming Code: "; 
            printHammingCode(hammingCode); 
        } 
 
        int option; 
        cout << "\t\t\tIntroduce a random error or have receiver's data? \n\t\t\t(1 for random error, 2 for receiver's data, 0 to exit): "; 
        cin >> option; 
 
        if (option == 1) { 
            introduceRandomError(hammingCode); 
            cout << "\t\t\tHamming code after introducing a random error: "; 
            printHammingCode(hammingCode); 
            
            sleep(1);
            errorPosition = detectError(hammingCode, parityBits); 
            if (errorPosition == 0) { 
                cout << "\t\t\tNo errors detected in the Hamming code." << endl; 
                
                int choice;
                cout << "\t\t\tPress 1 to detect Error using other methods: ";
                cin >> choice;
                if(choice == 1){
                    detect* crcObj = new CRC();       // Pointer to CRC object
                    detect* checksumObj = new Checksum(); // Pointer to Checksum object
                
                    cout << "Performing CRC operations:" << endl;
                    sleep(2);
                    crcObj->crc(data); // Pass data to CRC method
                
                    cout << "Performing Checksum operations:" << endl;
                    sleep(2);
                    checksumObj->checksum(data); // Pass data to checksum method
                
                    delete crcObj;
                    delete checksumObj;
                }
                
            } else { 
                cout << "\t\t\tError detected at position: " << errorPosition << endl; 
                hammingCode[errorPosition - 1] ^= 1;  // Correct the error 
                sleep(2);
                cout << "\t\t\tCorrected Hamming Code: "; 
                printHammingCode(hammingCode); 
            } 
        } else if (option == 2) { 
            vector<int> receivedHammingCode(hammingCode.size()); 
            cout << "\t\t\tEnter the received Hamming Code: "; 
            for (int i = 0; i < receivedHammingCode.size(); i++) { 
                cin >> receivedHammingCode[i]; 
            } 
 
            errorPosition = detectError(receivedHammingCode, parityBits); 
            if (errorPosition == 0) { 
                cout << "\t\t\tNo errors detected in the Hamming code." << endl; 
                
                int choice;
                cout << "\t\t\tPress 1 to detect Error using other methods: ";
                cin >> choice;
                if(choice == 1){
                    detect* crcObj = new CRC();       // Pointer to CRC object
                    detect* checksumObj = new Checksum(); // Pointer to Checksum object
                
                    cout << "Performing CRC operations:" << endl;
                    sleep(2);
                    crcObj->crc(receivedHammingCode); // Pass receivedHammingCode to CRC method
                
                    cout << "Performing Checksum operations:" << endl;
                    sleep(2);
                    checksumObj->checksum(receivedHammingCode); // Pass receivedHammingCode to checksum method
                
                    delete crcObj;
                    delete checksumObj;
                }
                
            } else { 
                cout << "\t\t\tError detected at position: " << errorPosition << endl; 
                receivedHammingCode[errorPosition - 1] ^= 1;  // Correct the error 
                sleep(2);
                cout << "\t\t\tCorrected Hamming Code: "; 
                printHammingCode(receivedHammingCode); 
            } 
        } //else if (option == 0) { 
        //     cout << "\t\t\tExiting the program. Goodbye!" << endl; 
        //     break; 
        // } 
        
}


int main() {
    Welcome user;
    int choice;

    do {
        displayMenu();
        sleep(1);
        cout << "\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: user.login(); break;  // Login
            case 2: user.registration(); break;  // Registration
            case 3: user.displayAllUsers();  continue;
            case 4: cout << "\t\t\tExiting the program. Goodbye!" << endl; return 0;
            default: cout << "\t\t\tInvalid Option! Please try again." << endl; break;
        }
        
        
        do{
        int back, cont, exi, num;
        cout << "\t\t\t1. Back\n\t\t\t2.Continue\n\t\t\t3.Exit";
        sleep(1);
        cout << "\n\t\t\tEnter your choice: ";
        cin >> num;
        if(num == 1){
            break;
        }
        switch (num) {
            case 1: break; 
            case 2: 
                dataInput();
                break;
            case 3: cout << "\t\t\tExiting the program. Goodbye!" << endl; return 0;
            default: cout << "\t\t\tInvalid Option! Please try again." << endl; break;
        }
        } while (true);
 
    } while (true); 
 
    return 0; 
}
