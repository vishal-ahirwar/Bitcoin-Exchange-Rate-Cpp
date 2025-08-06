add_executable(BitcoinExRC src/bitcoin.cc src/curlHandler.cc src/main.cc)
target_link_libraries(BitcoinExRC CURL::libcurl nlohmann_json::nlohmann_json)