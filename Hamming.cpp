#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread> // for std::this_thread::sleep_for
#include <chrono>
#include <string> // for std::chrono::milliseconds
using namespace std;

class HammingCode {
    int data[10];        // Array for data bits
    int parity[10];      // Array for parity bits
    int codeword[20];    // Array for codeword (data + parity)
    int error[20];       // Array for codeword with error
    int m, p;            // m = number of data bits, p = number of parity bits

public:
    void inputData();
    void Graphics(int data[], int m);
    void calculateParityBits();
    void generateCodeword();
    void introduceError();
    void displayCodeword();
    void displayReceivedCodeword();
    void detectAndCorrectError();
};

void HammingCode::inputData() {
    cout << "Enter the number of data bits (max 10): ";
    cin >> m;

    // Ensure the number of data bits is within the limit
    if (m > 10) {
        cout << "Data bits limit exceeded. Please enter up to 10 bits only." << endl;
        exit(0);
    }

    for (int i = 0; i < m; ++i) {
        bool validInput = false;
        while (!validInput) {
            cout << "Enter " << (i + 1) << "-bit data: ";
            cin >> data[i];
            if (data[i] != 0 && data[i] != 1) {
                cout << "Error: Please enter binary data (only 0s and 1s)." << endl;
            } else {
                validInput = true;
            }
        }
    }

    // Calculate the number of parity bits required
    p = 0;
    while ((1 << p) < (m + p + 1)) {
        p++;
    }


    Graphics(data, m); // Pass the entire array and number of data bits to the Graphics function
}

// Function to simulate message sending animation
void HammingCode::Graphics(int data[], int m) {
    const int width = 30; // Width of the animation field
    const int delay = 100; // Delay in milliseconds

    cout << "Sender: [ ";
    for (int i = 0; i < m; ++i) {
        cout << data[i] << " ";
    }
    cout << "]" << endl;

    // Clear the initial position of the sender
    for (int i = 0; i < width + 2; ++i) {
        cout << '\r' << string(i, ' ') << ">> [ ";
        for (int j = 0; j < m; ++j) {
            cout << data[j] << " ";
        }
        cout << "]";
        cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    
    // Final position at receiver
}

void HammingCode::calculateParityBits() {
    for (int i = 0; i < p; ++i) {
        int parityPosition = (1 << i);
        parity[i] = 0;

        for (int j = 0; j < m; ++j) {
            if ((j + 1) & parityPosition) {
                parity[i] ^= data[j];
            }
        }
    }
}

void HammingCode::generateCodeword() {
    int j = 0, k = 0;
    for (int i = 0; i < m + p; ++i) {
        if ((i + 1) && !(i & (i + 1))) {
            codeword[i] = parity[k++];
        } else {
            codeword[i] = data[j++];
        }
    }
}

void HammingCode::introduceError() {
    for (int i = 0; i < m + p; ++i) {
        error[i] = codeword[i];
    }
    int pos = rand() % (m + p);
    error[pos] = error[pos] == 0 ? 1 : 0;
    cout << "\nIntroducing error at position " << pos + 1 << endl;
}

void HammingCode::displayCodeword() {
    cout << "Data with parity bits (codeword): ";
    for (int i = 0; i < m + p; ++i) {
        cout << codeword[i];
    }
    cout << endl;
}

void HammingCode::displayReceivedCodeword() {
    cout << "Receiver: [ ";
    for (int i = 0; i < m + p; ++i) {
        cout << error[i];
    }
    cout << " ]" << endl;
}

void HammingCode::detectAndCorrectError() {
    int errorPosition = 0;
    for (int i = 0; i < p; ++i) {
        int parityPosition = (1 << i);
        int parityCheck = 0;

        for (int j = 0; j < m + p; ++j) {
            if ((j + 1) & parityPosition) {
                parityCheck ^= error[j];
            }
        }

        if (parityCheck != 0) {
            errorPosition += parityPosition;
        }
    }

    if (errorPosition == 0) {
        cout << "No error detected." << endl;
    } else {
        cout << "Error detected at position: " << errorPosition << endl;
        error[errorPosition - 1] = error[errorPosition - 1] == 0 ? 1 : 0;
        cout << "Corrected codeword: [ ";
        for (int i = 0; i < m + p; ++i) {
            cout << error[i];
        }
        cout << " ]" << endl;
    }
}

int main() {
    srand(time(0));

    HammingCode hamming;

    hamming.inputData();
    hamming.calculateParityBits();
    hamming.generateCodeword();
    hamming.displayCodeword();
    hamming.introduceError();
    hamming.displayReceivedCodeword();
    hamming.detectAndCorrectError();

    return 0;
}