#include<stdio.h>

/// Display the menu and get user input.
char DisplayMenu()
{
	printf("Please choose a command:\n");
	printf(" - [R]ead a file\n");
	printf(" - [E]ncrypt\n");
	printf(" - [D]ecrypt\n");
	printf(" - [Q]uit the program\n");
	printf("Your choice: ");

	return getchar();
}

int main(int argc, char* argv[])
{
	printf("AES Encryption Demo - Kade Samson & Ari Salehpour\n");
	
	// While we have not quit, cycle through user input and options.
	char user_choice = '\0';
	while(user_choice != 'Q' && user_choice != 'q')
	{
		// After the user enters input, don't display the menu twice.
		if(user_choice != '\n') {
			user_choice = DisplayMenu();
		} else {
			user_choice = getchar();
		}

		// Handle user choices.
		switch(user_choice)
		{
			// Read file.
			case 'R':
			case 'r':
				break;
			
			// Encrypt file.
			case 'E':
			case 'e':
				break;

			// Decrypt file.
			case 'D':
			case 'd':
				break;

			default:
				break;
		}
	}

	return 0;
}
