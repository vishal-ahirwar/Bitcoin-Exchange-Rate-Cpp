#define NOMINMAX

#ifndef BITCOIN_H
#define BITCOIN_H
//Copyright(c)2022 Vishal Ahirwar.
#include <iostream>
#include <string>
#include <cstdio>
#include "curlHandler.h"
#include<nlohmann/json.hpp>

class BitCoin
{
    using json=nlohmann::json;
public:
    json fetch();
    BitCoin();
private:
    constexpr static const char*const URL="https://blockchain.info/ticker";
    CurlHandler curlHandle;
protected:
};
#endif // BITCOIN_H