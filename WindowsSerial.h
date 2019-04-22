#pragma once
#include <iostream>
using namespace std;
#include <windows.h>
#include <vector>
#include <stdint.h>
#include   <functional>

typedef void (*serial_callback)(vector<uint8_t> &);

void default_serial_callback(vector<uint8_t> & vec);

class WindowsSerial{
public:
	WindowsSerial(long uPort, long uBandRate = 57600);
	~WindowsSerial();
	DWORD WINAPI open();
	DWORD WINAPI recv(vector<uint8_t> & vec);
	DWORD WINAPI send(vector<uint8_t> & vec);
	DWORD WINAPI recvloop(serial_callback callback = default_serial_callback);

	long m_uPort;
	long m_uBandRate;
	HANDLE hCom;
	COMMTIMEOUTS timeouts;
	DCB dcb;
	HANDLE hMutex;
	char buffer[1024];
    DWORD readsize,dwError; 
    BOOL bReadStatus;
    COMSTAT cs;
};

