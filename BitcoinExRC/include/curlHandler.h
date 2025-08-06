#ifndef CURL_HANDLER_H
#define CURL_HANDLER_H
//Copyright(c)2022 Vishal Ahirwar.
#include <memory>
#include <curl/curl.h>
#include <functional>
#include <string>
typedef std::unique_ptr<CURL, std::function<void(CURL *)>> curl_ptr;
class CurlHandler
{
public:
  CurlHandler();
  void setUrl(const std::string &url);

  const std::string getFetchedData() const;
public:
    // ... your existing public methods ...
    
    // Debug method to print response information
    void printDebugInfo() const;
    
    // Modified fetch method signature (remove const if it was const)
    CURLcode fetch();  // Note: removed const since we're modifying data
private:
  curl_ptr curlptr;
  std::string data{};
  constexpr static auto deleter = [](CURL *c)
  {
    curl_easy_cleanup(c);
    curl_global_cleanup();
  };

protected:
};

#endif // CURL_HANDLER_H