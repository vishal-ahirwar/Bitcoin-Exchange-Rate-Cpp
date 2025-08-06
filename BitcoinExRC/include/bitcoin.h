
#ifndef BITCOIN_H
#define BITCOIN_H
// Copyright(c)2022 Vishal Ahirwar.
#include <cstdio>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "curlHandler.h"


class BitCoin {
  using json = nlohmann::json;

 public:
  json fetch();
  BitCoin();

 private:
  constexpr static const char* const URL = "https://blockchain.info/ticker";
  CurlHandler curlHandle;

 private:
  // Helper function for JSON validation and cleaning
  std::string validateAndCleanJson(const std::string& rawData);

 protected:
};
#endif  // BITCOIN_H