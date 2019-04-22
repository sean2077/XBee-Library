// 文件名：Utility.hpp
// 工程：XbeeSerial
// 简介：
// 作者：张先炳
// 最后修改：2018年10月17日  0:34

#pragma once
#include <vector>
#include <iostream>

template <typename T>
void displayVector(std::vector<T> vec)
{
    uint8_t*p = new uint8_t[vec.size()];
    if(!vec.empty())    memcpy(p, &vec[0], vec.size() * sizeof(T));
    std::cout << "frameBuffer：";
    for (auto i = 0; i < vec.size(); i++) printf("%02x ", p[i]);
    std::cout << std::endl;
    delete[]p;
}

