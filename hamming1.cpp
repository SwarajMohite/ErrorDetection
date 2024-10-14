#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
//#include <conio.h>

int calculateParityBits(int dataBits){
	int parityBits = 0;
	while(pow(2,parityBits) < (dataBits + parityBits + 1)){
		parityBits++;
	}
	return parityBits;
}

vector<int> insertParityBits(vector<int>& data,int parityBits){
	int totalBits = data.size() + parityBits;
	vector<int> hammingCode(totalBits, 0);

	int j =0;
	for(int i=1; i<=totalBits; i++){
		if((i&(i-1)) != 0){
		   hammingCode[i-1] = data[j];
		   j++;
		}
	}
	return hammingCode;
}

void calculateParityBits(vector<int>& hammingCode, int parityBits){
	int totalBits = hammingCode.size();

	for(int i=0; i<parityBits; i++){
	    int parityPos = pow(2,i) - 1;
	    int parity = 0;

	    for(int j=parityPos; j<totalBits; j++){
	       if((j+1) & (parityPos + 1)){
		  parity ^= hammingCode[j];
	       }
	    }

	    hammingCode[parityPos] = parity;
	}
}

int detectError(vector<int>& hammingCode, int parityBits){
	int totalBits = hammingCode.size();
	int errorPosition = 0;

	for(int i=0;i<parityBits;i++){
	  int parityPos = pow(2,i) - 1;
	  int parity = 0;

	  for(int j=parityPos; j<totalBits; j++){
	     if((j+1) & (parityPos + 1)){
	       parity ^= hammingCode[j];
	     }
	  }

	  if(parity != 0){
	      errorPosition += parityPos + 1;
	  }
	}
	return errorPosition;
}

void printHammingCode(vector<int>& hammingCode){
	for(int i=0;i<hammingCode.size();i++){
	   cout<<hammingCode[i]<<"  ";
	}
	cout<<endl;
}

void introduceRandomErrors(vector<int>& hammingCode, int errorCount){
	srand(static_cast<unsigned int>(time(0)));
	
	for(int i=0;i<errorCount;i++){
		int pos = rand() % hammingCode.size();
		hammingCode[pos] ^= 1;
	}
}

int main(){
	int dataBits;
//	clrscr();
	cout<<"Enter the number of data bits: ";
	cin>>dataBits;

	vector<int> data(dataBits);
	cout<<"Enter the data bits seperated by space: ";
	for(int i=0; i<dataBits;i++){
	   cin>>data[i];
	}

	int parityBits = calculateParityBits(dataBits);
	cout<<"Number of Parity bits needed : "<<parityBits<<endl;

	vector<int> hammingCode = insertParityBits(data,parityBits);

	calculateParityBits(hammingCode,parityBits);

	cout<<"Hamming code with parity bits: ";
	printHammingCode(hammingCode);

	int errorPosition = detectError(hammingCode,parityBits);

	if(errorPosition == 0){
	   cout<<"No errors detected in the hamming code. "<<endl;
	   
	}
	else{
	   cout<<"Error detected at position : "<<errorPosition<<endl;

	   hammingCode[errorPosition - 1] ^= 1;

	   cout<<"Corrected Hamming Code: ";

	   printHammingCode(hammingCode);
	}

	int option;
	cout<<"Do you want to introduce random errors \nor\n Do you have data of Receivers side ? \n(1 for random errors, 2 to give data of receivre's side , 0 for No option): ";
	cin>>option;
	
	if(option == 1){
		int errorCount;
		cout<<"Enter the number of random error to introduce: ";
		cin>>errorCount;
		introduceRandomErrors(hammingCode,errorCount);
		cout<<"Hamming code after intoducing random error : ";
		printHammingCode(hammingCode);	
	
	int errorPosition = detectError(hammingCode,parityBits);

	if(errorPosition == 0){
	   cout<<"No errors detected in the hamming code. "<<endl;
	}
	else{
	   cout<<"Error detected at position : "<<errorPosition<<endl;

	   hammingCode[errorPosition - 1] ^= 1;

	   cout<<"Corrected Hamming Code: ";

	   printHammingCode(hammingCode);
	}
	}
	
	else if(option == 2){
		vector<int> receivedHammingCode(hammingCode.size());
		cout<<"Enter the received Hamming Code : ";
		
		for(int i=0;i<receivedHammingCode.size();i++){
			cin>>receivedHammingCode[i];
		}
		
		int errorPosition = detectError(receivedHammingCode,parityBits);
		
		if(errorPosition == 0){
	   		cout<<"No errors detected in the hamming code. "<<endl;
		}
		else{
	   		cout<<"Error detected at position : "<<errorPosition<<endl;

	   		hammingCode[errorPosition - 1] ^= 1;

	   		cout<<"Corrected Hamming Code: ";

	   		printHammingCode(hammingCode);
		}
	}
	else if(option == 0){
		exit (1);
	}

//	getch();
	return 0;
}
