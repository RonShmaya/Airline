#pragma once

#define CHECK_RETURN_0(ptr) { if(!ptr) return 0;} //1.	use 3 times minimom-> Company.c addFlight twice,AirportManager.c addAirport one
#define CHECK_RETURN_NULL(ptr) { if(!ptr) return NULL; } //2.	use 1 time minimom -> AirportManager.c createAirport one
#define CHECK_MSG_RETURN_0(ptr,msg) { if(!ptr){ printf("%s",msg); return 0;}}	//3.	use 3 times minimon -> AirportManager.c ( saveManagerToFile one
																				//		loadManagerFromFile one) ,Company.c loadCompanyFromFile one
//macros for file
#define CHECK_0_MSG_COLSE_FILE(x,file,msg){ if(x==0){  printf("%s",msg); fclose(file); return 0;}} //4.	use 1 time minimom -> filehelper writeStringToFile one
#define CHECK_NULL__MSG_COLSE_FILE(ptr,file,msg) {if(!ptr){  printf("%s",msg); fclose(file); return 0;}} //5. 	use 1 time minimom -> Company.c loadCompanyFromFile
																									//								one
#define MSG_CLOSE_RETURN_0(msg,file) { printf("%s",msg); fclose(file); return 0;} //6.	 use 3 times minimom -> Company.c loadCompanyFromFile one
																				//		AirportManager.c saveManagerToFile one	,loadManagerFromFile one	

// we use macro the miniom times with the directly place for easy check.

