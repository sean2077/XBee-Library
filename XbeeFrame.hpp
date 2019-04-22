// File：XbeeFrame.hpp
// Project：XbeeSerial3.0
// Brief：
// Author：张先炳
// Last Modified：2018/10/19  22:34
// Note：

#pragma once
#include <vector>
#include <numeric>


/** @note 序号若不特别说明，均默认从0开始计数
 *  *@note TODO 为了方便，暂时全设为public 后期再完善
 */


// 支持两个系列：1系列（802.15.4）和2系列（ZB Pro / ZNet），以下不带ZB前缀的代表1系列，带ZB前缀的代表2系列
#define SERIES_1
#define SERIES_2

// 需要转义的字符
#define START_BYTE 0x7E
#define ESCAPE 0x7D
#define XON 0x11
#define XOFF 0x13

//此值确定用于接收RX数据包的字节数组的大小, 如果RX数据包超过此大小，则无法解析！
#define MAX_FRAME_DATA_SIZE 110
#define MAX_FRAME_SIZE 256

//广播地址
#define BROADCAST_ADDRESS 0xFFFF
#define ZB_BROADCAST_ADDRESS 0xFFFE

//64位地址
#define COORDINATOR_ADDRESS_64 0x0000000000000000
#define BROADCAST_ADDRESS_64      0x000000000000FFFF
#define UNKNOWN_ADDRESS_64         0xFFFFFFFFFFFFFFFF

// 帧数据中固定不变的字节长度（不包括帧id，帧类型，以及变数据）
#define ZB_TX_API_LENGTH 12
#define ZB_EXPLICIT_TX_API_LENGTH 18
#define TX_16_API_LENGTH 3
#define TX_64_API_LENGTH 9
#define AT_COMMAND_API_LENGTH 2
#define REMOTE_AT_COMMAND_API_LENGTH 13
#define PACKET_OVERHEAD_LENGTH 6

//帧类型在包中的位置（从0开始计数）
#define API_ID_INDEX 3

// RSSI在帧数据中的位置
#define RX_16_RSSI_OFFSET 2
#define RX_64_RSSI_OFFSET 8

// 帧ID
#define FRAME_ID_DEFAULT 0x01
#define FRAME_ID_NO_RESPONSE 0x00

//默认广播半径，为0表示最大广播节点
#define ZB_BROADCAST_RADIUS_MAX_HOPS 0x00

//ZB模块（系列2）Tx请求中常用参数
#define DEFAULT_ENDPOINT 232
#define DEFAULT_CLUSTER_ID 0x0011
#define DEFAULT_PROFILE_ID 0xc105
#define ACK_OPTION 0
#define DISABLE_ACK_OPTION 1
#define BROADCAST_OPTION 4
//TX 选项
#define TX_OPTION_DISABLE_ACK 0x01
#define TX_OPTION_BROADCAST 0x04
//RX选项
#define ZB_PACKET_ACKNOWLEDGED 0x01 //数据包已确认
#define ZB_BROADCAST_PACKET 0x02   // 数据包是广播包
#define ZB_APS_ENCRYPTION 0x20   //APS加密包

/**  @帧类型：*/
/// 后缀为REQUEST的表示PC发给Xbee的指令（请求）帧
#define TX_64_REQUEST 0x00
#define TX_16_REQUEST 0x01
#define AT_COMMAND_REQUEST 0x08
#define AT_COMMAND_QUEUE_REQUEST 0x09
#define ZB_TX_REQUEST 0x10
#define ZB_EXPLICIT_TX_REQUEST 0x11
#define REMOTE_AT_REQUEST 0x17
/// 后缀为RESPONSE的表示Xbee发给PC的数据包或状态包
#define RX_64_RESPONSE 0x80
#define RX_16_RESPONSE 0x81
#define RX_64_IO_RESPONSE 0x82
#define RX_16_IO_RESPONSE 0x83
#define AT_COMMAND_RESPONSE 0x88
#define TX_STATUS_RESPONSE 0x89
#define MODEM_STATUS_RESPONSE 0x8A
#define ROUTE_INFO_RESPONSE 0x8D
#define AGGREGATE_ADDR_UPDATE_RESPONSE 0x8E
#define ZB_RX_RESPONSE 0x90
#define ZB_EXPLICIT_RX_RESPONSE 0x91
#define ZB_TX_STATUS_RESPONSE 0x8B
#define ZB_IO_SAMPLE_RESPONSE 0x92
#define ZB_IO_NODE_IDENTIFIER_RESPONSE 0x95
#define REMOTE_AT_COMMAND_RESPONSE 0x97

// 发送状态
#define	SUCCESS 0x0
#define CCA_FAILURE 0x2
#define INVALID_DESTINATION_ENDPOINT_SUCCESS 0x15
#define	NETWORK_ACK_FAILURE 0x21
#define NOT_JOINED_TO_NETWORK 0x22
#define	SELF_ADDRESSED 0x23
#define ADDRESS_NOT_FOUND 0x24
#define ROUTE_NOT_FOUND 0x25
#define PAYLOAD_TOO_LARGE 0x74
#define XBEE_WAIT_TIMEOUT 0xFF

// Modem状态
#define HARDWARE_RESET 0
#define WATCHDOG_TIMER_RESET 1
#define ASSOCIATED 2
#define DISASSOCIATED 3
#define SYNCHRONIZATION_LOST 4
#define COORDINATOR_REALIGNMENT 5
#define COORDINATOR_STARTED 6

#define ZB_TX_UNICAST 0
#define ZB_TX_BROADCAST 8

#define AT_OK 0
#define AT_ERROR  1
#define AT_INVALID_COMMAND 2
#define AT_INVALID_PARAMETER 3
#define AT_NO_RESPONSE 4

// 指示错误的类型
#define NO_ERR 0        //为了区别于win32API 里的宏NO_ERROR，命名为NO_ERR
#define CHECKSUM_FAILURE 1
#define PACKET_EXCEEDS_BYTE_ARRAY_LENGTH 2
#define UNEXPECTED_START_BYTE 3

/** @note Digi中的概念：
 *@帧数据  除帧头、帧长度以及校验和外的其他字段。
 *@帧长度  帧数据段的长度。
 *@校验和  0xff-帧数据段字节之和
 */

class XbeeFrame
{
public:
    /// 完整的帧（读到的帧，或者要发送的帧）存储在此
    std::vector<uint8_t> frame; //原始帧
    std::vector<uint8_t> frame_e; //转义帧
    /// 完整的Xbee帧包括以下 5个部分：起始符，帧长度，帧类型，帧数据，校验和
    static const uint8_t start = START_BYTE; //起始符
    uint16_t length = 0; //帧长度（原始帧中frameType+frameData部分的总字节数）
    uint8_t frameType = 0; //帧类型
    std::vector<uint8_t> frameData; //帧数据：帧类型之后、校验和之前的数据，注意这里与Digi的定义不同，Digi的帧数据=frameType+frameData
    uint8_t checksum = 0; //校验和
    /// 其他
    bool needEscape = false; //是否需要转义(默认API 1模式，即不转义）
    uint8_t errorCode = NO_ERR; //错误代码：NO_ERR、CHECKSUM_FAILURE、PACKET_EXCEEDS_BYTE_ARRAY_LENGTH、UNEXPECTED_START_BYTE
    bool complete = false; //帧是否准备完毕（要发送的帧：通过了计算校验和和转义；接收的帧：通过了转回和校验）

    XbeeFrame()
    {
        frame.reserve(MAX_FRAME_SIZE);
        frame_e.reserve(MAX_FRAME_SIZE);
        frameData.reserve(MAX_FRAME_DATA_SIZE);
    }

    virtual ~XbeeFrame()
    {
    }
};


/**
 * @brief 此为Xbee请求帧的基类，实现的功能主要是生成帧，根据给定指令内容和需要发送的数据包，生成完整的指令帧
 * @note 请求帧是指PC发给Xbee的帧，告诉Xbee要做什么，如发送数据包指令，AT指令等
 */
class XbeeRequest:public XbeeFrame
{
public:
    XbeeRequest()
    {
    }

    XbeeRequest(uint8_t frameType_, std::vector<uint8_t>&frameData_)
    {
        frameData.reserve(MAX_FRAME_SIZE);
        frame.reserve(MAX_FRAME_SIZE);
        frame_e.reserve(MAX_FRAME_SIZE);

        setFrame(frameType_, frameData_);
    }

    void setFrame(uint8_t frameType_, std::vector<uint8_t>&frameData_)
    {
        //给定帧类型
        frameType = frameType_;
        //给定帧数据
        frameData = frameData_;
        generateFrame();
    }

    /// 生成帧。给定：frameType和frameData，需填充：length，checksum，frame，frame_e
    /** @note 调用该函数之前必须给定frameType和frameData*/
    void generateFrame()
    {
        //计算帧长度和校验和
        computeLength();
        computeChecksum();

        //填充到frame
        frame.clear();
        frame.push_back(start);
        frame.push_back(uint8_t(length >> 8));
        frame.push_back(uint8_t(length));
        frame.push_back(frameType);
        frame.insert(frame.end(), frameData.begin(), frameData.end());
        frame.push_back(checksum);

        //将frame转义并存到frame_e
        frame_e.clear();
        escapeFrame();
    }

    // 计算帧长度length
    void computeLength()
    {
        length = uint16_t(frameData.size() + 1);
    }

    //计算校验和      注意：不能在计算校验和之前进行转义
    void computeChecksum()
    {
        checksum = 0xff - ((accumulate(frameData.begin(), frameData.end(), frameType)) & 0xff);
    };

    //将原始帧进行转义，填充到frame_e       注意：除了起始字段外，其他均需转义
    void escapeFrame()
    {
        frame_e = frame;
        if (needEscape)
        {
            for (uint8_t i = 1; i < frame_e.size(); i++)
            {
                if (frame_e[i] == ESCAPE || frame_e[i] == START_BYTE)
                {
                    frame_e.insert(frame_e.begin() + i, ESCAPE);
                    frame_e[++i] -= 0x20;
                }
                else if (frame_e[i] == XOFF || frame_e[i] == XON)
                {
                    frame_e.insert(frame_e.begin() + i, ESCAPE);
                    frame_e[++i] += 0x20;
                }
            }
        }
    }
};

/**
 * @brief 此为Xbee响应帧的基类，实现的功能主要是解析帧，填充帧数据里的每个具体内容
 * @note 响应帧是指Xbee发给PC的帧，包括返回状态帧，发送数据包帧等
 */
class XbeeResponse:public XbeeFrame
{
public:
    XbeeResponse()
    {
    }

    XbeeResponse(std::vector<uint8_t>&frame_e_)
    {
        XbeeResponse::getFrame(frame_e_);
    }

    // 给定转义帧，将各字段填充完整
    virtual void getFrame(std::vector<uint8_t>&frame_e_)
    {
        frame_e = frame_e_;
        parseFrame();
    }

    /// 初步解析并验证校验和。给定：frame_e，需填充：frame，length，frameType，frameData，checksum
    /** @note  在调用该函数之前必须将要解析的帧存到frame_e里*/
    void parseFrame()
    {
        //将frame_e转回并存到frame
        frame.clear();
        unescapeFrame();

        //根据帧类型分别读取各部分数据
        length = uint16_t(frame[1]) << 8 + uint16_t(frame[2]);
        frameType = frame[3];
        frameData.clear();
        frameData.insert(frameData.end(), frame.begin() + 4, frame.end() - 1);
        checksum = frame.back();

        //验证校验和
        verifyChecksum();
    }

    //将转义帧转回，填充到frame
    void unescapeFrame()
    {
        frame = frame_e;
        if (needEscape)
        {
            //注意：除了起始字段外，其他均需转回
            for (uint8_t pos = 1; pos < frame.size(); pos++)
            {
                if (frame[pos] == ESCAPE)
                {
                    frame.erase(frame.begin() + pos);
                    if (frame[pos] == (ESCAPE - 0x20) || frame[pos] == (START_BYTE - 0x20))
                    {
                        frame[pos] += 0x20;
                    }
                    else if (frame[pos] == (XOFF + 0x20) || frame[pos] == (XON + 0x20))
                    {
                        frame[pos] -= 0x20;
                    }
                }
            }
        }
    }

    //验证校验和 
    void verifyChecksum()
    {
        if ((accumulate(frame.begin() + 3, frame.end(), 0) & 0xff) == 0xff) //帧前面三个字符分别代表起始符，帧长度高位，帧长度低位，因此从第四个开始计算
        {
            errorCode = NO_ERR;
            complete = true;
        }
        else
        {
            errorCode = CHECKSUM_FAILURE;
            complete = false;
        }
    }
};

// 帧类型：TX_16_REQUEST 0x01
class Tx16Request:public XbeeRequest
{
public:
    uint8_t frameID;
    uint16_t destAddr16;
    uint8_t option;
    std::vector<uint8_t> RFData;

    Tx16Request()
    {
    }

    Tx16Request(std::vector<uint8_t>&RFData_, uint8_t frameID_ = FRAME_ID_NO_RESPONSE,
                uint16_t destAddr16_ = BROADCAST_ADDRESS, uint8_t option_ = 0)
    {
        setFrame(RFData_, frameID_, destAddr16_, option_);
    }

    void setFrame(std::vector<uint8_t>&RFData_, uint8_t frameID_ = FRAME_ID_NO_RESPONSE,
                  uint16_t destAddr16_ = BROADCAST_ADDRESS, uint8_t option_ = 0)
    {
        frameType = TX_16_REQUEST;
        frameID = frameID_;
        destAddr16 = destAddr16_;
        option = option_;
        RFData = RFData_;
        generateFrameData();
        generateFrame();
    }

    void generateFrameData()
    {
        frameData.clear();
        frameData.push_back(frameID);
        frameData.push_back(uint8_t(destAddr16 >> 8));
        frameData.push_back(uint8_t(destAddr16));
        frameData.push_back(option);
        frameData.insert(frameData.end(), RFData.begin(), RFData.end());
    }
};

// 帧类型：RX_16_RESPONSE 0x81
class Rx16Response:public XbeeResponse
{
public:
    uint16_t sourceAddr16;
    uint8_t rssi;
    uint8_t option;
    std::vector<uint8_t> RFData;

    Rx16Response()
    {
    }

    Rx16Response(std::vector<uint8_t>&frame_e_)
    {
        Rx16Response::getFrame(frame_e_);
    }

    void getFrame(std::vector<uint8_t>&frame_e_) override
    {
        XbeeResponse::getFrame(frame_e_);
        parseFrameData();
    }

    void parseFrameData()
    {
        sourceAddr16 = (uint16_t(frameData[0]) << 8) + uint16_t(frameData[1]);
        rssi = frameData[2];
        option = frameData[3];
        RFData.clear();
        RFData.insert(RFData.end(), frameData.begin() + 4, frameData.end());
    }
};

// 帧类型：ZB_TX_REQUEST 0x10
class ZBTxRequest:public XbeeRequest
{
public:
    uint8_t frameID;
    uint64_t destAddr64;
    uint16_t destAddr16;
    uint8_t broadcastRadius;
    uint8_t option;
    std::vector<uint8_t> RFData;

    ZBTxRequest()
    {
    }

    ZBTxRequest(std::vector<uint8_t>&RFData_, uint8_t frameID_ = FRAME_ID_DEFAULT,
                uint64_t destAddr64_ = BROADCAST_ADDRESS_64,
                uint16_t destAddr16_ = ZB_BROADCAST_ADDRESS, uint8_t broadcastRadius_ = ZB_BROADCAST_RADIUS_MAX_HOPS,
                uint8_t option_ = 0)
    {
        setFrame(RFData_, frameID_, destAddr64_, destAddr16_, broadcastRadius_, option_);
    }

    void setFrame(std::vector<uint8_t>&RFData_, uint8_t frameID_ = FRAME_ID_DEFAULT,
                  uint64_t destAddr64_ = BROADCAST_ADDRESS_64,
                  uint16_t destAddr16_ = ZB_BROADCAST_ADDRESS, uint8_t broadcastRadius_ = ZB_BROADCAST_RADIUS_MAX_HOPS,
                  uint8_t option_ = 0)
    {
        frameType = ZB_TX_REQUEST;
        frameID = frameID_;
        destAddr64 = destAddr64_;
        destAddr16 = destAddr16_;
        broadcastRadius = broadcastRadius_;
        option = option_;
        RFData = RFData_;
        generateFrameData();
        generateFrame();
    }

    void generateFrameData()
    {
        frameData.clear();
        frameData.push_back(frameID);
        frameData.push_back(uint8_t(destAddr64 >> 56));
        frameData.push_back(uint8_t(destAddr64 >> 48));
        frameData.push_back(uint8_t(destAddr64 >> 40));
        frameData.push_back(uint8_t(destAddr64 >> 32));
        frameData.push_back(uint8_t(destAddr64 >> 24));
        frameData.push_back(uint8_t(destAddr64 >> 16));
        frameData.push_back(uint8_t(destAddr64 >> 8));
        frameData.push_back(uint8_t(destAddr64));
        frameData.push_back(uint8_t(destAddr16 >> 8));
        frameData.push_back(uint8_t(destAddr16));
        frameData.push_back(broadcastRadius);
        frameData.push_back(option);
        frameData.insert(frameData.end(), RFData.begin(), RFData.end());
    }
};

// 帧类型：ZB_RX_RESPONSE 0x90
class ZBRxResponse:public XbeeResponse
{
public:
    uint64_t sourceAddr64;
    uint16_t sourceAddr16;
    uint8_t option;
    std::vector<uint8_t> RFData;

    ZBRxResponse()
    {
    }

    ZBRxResponse(std::vector<uint8_t>&frame_e_)
    {
        ZBRxResponse::getFrame(frame_e_);
    }

    void getFrame(std::vector<uint8_t>&frame_e_) override
    {
        XbeeResponse::getFrame(frame_e_);
        parseFrameData();
    }

    void parseFrameData()
    {
        sourceAddr64 = (uint64_t(frameData[0]) << 56) + (uint64_t(frameData[1]) << 48) + (uint64_t(frameData[2]) << 40)
            + (uint64_t(frameData[3]) << 32) + (uint64_t(frameData[4]) << 24) + (uint64_t(frameData[5]) << 16) + (
                uint64_t(frameData[6]) << 8) + uint64_t(frameData[7]);
        sourceAddr16 = (uint16_t(frameData[8]) << 8) + uint16_t(frameData[9]);
        option = frameData[10];
        RFData.clear();
        RFData.insert(RFData.end(), frameData.begin() + 11, frameData.end());
    }
};
