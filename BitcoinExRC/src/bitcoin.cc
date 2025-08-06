//Copyright(c)2022 Vishal Ahirwar.
#include"../include/bitcoin.h"
BitCoin::json BitCoin::fetch()
{
    this->curlHandle.fetch();
    return json::parse(this->curlHandle.getFetchedData());
};

BitCoin::BitCoin():curlHandle({})
{
    this->curlHandle.setUrl(this->URL);
};

