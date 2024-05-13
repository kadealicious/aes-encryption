#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

typedef unsigned char BYTE;

/// Display the menu and get user input.
char DisplayMenu()
{
	printf("Please choose a command:\n");
	printf(" - [E]ncrypt\n");
	printf(" - [D]ecrypt\n");
	printf(" - [Q]uit the program\n");
	printf("Your choice: ");

	return getchar();
}

/// Return data from a file given a path.
string ReadFile(string file_path)
{
	ifstream file_stream(file_path);
	string file_data;
	if(file_stream)
	{
		ostringstream ss;
		ss << file_stream.rdbuf();
		file_data = ss.str();
		return file_data;
	}
	cout << "File \"" << file_path << "\" not found!" << endl;
	return "";
}

/// Encrypt a file using AES-128.
void EncryptFile(BYTE key[16], string unencrypted_file_path)
{
	string file_data = ReadFile(unencrypted_file_path);
	if(file_data == "")
	{
		cout << "Nothing to encrypt!" << endl;
		return;
	}
	cout << "Unencrypted file data: " << file_data << endl;
}

/** Decrypt a block a AES-128 cipher text; if successful, will place a cstring 
	into the decrypted_block parameter. */
void DecryptBlock(BYTE input_block[16], BYTE (&decrypted_block)[16])
{
	// AES-128 always has 10 rounds of encryption and an exp. key size of 176.
	const int round_count		= 10;
	const int expanded_key_size	= 176;
	BYTE expanded_key[expanded_key_size];
	
	// Swap rows and columns (AES is column major).
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			decrypted_block[(i + (j * 4))] = input_block[(i * 4) + j];
		}
	}
}

/// Decrypt a file that was encrypted using AES-128.
void DecryptFile(BYTE key[16], string encrypted_file_path)
{
	string file_data = ReadFile(encrypted_file_path);
	if(file_data == "")
	{
		cout << "Nothing to decrypt!" << endl;
		return;
	}
	cout << "Encrypted file data: " << file_data << endl;
}

int main(int argc, char* argv[])
{
	printf("AES Encryption Demo - Kade Samson & Ari Salehpour\n");
	
	char user_choice	= '\0';
	BYTE key[16]		= {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	
	// While we have not quit, cycle through user input and options.
	while(user_choice != 'Q' && user_choice != 'q')
	{
		// After the user enters input, don't display the menu twice.
		if(user_choice != '\n')	{ user_choice = DisplayMenu(); }
		else					{ user_choice = getchar(); }

		// Handle user choices.
		switch(user_choice)
		{
			// Encrypt file.
			case 'E':
			case 'e':
				EncryptFile(key, "secret_message.txt");
				break;

			// Decrypt file.
			case 'D':
			case 'd':
				DecryptFile(key, "secret_message_encrypted.txt");
				break;

			default:
				break;
		}
	}
	
	printf("Bye bye!\n");
	return 0;
}
