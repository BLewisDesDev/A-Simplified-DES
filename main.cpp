//============================================================================
// Name        : InfoSec assignement
// Author      : Byron
// Version     : 2.0
// Description : Data Encryption Standard
//============================================================================

#include <iostream>

using namespace std;

struct keyPair{
	string k1;
	string k2;
};

	// -- -- -- Main Method Stubs -- -- -- //

keyPair keyGen(string);
string encrypt(string,string,string);
string decrypt(string,string,string);

	// -- -- -- Helper Method Stubs -- -- -- //

void leftShift(char*, int);
void strToChAr(string,char*,int);
void printChAr(char*,int,string);
void permute(int* func,char* target ,char* dest, int length );
void split(char* fh, char* sh, char* src, int hSize);
void bitAnd(char* key, char* src, char* res, int size);
int binToInt(char* in);
void intToBin(int in, char* out);
void s0(char* fh, char* s0Res);
void s1(char* sh, char* s1Res);
void swap(char*,char*,int);

// -- -- -- MAIN -- -- -- //

int main()
{
	string choice;
	string enkey;
	keyPair keys;
	string plainText;
	string cypherText;

	cout << "\n'e' = Encryption \n'd' = Decryption \n";
	cin >> choice;

	if (choice == "e")
	{
		cout << "\nEnter Encryption Key (10 bits): ";
		cin >> enkey;

		cout << "\nEnter Plain Text (8 bits): ";
		cin >> plainText;

//		enkey = "1010101010";
//		plainText = "10101010";
//		string key = "1010101010";
//		string p = "10101010";
//		string k1 = "11100100";
//		string k2 = "01010011";
//		string c = "01101011";

		keys = keyGen(enkey);

		cypherText = encrypt(keys.k1, keys.k2, plainText);

		cout << "\n\nCypher text: " << cypherText << endl;
//		cout << "Answer text: 01101011" << endl;

	}
	else if (choice == "d")
	{
		cout << "\nEnter Decryption Key (10 bits): ";
		cin >> enkey;

		cout << "\nEnter Cypher Text (8 bits): ";
		cin >> cypherText;

//		enkey = "1010101010";
//		cypherText = "01101011";
//		string key = "1010101010";
//		string p = "10101010";
//		string k1 = "11100100";
//		string k2 = "01010011";
//		string c = "01101011";

		keys = keyGen(enkey);

		plainText = decrypt(keys.k1, keys.k2, cypherText);

		cout << "\n Plain text: " << plainText << endl;
//		cout << "Answer text: 10101010" << endl;
	}
	else
	{
		cout << "\nOnly single characters 'e' and 'd' allowed" << endl;
	}

    return 0;
}

	// - - - - - Main Methods - - - - - //

keyPair keyGen(string s) {

	cout << "\n\n< - - - - Generating Keys - - - >" << endl;

	keyPair result;

	char key[10] = {'x','x','x','x','x','x','x','x','x','x'};
	char intermediate10[10] = {'x','x','x','x','x','x','x','x','x','x'};
	char intermediate8[9] = {'x','x','x','x','x','x','x','x','x'};
	char firstHalf[5] = {'x','x','x','x','x'};
	char secondHalf[5] = {'x','x','x','x','x'};


	// - - - - - Convert to char array - - - - - //

	strToChAr(s, key, 10);

	// - - - - - - - P10 - - - - - - - //

	int p10[10] = {3,5,2,7,4,10,1,9,8,6};
	permute(p10,key,intermediate10,10);

	// - - - - - - - Split - - - - - - - //

	split(firstHalf,secondHalf,intermediate10,5);

	// - - - - - LS1 - - - - - //

	leftShift(firstHalf,1);
	leftShift(secondHalf,1);

	// - - - - - Reassemble String - - - - - //

	for(int i = 0; i < 5; i++) {
		intermediate10[i] = firstHalf[i];
	}

	for(int i = 0; i < 5; i++) {
		intermediate10[i+5] = secondHalf[i];
	}

	// - - - - - P8 - - - - - //

	int p8[8] = {6,3,7,4,8,5,10,9};
	permute(p8,intermediate10,intermediate8,8);

	intermediate8[8] = '\0';
	result.k1 = intermediate8;
	cout << "Key 1: " << result.k1 << endl;

	// - - - - - LS2 - - - - - //

	leftShift(firstHalf,2);
	leftShift(secondHalf,2);

	// - - - - - Reassemble String - - - - - //

	for(int i = 0; i < 5; i++) {
		intermediate10[i] = firstHalf[i];
	}

	for(int i = 0; i < 5; i++) {
		intermediate10[i+5] = secondHalf[i];
	}

	// - - - - - P8 - - - - - //

	permute(p8,intermediate10,intermediate8,8);

	intermediate8[8] = '\0';
	result.k2 = intermediate8;
	cout << "Key 2: " << result.k2 << endl;

	return result;
}

string encrypt(string k1, string k2,  string p) {

	cout << "\n< - - - - Encrypting - - - >" << endl;

	string result;

	//Resources

	char plainText[8] = {'x','x','x','x','x','x','x','x'};

	char key1[8] = {'x','x','x','x','x','x','x','x'};
	char key2[8] = {'x','x','x','x','x','x','x','x'};

	char intermediate8[8] = {'x','x','x','x','x','x','x','x'};
	char and8[8] = {'x','x','x','x','x','x','x','x'};

	char firstHalf[4] = {'x','x','x','x'};
	char secondHalf[4] = {'x','x','x','x'};

	char ipFirstHalf[4] = {'x','x','x','x'};
	char ipSecondHalf[4] = {'x','x','x','x'};

	char s0Result[4];
	char s1Result[4];

	char intermediate4[4];
	char and4[4] = {'x','x','x','x'};

	//Permutation Functions

	int ip[8] = {2,6,3,1,4,8,5,7};
	int ep[8] = {4,1,2,3,2,3,4,1};
	int p4[4] = {2,4,3,1};
	int invp[8] = {4,1,3,5,7,2,8,6};

	// - - - - - Char Array and print - - - - - //

	strToChAr(p, plainText, 8);
	printChAr(plainText,8,"Plain Text: ");

	// - - - - - ip8 - - - - - //

	permute(ip,plainText,intermediate8,8);
	printChAr(intermediate8,8,"ip on Plain Text: ");

	ipFirstHalf[0] = intermediate8[0];
	ipFirstHalf[1] = intermediate8[1];
	ipFirstHalf[2] = intermediate8[2];
	ipFirstHalf[3] = intermediate8[3];

	printChAr(ipFirstHalf,4,"Ip firstHalf: ");

	ipSecondHalf[0] = intermediate8[4];
	ipSecondHalf[1] = intermediate8[5];
	ipSecondHalf[2] = intermediate8[6];
	ipSecondHalf[3] = intermediate8[7];

	printChAr(ipSecondHalf,4,"Ip SecondHalf: ");

	// - - - - - - - - - - - - - - - - - - FK1 - - - - - - - - - - - - - - - - - - - - //

	// - - - - - Split String - - - - - //

	split(firstHalf,secondHalf,intermediate8,4);
	printChAr(firstHalf,4,"First Half: ");
	printChAr(secondHalf,4,"Second Half: ");

	// - - - - - Second Half EP - - - - - //



	permute(ep,secondHalf,intermediate8,8);
	printChAr(intermediate8,8,"Ep on SecondHalf: ");

	// - - - - - Bitwise & K1 - - - - - //

	strToChAr(k1,key1,8);
	bitAnd(key1,intermediate8,and8,8);

	printChAr(intermediate8,8,"a1: ");
	printChAr(key1,8,"k1: ");
	printChAr(and8,8,"And Result: ");

	// - - - - - split And Result - - - - - //

	split(firstHalf,secondHalf,and8,4);
	printChAr(firstHalf,4,"First Half: ");
	printChAr(secondHalf,4,"Second Half: ");

	// - - - - - S-box - - - - - //  // - - - - - Problem - - - - - //

	s0(firstHalf, s0Result);
	s1(secondHalf, s1Result);

	cout << "s0Box bin res: " << s0Result << endl;
	cout << "s1Box bin res: " << s1Result << endl;

	// - - - - - Assemble 4 - - - - - //

	intermediate4[0] = s0Result[0];
	intermediate4[1] = s0Result[1];
	intermediate4[2] = s1Result[0];
	intermediate4[3] = s1Result[1];

	printChAr(intermediate4,4,"Assembled after sboxes: ");

	// - - - - - p4 - - - - - //

	char intermediate41[4];

	permute(p4,intermediate4,intermediate41,4);
	printChAr(intermediate41,4,"After p4: ");

	// - - - - - And with IP first Half - - - - - //

	bitAnd(ipFirstHalf,intermediate41,and4,4);
	printChAr(ipFirstHalf,4,"a1: ");
	printChAr(intermediate41,4,"a2: ");

	printChAr(and4,4,"And 4 Result: ");

//	// - - - - - Swap and assemble - - - - - //

	char sw[8];

	sw[0] = ipSecondHalf[0];
	sw[1] = ipSecondHalf[1];
	sw[2] = ipSecondHalf[2];
	sw[3] = ipSecondHalf[3];

	sw[4] = and4[0];
	sw[5] = and4[1];
	sw[6] = and4[2];
	sw[7] = and4[3];

	printChAr(sw,8,"After Swap: ");

	// - - - - - - - - - - - - - - - - - - END FK1 - - - - - - - - - - - - - - - - - - - - //

	// - - - - - - - - - - - - - - - - - - FK2 - - - - - - - - - - - - - - - - - - - - //

	// - - - - - Split String - - - - - //

	split(firstHalf,secondHalf,sw,4);
	printChAr(firstHalf,4,"First Half: ");
	printChAr(secondHalf,4,"Second Half: ");

	ipFirstHalf[0] = sw[0];
	ipFirstHalf[1] = sw[1];
	ipFirstHalf[2] = sw[2];
	ipFirstHalf[3] = sw[3];

	printChAr(ipFirstHalf,4,"SW firstHalf: ");

	ipSecondHalf[0] = sw[4];
	ipSecondHalf[1] = sw[5];
	ipSecondHalf[2] = sw[6];
	ipSecondHalf[3] = sw[7];

	printChAr(ipSecondHalf,4,"SW SecondHalf: ");

	// - - - - - Second Half EP - - - - - //

	permute(ep,secondHalf,intermediate8,8);
	printChAr(intermediate8,8,"Ep on SecondHalf: ");


	// - - - - - Bitwise & K2 - - - - - //

	strToChAr(k2,key2,8);
	bitAnd(key2,intermediate8,and8,8);

	printChAr(intermediate8,8,"a2: ");
	printChAr(key2,8,"k2: ");
	printChAr(and8,8,"Ar: ");

	// - - - - - split And Result - - - - - //

	split(firstHalf,secondHalf,and8,4);
	printChAr(firstHalf,4,"First Half: ");
	printChAr(secondHalf,4,"Second Half: ");

	// - - - - - S-box - - - - - //

	s0(firstHalf, s0Result);
	s1(secondHalf, s1Result);

	cout << "s0Box bin res: " << s0Result << endl;
	cout << "s1Box bin res: " << s1Result << endl;

	// - - - - - Assemble 4 - - - - - //

	intermediate4[0] = s0Result[0];
	intermediate4[1] = s0Result[1];
	intermediate4[2] = s1Result[0];
	intermediate4[3] = s1Result[1];

	printChAr(intermediate4,4,"Before P4: ");

	// - - - - - p4 - - - - - //

	permute(p4,intermediate4,intermediate41,4);
	printChAr(intermediate41,4,"After P4: ");

	// - - - - - And with IP first Half - - - - - //

	bitAnd(ipFirstHalf,intermediate41,and4,4);
	printChAr(ipFirstHalf,4,"a1: ");
	printChAr(intermediate41,4,"a2: ");

	printChAr(and4,4,"Ar: ");

	// - - - - - Assemble 8 - - - - - //

	sw[0] = and4[0];
	sw[1] = and4[1];
	sw[2] = and4[2];
	sw[3] = and4[3];
	sw[4] = ipSecondHalf[0];
	sw[5] = ipSecondHalf[1];
	sw[6] = ipSecondHalf[2];
	sw[7] = ipSecondHalf[3];

// - - - - - - - - - - - - - - - - - - END FK2 - - - - - - - - - - - - - - - - - - - - //

	printChAr(sw,8,"Before ip-1: ");

	permute(invp,sw,intermediate8,8);

	intermediate8[8] = '\0';
	result = intermediate8;

	return result;
}

string decrypt(string k1, string k2, string c) {

	cout << "\n< - - - - Decrypting - - - >" << endl;

	string result;

		//Resources

		char plainText[8] = {'x','x','x','x','x','x','x','x'};

		char key1[8] = {'x','x','x','x','x','x','x','x'};
		char key2[8] = {'x','x','x','x','x','x','x','x'};

		char intermediate8[8] = {'x','x','x','x','x','x','x','x'};
		char and8[8] = {'x','x','x','x','x','x','x','x'};

		char firstHalf[4] = {'x','x','x','x'};
		char secondHalf[4] = {'x','x','x','x'};

		char ipFirstHalf[4] = {'x','x','x','x'};
		char ipSecondHalf[4] = {'x','x','x','x'};

		char s0Result[4];
		char s1Result[4];

		char intermediate4[4];
		char and4[4] = {'x','x','x','x'};

		//Permutation Functions

		int ip[8] = {2,6,3,1,4,8,5,7};
		int ep[8] = {4,1,2,3,2,3,4,1};
		int p4[4] = {2,4,3,1};
		int invp[8] = {4,1,3,5,7,2,8,6};

		// - - - - - Char Array and print - - - - - //

		strToChAr(c, plainText, 8);
		printChAr(plainText,8,"Plain Text: ");

		// - - - - - ip8 - - - - - //

		permute(ip,plainText,intermediate8,8);
		printChAr(intermediate8,8,"ip on Plain Text: ");

		ipFirstHalf[0] = intermediate8[0];
		ipFirstHalf[1] = intermediate8[1];
		ipFirstHalf[2] = intermediate8[2];
		ipFirstHalf[3] = intermediate8[3];

		printChAr(ipFirstHalf,4,"Ip firstHalf: ");

		ipSecondHalf[0] = intermediate8[4];
		ipSecondHalf[1] = intermediate8[5];
		ipSecondHalf[2] = intermediate8[6];
		ipSecondHalf[3] = intermediate8[7];

		printChAr(ipSecondHalf,4,"Ip SecondHalf: ");

		// - - - - - - - - - - - - - - - - - - FK2 - - - - - - - - - - - - - - - - - - - - //

		// - - - - - Split String - - - - - //

		split(firstHalf,secondHalf,intermediate8,4);
		printChAr(firstHalf,4,"First Half: ");
		printChAr(secondHalf,4,"Second Half: ");

		// - - - - - Second Half EP - - - - - //



		permute(ep,secondHalf,intermediate8,8);
		printChAr(intermediate8,8,"Ep on SecondHalf: ");

		// - - - - - Bitwise & K1 - - - - - //

		strToChAr(k2,key2,8);
		bitAnd(key2,intermediate8,and8,8);

		printChAr(intermediate8,8,"a1: ");
		printChAr(key1,8,"k1: ");
		printChAr(and8,8,"And Result: ");

		// - - - - - split And Result - - - - - //

		split(firstHalf,secondHalf,and8,4);
		printChAr(firstHalf,4,"First Half: ");
		printChAr(secondHalf,4,"Second Half: ");

		// - - - - - S-box - - - - - //  // - - - - - Problem - - - - - //

		s0(firstHalf, s0Result);
		s1(secondHalf, s1Result);

		cout << "s0Box bin res: " << s0Result << endl;
		cout << "s1Box bin res: " << s1Result << endl;

		// - - - - - Assemble 4 - - - - - //

		intermediate4[0] = s0Result[0];
		intermediate4[1] = s0Result[1];
		intermediate4[2] = s1Result[0];
		intermediate4[3] = s1Result[1];

		printChAr(intermediate4,4,"Assembled after sboxes: ");

		// - - - - - p4 - - - - - //

		char intermediate41[4];

		permute(p4,intermediate4,intermediate41,4);
		printChAr(intermediate41,4,"After p4: ");

		// - - - - - And with IP first Half - - - - - //

		bitAnd(ipFirstHalf,intermediate41,and4,4);
		printChAr(ipFirstHalf,4,"a1: ");
		printChAr(intermediate41,4,"a2: ");

		printChAr(and4,4,"And 4 Result: ");

	//	// - - - - - Swap and assemble - - - - - //

		char sw[8];

		sw[0] = ipSecondHalf[0];
		sw[1] = ipSecondHalf[1];
		sw[2] = ipSecondHalf[2];
		sw[3] = ipSecondHalf[3];

		sw[4] = and4[0];
		sw[5] = and4[1];
		sw[6] = and4[2];
		sw[7] = and4[3];

		printChAr(sw,8,"After Swap: ");

		// - - - - - - - - - - - - - - - - - - END FK2 - - - - - - - - - - - - - - - - - - - - //

		// - - - - - - - - - - - - - - - - - - FK1 - - - - - - - - - - - - - - - - - - - - //

		// - - - - - Split String - - - - - //

		split(firstHalf,secondHalf,sw,4);
		printChAr(firstHalf,4,"First Half: ");
		printChAr(secondHalf,4,"Second Half: ");

		ipFirstHalf[0] = sw[0];
		ipFirstHalf[1] = sw[1];
		ipFirstHalf[2] = sw[2];
		ipFirstHalf[3] = sw[3];

		printChAr(ipFirstHalf,4,"SW firstHalf: ");

		ipSecondHalf[0] = sw[4];
		ipSecondHalf[1] = sw[5];
		ipSecondHalf[2] = sw[6];
		ipSecondHalf[3] = sw[7];

		printChAr(ipSecondHalf,4,"SW SecondHalf: ");

		// - - - - - Second Half EP - - - - - //

		permute(ep,secondHalf,intermediate8,8);
		printChAr(intermediate8,8,"Ep on SecondHalf: ");


		// - - - - - Bitwise & K2 - - - - - //

		strToChAr(k1,key1,8);
		bitAnd(key1,intermediate8,and8,8);

		printChAr(intermediate8,8,"a2: ");
		printChAr(key1,8,"k2: ");
		printChAr(and8,8,"Ar: ");

		// - - - - - split And Result - - - - - //

		split(firstHalf,secondHalf,and8,4);
		printChAr(firstHalf,4,"First Half: ");
		printChAr(secondHalf,4,"Second Half: ");

		// - - - - - S-box - - - - - //

		s0(firstHalf, s0Result);
		s1(secondHalf, s1Result);

		cout << "s0Box bin res: " << s0Result << endl;
		cout << "s1Box bin res: " << s1Result << endl;

		// - - - - - Assemble 4 - - - - - //

		intermediate4[0] = s0Result[0];
		intermediate4[1] = s0Result[1];
		intermediate4[2] = s1Result[0];
		intermediate4[3] = s1Result[1];

		printChAr(intermediate4,4,"Before P4: ");

		// - - - - - p4 - - - - - //

		permute(p4,intermediate4,intermediate41,4);
		printChAr(intermediate41,4,"After P4: ");

		// - - - - - And with IP first Half - - - - - //

		bitAnd(ipFirstHalf,intermediate41,and4,4);
		printChAr(ipFirstHalf,4,"a1: ");
		printChAr(intermediate41,4,"a2: ");

		printChAr(and4,4,"Ar: ");

		// - - - - - Assemble 8 - - - - - //

		sw[0] = and4[0];
		sw[1] = and4[1];
		sw[2] = and4[2];
		sw[3] = and4[3];
		sw[4] = ipSecondHalf[0];
		sw[5] = ipSecondHalf[1];
		sw[6] = ipSecondHalf[2];
		sw[7] = ipSecondHalf[3];

	// - - - - - - - - - - - - - - - - - - END FK2 - - - - - - - - - - - - - - - - - - - - //

		printChAr(sw,8,"Before ip-1: ");

		permute(invp,sw,intermediate8,8);

		intermediate8[8] = '\0';
		result = intermediate8;

		return result;
}


	// - - - - - Helper Methods - - - - - //

void leftShift(char* str, int shift) {

	for (int i = 0; i < shift; i++) {
		char temp;

		temp = str[0];

		for(int i = 0; i < 4; i++) {
			str[i] = str[i+1];
		}

		str[4] = temp;
	}
}

void strToChAr(string str, char* arr, int length){

	for(int i = 0; i < length; i ++) {
		arr[i] = str[i];
	}
}

void printChAr(char* arr, int length, string msg){

	cout << msg;

	for(int i = 0; i < length; i ++) {
		cout << arr[i];
	}

	cout << endl;
}

void permute(int* func, char* target, char* dest, int length){

	int choice;

	for ( int i = 0; i < length; i++ ) {
		choice = func[i];
		dest[i] = target[choice-1];
	}
}

void split(char* fh, char* sh, char* src, int hSize){

	for(int i = 0; i < hSize; i++) {
		fh[i] = src[i];
	}

	for(int i = 0; i < hSize; i++) {
		sh[i] = src[i+hSize];
	}
}

void bitAnd(char* a1, char* a2, char* res, int size) {
	for(int i = 0; i < size; i++) {
		if (a1[i] == a2[i]) {
			res[i] = '0';
		} else {
			res[i] = '1';
		}
	}

}

void s0(char* fh, char* s0Res){

	int s0[4][4] = {{1,0,3,2},
					{3,2,1,0},
					{0,2,1,3},
					{3,1,3,2}};

	int row;
	int col;

	char r[2]  = {fh[0],fh[3]};
	char c[2]  = {fh[1],fh[2]};

	cout << "row: " << fh[0] << fh[3] << endl;
	cout << "col: " << fh[1] << fh[2] << endl;

	row = binToInt(r);
	col = binToInt(c);


	intToBin(s0[row][col], s0Res);

}

void s1(char* sh, char* s1Res) {

	int s1[4][4] = {{0,1,2,3},
					{2,0,1,3},
					{3,0,1,0},
					{2,1,0,3}};

	int row;
	int col;

	char r[2]  = {sh[0],sh[3]};
	char c[2]  = {sh[1],sh[2]};

	cout << "row: " << sh[0] << sh[3] << endl;
	cout << "col: " << sh[1] << sh[2] << endl;

	row = binToInt(r);
	col = binToInt(c);

	intToBin(s1[row][col], s1Res);
}

int binToInt(char* in){

	int out;

	if(in[0] == '0' && in[1] == '0') {
		out = 0;
	} else if (in[0] == '0' && in[1] == '1') {
		out = 1;
	} else if (in[0] == '1' && in[1] == '0') {
		out = 2;
	} else if (in[0] == '1' && in[1] == '1') {
		out = 3;
	} else {
		out = 0;
	}

	return out;
}

void intToBin(int in, char* out)
{
	if(in == 0) {
		out[0] = '0';
		out[1] = '0';
	} else if (in == 1) {
		out[0] = '0';
		out[1] = '1';
	}else if (in == 2) {
		out[0] = '1';
		out[1] = '0';
	} else if (in == 3){
		out[0] = '1';
		out[1] = '1';
	}
}

void swap(char* in, char* out, int l){
	int j = 0;
	for (int i = l-1; i >= 0 ; i--){
		out[j] = in[i];
		j++;
	}
}
