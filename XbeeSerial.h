// File：XbeeSerial.h
// Project：XbeeSerial3.0
// Brief：
// Author：张先炳
// Last Modified：2018/10/19  22:34
// Note：

#pragma once
#include "WindowsSerial.h"
#include "XbeeFrame.hpp"
#include "Utility.hpp"

class XbeeSerial:public WindowsSerial
{
public:
    ZBRxResponse rx;
    ZBTxRequest tx;
    Rx16Response rx16;
    Tx16Request tx16;

    /// 用于读帧
    std::vector<uint8_t> frameBuffer;
    std::vector<uint8_t> rcvBuffer;
    uint16_t frameLength;
    bool escape = false;
    uint16_t pos;
    uint8_t msb;
    uint8_t lsb;
    uint8_t apiID;
    bool complete;

    bool needEscape = false;

    /// 用于发送消息帧
    //广播方式发送消息报，（有效负载RFData_需预先设好）
    void send()
    {
        WindowsSerial::send(tx.frame_e);
    }

    //以默认广播方式发送带有效负载RFData_的消息包
    void send(std::vector<uint8_t> RFData_)
    {
        tx.setFrame(RFData_);
        WindowsSerial::send(tx.frame_e);
    }

    void send16()
    {
        WindowsSerial::send(tx16.frame_e);
    }

    void send16(std::vector<uint8_t> RFData_)
    {
        tx16.setFrame(RFData_);
        WindowsSerial::send(tx16.frame_e);
    }

    void read()
    {
        recv(rcvBuffer);
        rcvCallback();
    }

public:
    XbeeSerial(): WindowsSerial(8, 115200)
    {
    }

    XbeeSerial(long uPort, long uBandRate): WindowsSerial(uPort, uBandRate)
    {
    }

    void rcvCallback();
    void handleFrameData();
};
