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
  CURLcode fetch() const;
  const std::string getFetchedData() const;

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