// 文件名：main.cpp
// 工程：XbeeSerial
// 简介：
// 作者：张先炳
// 最后修改：2018年10月17日  8:52

#include <iostream>
#include "WindowsSerial.h"
#include "XbeeSerial.h"

using namespace std;
using namespace std::placeholders;

#define XBEE1_ADDR 0x0013A200410607E7
#define XBEE2_ADDR 0x0013A200410607CF

int main()
{
    XbeeSerial xbee(8,115200);

    // 发送消息包
    const std::vector<uint8_t>RFData_{ 0x11, 0x13, 0x7E, 0x7D, 0x56, 0x98 };
    //xbee.send(RFData_);
    xbee.send16(RFData_);

    while(1)
    {
        xbee.read();

    }



    
    std::cout << "主线程已结束" << std::endl;

    std::cin.get();
    return 0;
}
