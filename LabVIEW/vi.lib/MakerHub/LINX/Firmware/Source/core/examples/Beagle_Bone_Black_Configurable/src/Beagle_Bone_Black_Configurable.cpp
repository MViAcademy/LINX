#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h> 

#include "LinxDevice.h"
#include "LinxBeagleBone.h"
#include "LinxBeagleBoneBlack.h"
#include "LinxSerialListener.h"
#include "LinxLinuxTcpListener.h"

//Helper Functions
int parseInputTokens(LinxDevice* linxDev, int argc, char* argv[]);
void printUsage(char* argv[]);

#define NUMTOKENS 10
string tokens[NUMTOKENS] = {"-h", "-H", "--help", "--Help", "-v", "-V", "--version", "--Version", "-serial", "-tcp"};

int uartListenerPort = -1;
int tcpListenerPort = -1;


LinxBeagleBoneBlack* LinxDev;

int main(int argc, char* argv[])
{
	//Instantiate The LINX Device
	LinxDev = new LinxBeagleBoneBlack();
	
	if(parseInputTokens(LinxDev, argc, argv) >= 0)
	{	
		if(uartListenerPort > -1 && tcpListenerPort > -1)
		{
			while(1)
			{
				LinxSerialConnection.CheckForCommands();
				LinxTcpConnection.CheckForCommands();
			}
		}
		else if(uartListenerPort > -1)
		{
			while(1)
			{
				LinxSerialConnection.CheckForCommands();
			}
			
		}
		else if(tcpListenerPort > -1)
		{
			while(1)
			{
				LinxTcpConnection.CheckForCommands();
			}
		}
		else
		{
			cout << "No bus specified.\n";
			printUsage(argv);
			return -1;
		}
	}
	return 0;
}

int parseInputTokens(LinxDevice* linxDev, int argc, char* argv[])
{
	cout << "\n";
	
	//Loop Over All Tokens
	for(int i=1; i<argc; i++)
	{
		//cout << argv[i];
		
		bool match = false;
		for(int j=0; j<NUMTOKENS; j++)
		{
			if(tokens[j].compare(argv[i]) == 0)
			{
				//cout << " - match\n";
				match = true;
				
				switch(j)
				{
					case 0: 	//-h
					case 1: 	//-H
					case 2:	//--help
					case 3: 	//--Help
						printUsage(argv);
						return 0;
					case 4: 	//-v
					case 5: 	//-V
					case 6:	//--version
					case 7: 	//--Version
						cout << "\n"<< argv[0] << " version 2.1.0\n";
						return 0;
					case 8:	//-serial
						if(i+1 >= argc)
						{
							cout << "\n"<< "Missing port\n";
							printUsage(argv);
							return -1;
						}
						else
						{
							uartListenerPort = atoi(argv[i+1]);
							bool validPort = false;
							
							for(int i=0; i<linxDev->NumUartChans; i++)
							{
								if(uartListenerPort == linxDev->UartChans[i])
								{	
									LinxSerialConnection.Start(LinxDev, (unsigned char)uartListenerPort);
									validPort = true;
									break;
								}
							}
							if(!validPort)
							{
								//Invalid Port, Print Usage
								cout << "Invalid port.\n";
								printUsage(argv);
								return -1;
							}
							break;
						}
						break;
					case 9:	//-tcp
						if(i+1 >= argc)
						{
							cout << "\n"<< "Missing port\n";
							printUsage(argv);
							return -1;
						}
						else
						{
							tcpListenerPort = atoi(argv[i+1]);
							cout << "\n"<< "Listening on " << (unsigned short)tcpListenerPort << "\n";
							LinxTcpConnection.Start(LinxDev, 44300);
						}
						break;
					default:
						break;
				}
			}
		}
		if(!match)
		{
			//Either Argument or invalid token
		}
		
	}
	return 0;
}

void printUsage(char* argv[])
{
	cout << "\nusage: " << argv[0] << " -serial [port]\n";
	cout << "   or: " << argv[0] << " -serial [port] -tcp [port]\n";
	cout << "   or: " << argv[0] << " -tcp [port]\n\n";
	cout << "Available options are:\n";
	cout << "  -serial\t 0, 1, or 4\n";
	cout << "  -tcp  \t Any valid, unused TCP port. (ex 44300)\n\n";
}