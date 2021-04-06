#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Company.h"
#include "AirportManager.h"
#include "General.h"
#include "main.h"

const char* str[eNofOptions] = { "Add Flight", "Add Airport",
"PrintCompany", "Print all Airports",
	"Print flights between origin-destination",
"Sort Flights", "Search Flight" };

//"airport_authority.txt"
//"company.bin"
int main(int argc,char* argv[]) //-------------------------------- exist files name in project for add in args -> airport_authority companyBytes 
{
	AirportManager	manager;
	Company			company;

	int isParameterFileOk=0; 
	if (argc == 3)				//2. main gets inputs
	{
		isParameterFileOk = 1;
		initManagerAndCompany(&manager, &company, argv[1], argv[2]);
	}
	else
	{
			initManager(&manager, ""); //without file name ""
			initCompany(&company, &manager);
	}
		
	int option;
	int stop = 0;
	do
	{
		option = menu();
		switch (option)
		{
		case eAddFlight:
			if (!addFlight(&company, &manager))
				printf("Error adding flight\n");
			break;


		case eAddAirport:
			if (!addAirport(&manager))
				printf("Error adding airport\n");
			break;

		case ePrintCompany:
			printCompany(&company,company.name, "Hachi", "Babit", "Ba", "Olam", NULL); //1. the variadic method 
			break;

		case ePrintAirports:
			printAirports(&manager);
			break;

		case ePrintFlightOrigDest:
			printFlightsCount(&company);
			break;

		case eSortFlights:
			sortFlight(&company);
			break;

		case eSearchFlight:
			findFlight(&company);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	if(isParameterFileOk)
	{
		saveManagerToFile(&manager, argv[1]);
		#ifdef BINARY_FILE_BY_BITS
			saveCompanyBytesDetails(&company, argv[2]);
		#else
			saveCompanyToFile(&company, argv[2]);
		#endif //BINARY_FILE_BY_BITS or not
	
	}
	freeManager(&manager);
	freeCompany(&company);
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);

	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initManagerAndCompany(AirportManager* pManager, Company* pCompany, char* managerFileName, char* companyFileName)
{	
		int res = initManager(pManager, managerFileName);
		if (!res)
		{
			printf("error init manager\n");
			return 0;
		}
		if (res == FROM_FILE)
		{
			if (initCompanyFromFile(pCompany, pManager, companyFileName) == 0)
				initCompany(pCompany, pManager);
		}
		else
			initCompany(pCompany, pManager);
	return 1;
}