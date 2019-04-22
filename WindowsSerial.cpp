#pragma once
#include "WindowsSerial.h"

WindowsSerial::WindowsSerial(long uPort, long uBandRate)
{
	this->m_uPort = uPort;
	this->m_uBandRate = uBandRate;
	this->hCom = NULL;
    open();
}

WindowsSerial::~WindowsSerial()
{
	if (this->hCom != NULL)
		CloseHandle(this->hCom);
}

DWORD WINAPI WindowsSerial::open()
{
	char aPort[10];
	sprintf_s(aPort, "\\\\.\\COM%d", this->m_uPort);
    hCom = CreateFileA(aPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);

    GetCommTimeouts(hCom, &timeouts);
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    if (!SetCommTimeouts(hCom, &timeouts))
    {
        cout << "set comm timeouts failed." << endl;
        CloseHandle(hCom);
        return 0;
    }
	
	SetupComm(hCom, 1024, 1024);

    GetCommState(hCom, &dcb);
    dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = this->m_uBandRate;
    dcb.Parity = 0;  
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;   
    if (!SetCommState(hCom, &dcb))
    {
        cout << "set comm state failed." << endl;
        CloseHandle(hCom);
        return 0;
    }

	PurgeComm(hCom, PURGE_RXCLEAR|PURGE_TXCLEAR);
	ClearCommError(hCom, &dwError, &cs);

	return 1;
}

DWORD WINAPI WindowsSerial::recvloop(serial_callback callback)
{
	while(true)
    {   
        readsize = 0;
        ClearCommError(hCom, &dwError, &cs);

        if(cs.cbInQue > sizeof(buffer))
        {
			cout << "comm receive more than buffer!" << endl;
            PurgeComm(hCom,PURGE_RXCLEAR);
            return 0;
        }
        memset(buffer, 0 , sizeof(buffer));
        bReadStatus = ReadFile(hCom, buffer, cs.cbInQue, &readsize, 0);
        if(readsize > 0)
        {
			vector<uint8_t> recvVec;
			for (DWORD i = 0; i < cs.cbInQue; i++)
			{
				recvVec.push_back((uint8_t)buffer[i]);
			}
			callback(recvVec);
        }
        Sleep(1);
    }
}

DWORD WINAPI WindowsSerial::recv(vector<uint8_t> & vec)
{
	vec.clear();

	readsize = 0;
    ClearCommError(hCom, &dwError, &cs);
    if(cs.cbInQue > sizeof(buffer))
    {
		cout << "comm receive more than buffer!"  << cs.cbInQue << ":" << sizeof(buffer) << endl;
        PurgeComm(hCom,PURGE_RXCLEAR);
        return 0;
    }

    memset(buffer, 0 , sizeof(buffer));
    bReadStatus = ReadFile(hCom, buffer, cs.cbInQue, &readsize, 0);
    if(readsize > 0)
    {
		for (DWORD i = 0; i < cs.cbInQue; i++)
		{
			vec.push_back((uint8_t)buffer[i]);
		}
		return vec.size();
    }
	else{
		return 0;
	}
}

DWORD WINAPI WindowsSerial::send(vector<uint8_t> & vec)
{
	char *msg = new char[vec.size()];
	for (DWORD i = 0; i < vec.size(); i++){
		msg[i] = (char)vec[i];
	}

    DWORD dwBytesWritten = 0;
    if(WriteFile(hCom,msg,vec.size(),&dwBytesWritten,NULL)){
		return dwBytesWritten;
	}else{
		return 0;
	}
}

void default_serial_callback(vector<uint8_t>  & vec){
	cout << "msgsize:" << vec.size() << " ||| content:";
	for(DWORD i = 0; i < vec.size(); i++){
		printf("0x%x ", vec[i]);
	}
	cout << endl;
}