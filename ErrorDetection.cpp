#include <iostream>
#include <cmath>
using namespace std;

// Function to calculate parity bits and place them at positions that are powers of 2
void addParityBits(int data[], int &size) {
    int numParityBits = 0;

    // Calculate the number of parity bits needed
    while (pow(2, numParityBits) < (size + numParityBits + 1)) {
        numParityBits++;
    }

    int newSize = size + numParityBits;
    int newData[newSize];

    int j = 0, k = 0;
    for (int i = 1; i <= newSize; i++) {
        if (i == pow(2, k)) {
            newData[i - 1] = 0; // Place parity bits as 0 initially
            k++;
        } else {
            newData[i - 1] = data[j];
            j++;
        }
    }

    // Calculate parity bits
    for (int i = 0; i < numParityBits; i++) {
        int pos = pow(2, i);
        int count = 0;
        for (int j = pos - 1; j < newSize; j += 2 * pos) {
            for (int k = 0; k < pos && j + k < newSize; k++) {
                if (newData[j + k] == 1) {
                    count++;
                }
            }
        }
        newData[pos - 1] = count % 2;
    }

    for (int i = 0; i < newSize; ++i) {
        data[i] = newData[i];
    }

    size = newSize;
}

// Function to get user input data
void getUserInputData(int data[], int size) {
    cout << "Enter " << size << " data bits (only 0 and 1): ";
    for (int j = 0; j < size; ++j) {
        cin >> data[j];
        while (data[j] != 0 && data[j] != 1) {
            cout << "Invalid input! Only 0's and 1's are allowed. Enter again: ";
            cin >> data[j];
        }
    }
}

// Function to display bits
void displayBits(int bits[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << bits[i];
    }
    cout << endl;
}



int main() {
    int size;
    cout << "1. VRC (Vertical Redundancy Method) \n2. LRC (Longitudinal Redundancy Method)\n\n";
    cout << "Enter message size (Total bits present in the message): ";
    cin >> size;

    int numParityBits = 0;
    while (pow(2, numParityBits) < (size + numParityBits + 1)) {
        numParityBits++;
    }

    int dataSize = size;
    int data[size + numParityBits]; // allocate maximum possible size after adding parity bits

    getUserInputData(data, size);

    addParityBits(data, dataSize);

    cout << "Data with Parity bits: ";
    displayBits(data, dataSize);

    return 0;
}
