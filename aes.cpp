#include<iostream>
#include<string>

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

string ReadFile(string file_path)
{
	
}

void EncryptFile(char key[16], string file_data) {
	
}

void DecryptFile(char key[16], string file_data) {

}

int main(int argc, char* argv[])
{
	printf("AES Encryption Demo - Kade Samson & Ari Salehpour\n");
	
	char user_choice	= '\0';
	string key			= "thisisthekey!1!!";
	string file_data	= NULL;
	
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
				DecryptFile(key, "secret_message.txt");
				break;

			default:
				break;
		}
	}
	
	printf("Bye bye!\n");
	return 0;
}
