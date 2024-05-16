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

/// Prompt the user for the desired file's path.
string AskForFilePath() {
	
	// Clear stdin buffer.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Get file path from user.
	string filePath;
    cout << "Please enter the file path: ";
    getline(cin, filePath);
	
	return filePath;
}

/// Get the contents of a file from its path.
vector<unsigned char> GetFileContents(string file_path) {

	// Read file into memory.
    ifstream file(file_path, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << file_path << '\n';
		vector<unsigned char> empty_file(0);
        return empty_file;
    }

	// Read the file into file_contents.
    vector<unsigned char> file_contents(
		(istreambuf_iterator<char>(file)),
		istreambuf_iterator<char>()
	);
    file.close();
	
	return file_contents;
}

/// Attempt encryption using the referenced AES object.
void TryEncryption(AES& aes, string file_path, vector<unsigned char> file_data) {

    try {
		// Encrypt the data.
        vector<unsigned char> encryptedData = aes.Encrypt(file_data);
		
		// Create a new file path with the name prefix "encrypted_".
        fs::path path(file_path);
        string newFileName = "encrypted_" + path.stem().string() + ".txt";

		// Create and open the output file.
        ofstream outFile(newFileName, ios::out | ios::binary);
        if (!outFile.is_open()) {
            cerr << "Failed to create output file: " << newFileName << '\n';
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

/// Attempt decryption using the referenced AES object.
void TryDecryption(AES& aes, string file_path, vector<unsigned char> file_data) {

    try {
		// Encrypt the data.
        vector<unsigned char> decryptedData = aes.Decrypt(file_data);
		
		// Create a new file path with the name prefix "decrypted_".
        fs::path path(file_path);
        string newFileName = "decrypted_" + path.stem().string() + ".txt";

		// Create and open the output file.
        ofstream outFile(newFileName, ios::out | ios::binary);
        if (!outFile.is_open()) {
            cerr << "Failed to create output file: " << newFileName << '\n';
            return;
        }
		
		// Write to the output file and close it.
        outFile.write(reinterpret_cast<const char*>(decryptedData.data()), decryptedData.size());
        outFile.close();
        cout << "Decryption successful. Output file created: " << newFileName << '\n';
		
    } catch (const exception& e) {
	
		// Uh oh!  Encryption failed!!!
        cerr << "Decryption failed: " << e.what() << '\n';
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
			case 'e': {
				// Get the contents of the user's favorite file and attempt to encrypt it!
				string file_path = AskForFilePath();
				vector<unsigned char> file_data = GetFileContents(file_path);
				TryEncryption(aes, file_path, file_data);
				cout << endl;
				break;
			}

			// Decrypt file.
			case 'D':
			case 'd': {
				// Get the contents of the user's favorite file and attempt to decrypt it.
				string file_path = AskForFilePath();
				vector<unsigned char> file_data = GetFileContents(file_path);
				TryDecryption(aes, file_path, file_data);
				cout << endl;
				break;
			}

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