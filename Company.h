#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"
#include "fileHelper.h"

typedef enum { eNone, eHour, eDate, eSorceCode, eDestCode, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];
#define DETAIL_PRINT //if we want print all define, else not define

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	eSortOption	sortOpt;
	LIST		flighDateList;
}Company;

int		initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName);
void	initCompany(Company* pComp, AirportManager* pManaer);
void	initDateList(Company* pComp);
int		isUniqueDate(const Company* pComp, int index);
int		addFlight(Company* pComp, const AirportManager* pManager);

void	printCompany(Company* pComp, char* firstName, ...); // change to variadic
void	printFlightsCount(const Company* pComp);

int		saveCompanyToFile(const Company* pComp, const char* fileName);
int		loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName);

void	sortFlight(Company* pComp);
void	findFlight(const Company* pComp);
eSortOption showSortMenu();

void	freeCompany(Company* pComp);

//bytes file methods
int saveCompanyBytesDetails(const Company* pComp, const char* fileName);
int loadCompanyBytesDetails(Company* pComp, const char* fileName);

void saveToBufferCompanyBytesDetails(const Company* pComp, BYTE* pBuffer);
int loadFromBufferCompanyBytesDetails(BYTE* pBuffer, Company* pComp, int* nameLen);
#endif

