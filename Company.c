#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "myMacros.h"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Hour", "Date", "Airport takeoff code", "Airport landing code" };


int	initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flighDateList);
	#ifdef BINARY_FILE_BY_BITS
	if (loadCompanyBytesDetails(pComp, fileName))  //save binary to file use bytes. 
	{
		initDateList(pComp);
		return 1;
	}
	#else
	if (loadCompanyFromFile(pComp, pManaer, fileName))  //save binary to file not use byte, (like in last HW). 
	{
		initDateList(pComp);
		return 1;
	}
	#endif //BINARY_FILE_BY_BITS
	return 0;
}

void	initCompany(Company* pComp, AirportManager* pManaer)
{
	printf("-----------  Init Airline Company\n");
	L_init(&pComp->flighDateList);

	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->sortOpt = 0;
}

void	initDateList(Company* pComp)
{
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isUniqueDate(pComp, i))
		{
			char* sDate = createDateString(&pComp->flightArr[i]->date);
			L_insert(&(pComp->flighDateList.head), sDate);
		}
	}
}

int		isUniqueDate(const Company* pComp, int index)
{
	Date* pCheck = &pComp->flightArr[index]->date;
	for (int i = 0; i < index; i++)
	{
		if (equalDate(&pComp->flightArr[i]->date, pCheck))
			return 0;
	}
	return 1;
}

int		addFlight(Company* pComp, const AirportManager* pManager)
{

	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	CHECK_RETURN_0(pComp->flightArr); //use macro

	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	CHECK_RETURN_0(pComp->flightArr[pComp->flightCount]); //use macro

	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	if (isUniqueDate(pComp, pComp->flightCount))
	{
		char* sDate = createDateString(&pComp->flightArr[pComp->flightCount]->date);
		L_insert(&(pComp->flighDateList.head), sDate);
	}
	pComp->flightCount++;
	return 1;
}

void	printCompany(Company* pComp, char* firstName, ...) //we are not change first name , because when user pick print comp it will conact string again
{
	if (firstName == NULL)
		return;
	va_list lst;
	char* word = firstName;
	int count = 0;
	va_start(lst, firstName);
	while (word != NULL)
	{
		count++;
		word=va_arg(lst, char*);
	}
	va_end(lst);
	va_start(lst, firstName);
	word = firstName;
	for (int i = 0; i < count - 1; i++)
	{
		printf("%s_", word);
		word=va_arg(lst, char*);
	}
	printf("%s\n", word);
	va_end(lst);
	printf("Has %d flights\n", pComp->flightCount);
	// in all print cases we print until that point
	#ifdef DETAIL_PRINT
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flighDateList, printStr);
	#endif // DETAIL_PRINT
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}

int		saveCompanyToFile(const Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open copmpany file to write\n");
		return 0;
	}

	if (!writeStringToFile(pComp->name, fp, "Error write comapny name\n"))
		return 0;

	if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
		return 0;

	if (!writeIntToFile((int)pComp->sortOpt, fp, "Error write sort option\n"))
		return 0;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!saveFlightToFile(pComp->flightArr[i], fp))
			return 0;
	}

	fclose(fp);
	return 1;
}

int loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Error open company file\n"); //using macro

	pComp->flightArr = NULL;


	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	if (!pComp->name)
		return 0;

	if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flight count name\n"))
		return 0;

	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
		return 0;

	pComp->sortOpt = (eSortOption)opt;

	if (pComp->flightCount > 0)
	{
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		CHECK_NULL__MSG_COLSE_FILE(pComp->flightArr, fp, "Alocation error\n"); //use macro
	}
	else
		pComp->flightArr = NULL;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		if (!pComp->flightArr[i])
		{
			MSG_CLOSE_RETURN_0("Alocation error\n", fp); //use macro
		}
		if (!loadFlightFromFile(pComp->flightArr[i], pManager, fp))
			return 0;
	}

	fclose(fp);
	return 1;
}

void	sortFlight(Company* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortOpt)
	{
	case eHour:
		compare = compareByHour;
		break;
	case eDate:
		compare = compareByDate;
		break;
	case eSorceCode:
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		compare = compareByCodeDest;
		break;

	}

	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);

}

void	findFlight(const Company* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;


	switch (pComp->sortOpt)
	{
	case eHour:
		f.hour = getFlightHour();
		compare = compareByHour;
		break;
	case eDate:
		getchar();
		getCorrectDate(&f.date);
		compare = compareByDate;
		break;
	case eSorceCode:
		getchar();
		getAirportCode(f.originCode);
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		getchar();
		getAirportCode(f.destCode);
		compare = compareByCodeDest;
		break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pF == NULL)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}

}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

void	freeCompany(Company* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->flighDateList, freePtr);
}
int saveCompanyBytesDetails(const Company* pComp, const char* fileName)
{
	BYTE pBuffer[BINARY_BUFFER_SIZE_COMPANY];
	saveToBufferCompanyBytesDetails(pComp, pBuffer);
	FILE* fp = fopen(fileName, "wb");
	if (fp == NULL)
		return 0;
	if (fwrite(pBuffer, sizeof(BYTE), BINARY_BUFFER_SIZE_COMPANY, fp) != BINARY_BUFFER_SIZE_COMPANY) //check if num of element write = BINARY_BUFFER_SIZE_COMPANY
	{
		fclose(fp);
		return 0;
	}
	int lenName = strlen(pComp->name)+1;
	if (fwrite(pComp->name, sizeof(char), lenName, fp) != lenName)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (saveFlightBytesDetails(pComp->flightArr[i], fp) == 0) //flight method
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}
int loadCompanyBytesDetails(Company* pComp, const char* fileName)
{
	BYTE pBuffer[BINARY_BUFFER_SIZE_COMPANY];
	int lenName = 0;
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL)
		return 0;
	if (fread(pBuffer, sizeof(BYTE), BINARY_BUFFER_SIZE_COMPANY, fp) != BINARY_BUFFER_SIZE_COMPANY)
	{
		fclose(fp);
		return 0;
	}
	if (loadFromBufferCompanyBytesDetails(pBuffer, pComp,&lenName) == 0)
	{
		fclose(fp);
		return 0;
	}
	if (fread(pComp->name, sizeof(char), lenName, fp) != lenName)
	{
		fclose(fp);
		return 0;
	}
	
	if(pComp->flightCount>0)
		pComp->flightArr = (Flight**)malloc(pComp->flightCount*sizeof(Flight*)); 
	else
	{
		pComp->flightArr = NULL;
		fclose(fp);
		return 1;
	}
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)malloc(sizeof(Flight));
		if (!pComp->flightArr[i])
		{
			fclose(fp);
			return 0;
		}
		else
			if (loadFlightBytesDetails(pComp->flightArr[i], fp) == 0) //flight method
			{
				fclose(fp);
				return 0;
			}
	}
	fclose(fp);
	return 1;
}
void saveToBufferCompanyBytesDetails(const Company* pComp, BYTE* pBuffer)
{
	int nameLenght = strlen(pComp->name);
	pBuffer[0] = (pComp->flightCount << 7)   | ((pComp->sortOpt << 4) & 0x70 | (nameLenght & 0xF));
	pBuffer[1] = pComp->flightCount >> 1;
}

int loadFromBufferCompanyBytesDetails(BYTE* pBuffer, Company* pComp, int* nameLen)
{
	pComp->flightCount = pBuffer[0] >> 7 | (pBuffer[1] << 1);
	pComp->sortOpt = (pBuffer[0] >> 4) & 0xF;
	*nameLen = pBuffer[0] & 0xF;
	*nameLen = *nameLen + 1;
	pComp->name = (char*)malloc((*nameLen)*sizeof(char)); //we prepered name size
	if (pComp->name == NULL)
		return 0;
	return 1;
}