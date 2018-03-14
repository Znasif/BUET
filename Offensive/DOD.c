#include<stdio.h>
#include<string.h>


main(int argc, char **argv)
{
	char *p ="j3d1h4x0r";


	if (strcmp(argv[1], p) == 0)
	{
		printf("\nWelcome to Department of Defense Secure Access!\n");

	}
	else 
	{
		printf("\nYou are not authorized to Access this AREA!\n");
	}

	return 0;

}

