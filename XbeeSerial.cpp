// File：XbeeSerial.cpp
// Project：XbeeSerial3.0
// Brief：
// Author：张先炳
// Last Modified：2018/10/19  22:34
// Note：

#include "XbeeSerial.h"


void XbeeSerial::rcvCallback()
{
    for (const auto&it : rcvBuffer)
    {
        // 如果在上一个包未读完之前读到起始符，则丢弃先前的数据包并重新开始新数据包
        if (it == START_BYTE)
        {
            frameBuffer.clear();
            pos = 0;
            // TODO 抛出错误
        }
        frameBuffer.push_back(it);

        if (it == ESCAPE)
        {
            if (needEscape)
            {
                escape = true;
                continue;
            }
        }
        switch (pos)
        {
        case 1:
            {
                if (escape)
                {
                    escape = false;
                    if (it == (ESCAPE - 0x20) || it == (START_BYTE - 0x20)) msb = it + 0x20;
                    if (it == (XOFF + 0x20) || it == (XON + 0x20))msb = it - 0x20;
                }
                else
                    msb = it;
                break;
            }
        case 2:
            {
                if (escape)
                {
                    if (it == (ESCAPE - 0x20) || it == (START_BYTE - 0x20)) lsb = it + 0x20;
                    if (it == (XOFF + 0x20) || it == (XON + 0x20))lsb = it - 0x20;
                }
                else
                    lsb = it;
                frameLength = (uint16_t(msb) << 8) + uint16_t(lsb);
                break;
            }
        case 3:
            {
                apiID = it;
                break;
            }
        default:
            {
                //此位置为校验和的位置
                if (pos == frameLength + 3)
                {
                    //本帧到达尾部，结束

                    // 进行处理
                    handleFrameData();

                    //清空
                    frameBuffer.clear();
                    pos = 0;
                }
                break;
            }
        }

        pos++;
    }
}

void XbeeSerial::handleFrameData()
{
    switch (apiID)
    {
    case RX_16_RESPONSE:
        {
            rx16.getFrame(frameBuffer);
            displayVector((rx16.RFData));
            break;
        }
    case ZB_RX_RESPONSE:
        {
            rx.getFrame(frameBuffer);
            displayVector(rx.RFData);
            break;
        }
    default:
        {
            break;
        }
    }
}
