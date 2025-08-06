#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
//Copyright(c)2022 Vishal Ahirwar.
#include <string>
#if __cplusplus
extern "C" 
#endif
std::size_t dataHandler(const char *buffer, std::size_t size, std::size_t nmemb, std::string *userData)
{
    if (userData == nullptr)
        return 0;
    userData->append(buffer, (size * nmemb));
    return (size * nmemb);
};

#endif // DATA_HANDLER_H