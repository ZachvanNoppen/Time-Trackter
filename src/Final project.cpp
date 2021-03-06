// FinalProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <comdef.h>
#include <wchar.h>
#include <algorithm>
#include <stdio.h>
//clocks
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <stdlib.h>

using namespace std;
//#include <iostream>
//#include <algorithm>
//#include <string.h> 

//Structure Definitions
struct Entry {
	WCHAR name[200] = TEXT("Placeholder");
	int time = 0;

}e1, e2, e3, e4, e5;
struct ProcessData
{
	WCHAR name[100];
};

//Variable Definitions
ProcessData processArray[500];
Entry eArray[5] = { e1,e2,e3,e4,e5 };
int numProc = 0;

//  Forward declarations:
BOOL GetProcessList();
BOOL ListProcessModules(DWORD dwPID);
BOOL ListProcessThreads(DWORD dwOwnerPID);
void printError(TCHAR* msg);
void WriteData();
void ReadData();
void ShowTracking();
void Run();

int main(void)
{

	ReadData();

	//Print Testing
	for (int u = 0; u < 5; u++) {
		wprintf(L"Array %ls\n", eArray[u].name);
	}

	int userIn = 0;
	int x;
	WCHAR track[200];
	printf("Welcome to the Application Tracker! Track any program on your computer!\n");
	//read from file "You are tracking x @ x mins
	while (userIn != -1) {
		printf("Press 1: Track a new application\nPress 2: Stop tracking an application\nPress 3: Show tracking stats\nPress 4: Run the tracker\nPress 0: Quit.\n");
		scanf_s("%d", &userIn);
		if (userIn == 0) {
			printf("Exiting the program");
			userIn = -1;
		}
		else if (userIn == 1) {
			wprintf(L"Application Tracker:\nEnter the name of the executable file you would like to track.\nFor example, to track your browser usage on Firefox, submit the execuable 'firefox.exe'.\n");
			wprintf(L"Enter the file name you would like tracked:");
			wscanf_s(L"%ls", track, 200);
			int p = 0;
			for (p = 0; p < 5; p++) {
				if (wcscmp(eArray[p].name, TEXT("Placeholder")) == 0) {
					wcscpy_s(eArray[p].name, 200, track);
					printf("hi%ls", track);
					//leaving the loop so it doesn't track more than once
					break;
				}
				else {
					//move on
					continue;
				}

				//Testing to see if the array is full
				if (p == 5) {
					printf("You have reached the maximum number of items to track. \nPlease delete and item to make room before adding another.");
				}

			}
			wprintf(L"Your program has been tracked!\n");
		}
		else if (userIn == 2) {
			printf("Application Tracker:\nEnter the name of the executable file you would like to remove./nFor example, to stop tracking your browser usage on Firefox, submit the execuable 'firefox.exe'.");
			BOOL broken = false;
			while (broken != true) {
				printf("Enter the file name you would like to stop tracking:");
				wscanf_s(L"%ls", track, 200);
				for (x = 0; x < 5; x++) {
					//Testing
					wprintf(L"Array %ls\nParse %ws\n", eArray[x].name, (WCHAR*)track);
					if (wcscmp(eArray[x].name, track) == 0) {
						//eArray[x].name = (WCHAR*)"Placeholder";
						wcscpy_s(eArray[x].name, 200, TEXT("Placeholder"));
						eArray[x].time = 0;
						wprintf(L"It worked %ls", eArray[x].name);
						break;
					}
					else {
						//move on
						continue;
					}
					wprintf(L"\nTracking a%ls \n 2%ls", eArray[x].name, (WCHAR*)track);
				}
				//after breaking out
				if (x == 5) {
					printf("There is no file with that name. Please make sure you have spelled it correctly.");
				}
				else {
					printf("Your program has been removed!\n");
					broken = true;
				}
			}
			for (int u = 0; u < 5; u++) {
				wprintf(L"Array %ls\n", eArray[u].name);
			}

		}
		else if (userIn == 3) {
			printf("Tracking:\n");
			ShowTracking();
		}
		else if (userIn == 4) {
			printf("STARTING TRACKING\n");
			Run();
		}
		else {
			printf("Please enter a valid number.\nPress 1 to track an application, press 2 to stop tracking an application, press 3 to show tracking stats, press 0 to quit.\n");
			scanf_s("%d", &userIn);
		}
	}

	GetProcessList();
	return 0;
}


void ShowTracking() {
	printf("You have currently logged the following:\n-------------\n");
	for (int n = 0; n < 5; n++) {
		if (wcscmp(eArray[n].name, TEXT("Placeholder")) != 0) {
			wprintf(L"Program: %ls\n", eArray[n].name);
			printf("Time: %d\n", eArray[n].time);
		}
		else {
			//move on
			continue;
		}
		printf("-------------\n");
	}
}

void ReadData() {
	//Setting an array with a maximum of 5 entries
	FILE* fp;
	WCHAR fpName[200];
	int fpTime = 0;
	int exit = 0;
	int i = 0;

	fopen_s(&fp, "datalog.txt", "r");
	//use files only if opened successfully

	if (fp != NULL)
	{
		while (exit != 1) {
			fscanf_s(fp, "%ls", fpName, 200);
			fscanf_s(fp, "%d", &fpTime, 200);
			fscanf_s(fp, "%d", &exit, 10);
			wcscpy_s(eArray[i].name, 200, fpName);
			eArray[i].time = fpTime;
			i++;
		}
		fclose(fp);
	}
}

void WriteData() {
	FILE* fp;

	fopen_s(&fp, "datalog.txt", "w");
	//use files only if opened successfully
	if (fp != NULL)
	{
		for (int i = 0; i < 5; i++) {

			fwrite(eArray[i].name, sizeof(WCHAR), wcslen(eArray[i].name), fp);
			fprintf(fp, "\n%d\n", eArray[i].time);

			if (i != 4) {

				if (wcscmp(eArray[i + 1].name, TEXT("Placeholder")) == 0) {
					//printing the 1 so we know to end
					fprintf(fp, "%ls\n", "1");
					break;
				}
				else {
					fprintf(fp, "%ls\n", "0");
				}
			}
			else {
				fprintf(fp, "%ls\n", "1");
			}
		}
		//TEst Print 
		//wprintf(L"data from file is %ls\n%d\n", eArray[i].name, eArray[i].time);
		fclose(fp);
	}
}

void Run() {
	chrono::steady_clock::time_point initC = chrono::steady_clock::now();
	chrono::steady_clock::time_point clockStart[5] = { initC,initC,initC,initC,initC };
	chrono::steady_clock::time_point clockEnd[5] = { initC,initC,initC,initC,initC };
	chrono::steady_clock::duration dur[5];
	WCHAR nameArr[5][200] = { TEXT(" "),TEXT(" "),TEXT(" "),TEXT(" "),TEXT(" ") };
	int nameIndex = 0;
	int items = 0;

	GetProcessList();
	//CHecking if there is a match
	for (int n = 0; n < 5; n++) {
		for (int y = 0; y < numProc; y++) {
			if (wcscmp(eArray[n].name, processArray[y].name) == 0) {
				//START TIMER
				wprintf(L"THis is the match %ls\n", processArray[y].name);
				clockStart[n] = chrono::steady_clock::now();
				items++;
				//nameArr[nameIndex][200] = (WCHAR)eArray[n].name;
				wcscpy_s(nameArr[nameIndex], 200, eArray[n].name);
				nameIndex++;
			}
		}

	}
	if (items == 0) {
		printf("None of the programs specified are listed. Please open the programs before tracking.\n");
	}
	else {
		BOOL end = false;
		while (end == false) {

			//PAUSING FOR 30 SECONDS
			std::this_thread::sleep_for(std::chrono::seconds(2));
			wprintf(L"Paused %d\n", items);
			//CHECK IF PROGRAM HAS BEEN TERMINATED
			GetProcessList();
			//for (int y = 0; y < numProc; y++) {
			//	printf_s("%ws\n", processArray[y].name);
			//}
			int itemC = items;
			for (int n = 0; n < itemC; n++) {
				BOOL isDone = true;
				if (wcscmp(nameArr[n], TEXT(" ")) != 0) {
					for (int y = 0; y < numProc; y++) {
						//print testing
						//wprintf(L"nt: %ls\n", nameArr[n]);
						if (wcscmp(nameArr[n], processArray[y].name) == 0) {
							wprintf(L"This is the match2 %ls\n\n\n", processArray[y].name);
							isDone = false;
							break;
						}
					}
				}
				else {
					isDone = true;
				}

				if (isDone == true) {
					clockEnd[n] = chrono::steady_clock::now();
					if (items != 0) {
						items--;
						wcscpy_s(nameArr[n], 200, TEXT(" "));

						for (int g = 0; g < 5; g++) {
							wprintf(L"ant: %ls\n", nameArr[g]);
						}
					}
					printf("THe item is gone\n %d\n", items);
				}
			}

			if (items == 0) {
				//Leave program
				printf("All programs are closed. Saving data\n");
				end = true;
				//Save in the writing data function
			}
		}
		//CACL DURATION
		for (int c = 0; c < 5; c++)
			if (clockStart[c] != initC) {
				dur[c] = clockEnd[c] - clockStart[c];
				//Using Cout for ease. Printf is hard to format with these wdata types
				cout << " The program has been running for\n" << chrono::duration_cast<std::chrono::seconds>(dur[c]).count() << " seconds\n";
				eArray[c].time += chrono::duration_cast<std::chrono::seconds>(dur[c]).count();
				WriteData();
			}
	}

}


BOOL GetProcessList()
{
	numProc = 0;
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		//printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		// printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		//Making a copy of the data in pe32.szExeFile
		wcscpy_s(processArray[numProc].name, 100, pe32.szExeFile);
		numProc++;
	} while (Process32Next(hProcessSnap, &pe32));

	/*TEST printing out the data
	for (int y = 0; y < numProc; y++) {
	printf_s("%ws\n", processArray[y].name);
	}*/
	CloseHandle(hProcessSnap);
	return(TRUE);
}




void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}		//liz rocks


