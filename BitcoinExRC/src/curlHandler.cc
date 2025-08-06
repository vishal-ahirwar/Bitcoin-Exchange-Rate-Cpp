//Copyright(c)2022 Vishal Ahirwar.
#include "../include/curlHandler.h"
#include"../include/dataHandler.h"
CurlHandler::CurlHandler() : curlptr(curl_easy_init(), deleter)
{

    curl_global_init(CURL_GLOBAL_ALL);
    curl_easy_setopt(this->curlptr.get(),CURLOPT_WRITEFUNCTION,dataHandler);
    curl_easy_setopt(this->curlptr.get(),CURLOPT_WRITEDATA,&(this->data));
};

void CurlHandler::setUrl(const std::string &url)
{
    curl_easy_setopt(this->curlptr.get(), CURLOPT_URL, url.c_str());
};

CURLcode CurlHandler::fetch() const
{
    return curl_easy_perform(this->curlptr.get());
};

const std::string CurlHandler::getFetchedData()const{
    return this->data;
};
