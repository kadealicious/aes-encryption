#include "aes.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

/// Display the menu and get user input.
void DisplayMenu()
{
	printf("Please choose a command:\n");
	printf(" - [E]ncrypt\n");
	printf(" - [D]ecrypt\n");
	printf(" - [Q]uit the program\n");
	printf("Your choice: ");
}

/// Attempt encryption using the referenced AES object.
void TryEncryption(AES& aes) {

	// Clear stdin buffer.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Get file path from user.
	string filePath;
    cout << "Please enter the file path: ";
    getline(cin, filePath);
	
	// Read file into memory.
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << '\n';
        return;
    }

	// Read the file into file_contents.
    vector<unsigned char> fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    try {
		// Encrypt the data.
        vector<unsigned char> encryptedData = aes.Encrypt(fileContents);
		
		// Create a new file path with the name prefix "encrypted_".
        fs::path p(filePath);
        string newFileName = "encrypted_" + p.stem().string() + ".txt";
        string newFilePath = p.parent_path().string() + "/" + newFileName;

		// Create and open the output file.
        ofstream outFile(newFileName, ios::out | ios::binary);
        if (!outFile.is_open()) {
            cerr << "Failed to create output file: " << newFilePath << '\n';
            return;
        }
		
		// Write to the output file and close it.
        outFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
        outFile.close();
        cout << "Encryption successful. Output file created: " << newFileName << '\n';
		
    } catch (const exception& e) {
	
		// Uh oh!  Encryption failed!!!
        cerr << "Encryption failed: " << e.what() << '\n';
        return;
    }
}

/// Attempt encryption using the referenced AES object.
void TryDecryption(AES& aes) {

	// Clear stdin buffer.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Get file path from user.
	string filePath;
    cout << "Please enter the file path: ";
    getline(cin, filePath);
	
	// Read file into memory.
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << '\n';
        return;
    }

	// Read the file into file_contents.
    vector<unsigned char> fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    try {
		// Encrypt the data.
        vector<unsigned char> encryptedData = aes.Decrypt(fileContents);
		
		// Create a new file path with the name prefix "encrypted_".
        fs::path p(filePath);
        string newFileName = "decrypted_" + p.stem().string() + ".txt";
        string newFilePath = p.parent_path().string() + "/" + newFileName;

		// Create and open the output file.
        ofstream outFile(newFileName, ios::out | ios::binary);
        if (!outFile.is_open()) {
            cerr << "Failed to create output file: " << newFilePath << '\n';
            return;
        }
		
		// Write to the output file and close it.
        outFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
        outFile.close();
        cout << "Decryption successful. Output file created: " << newFileName << endl;
		
    } catch (const exception& e) {
	
		// Uh oh!  Encryption failed!!!
        cerr << "Decryption failed: " << e.what() << endl;
        return;
    }
}

int main() {
	
	printf("AES Encryption Demo - Kade Samson & Ari Salehpour\n");
	
	// Create a key of 16 8-byte chars (128 bits total) and give it to an AES object.
    vector<unsigned char> key(16, 'a');
    AES aes(key);
	
	// While user has not requested to quit, process user input.
	char user_choice = '\0';
	while(user_choice != 'Q' && user_choice != 'q')
	{
		// Show the user their options and get their input.
		DisplayMenu();
		user_choice = getchar();
		cout << endl;

		// Handle user choices.
		switch(user_choice)
		{
			// Encrypt file.
			case 'E':
			case 'e':
				TryEncryption(aes);
				cout << endl;
				break;

			// Decrypt file.
			case 'D':
			case 'd':
				TryDecryption(aes);
				cout << endl;
				break;

			case '\n':
			default:
				// Clear stdin buffer.
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
		}
	}
	
	cout << "Farewell!" << endl;
	return 0;
}