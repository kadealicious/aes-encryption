#include<stdio.h>

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
	
	char user_choice = '\0';
	while(user_choice != 'Q' && user_choice != 'q')
	{
		switch(user_choice)
		{
			case 'R':
			case 'r':
				break;

			case 'E':
			case 'e':
				break;

			case 'D':
			case 'd':
				break;

			default:
				break;
		}
		
		if(user_choice != '\n') {
			user_choice = DisplayMenu();
		} else {
			user_choice = getchar();
		}
	}

	return 0;
}
