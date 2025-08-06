// Copyright(c)2022 Vishal Ahirwar.
#include "../include/curlHandler.h"
#include "../include/dataHandler.h"
#include <stdexcept>
#include <iostream>

CurlHandler::CurlHandler() : curlptr(curl_easy_init(), deleter) {
    curl_global_init(CURL_GLOBAL_ALL);
    
    if (!this->curlptr) {
        throw std::runtime_error("Failed to initialize curl");
    }
    
    // Basic curl options
    curl_easy_setopt(this->curlptr.get(), CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_WRITEFUNCTION, dataHandler);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_WRITEDATA, &(this->data));
    
    // Timeout settings
    curl_easy_setopt(this->curlptr.get(), CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_CONNECTTIMEOUT, 10L);
    
    // Additional important settings for reliable data transfer
    curl_easy_setopt(this->curlptr.get(), CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_USERAGENT, "BitcoinTracker/1.0");
    curl_easy_setopt(this->curlptr.get(), CURLOPT_ACCEPT_ENCODING, ""); // Enable compression
    
    // Ensure we get the complete response
    curl_easy_setopt(this->curlptr.get(), CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(this->curlptr.get(), CURLOPT_TCP_KEEPALIVE, 1L);
}

void CurlHandler::setUrl(const std::string &url) {
    if (url.empty()) {
        throw std::invalid_argument("URL cannot be empty");
    }
    curl_easy_setopt(this->curlptr.get(), CURLOPT_URL, url.c_str());
}

CURLcode CurlHandler::fetch() {
    // Clear previous data
    this->data.clear();
    
    if (!this->curlptr) {
        throw std::runtime_error("Curl handle not initialized");
    }
    
    // Perform the request
    CURLcode res = curl_easy_perform(this->curlptr.get());
    
    if (res != CURLE_OK) {
        std::string error = "Curl request failed: " + std::string(curl_easy_strerror(res));
        
        // Provide specific error context
        switch (res) {
            case CURLE_OPERATION_TIMEDOUT:
                error += " (Request timed out - API may be slow)";
                break;
            case CURLE_COULDNT_CONNECT:
                error += " (Could not connect - check internet connection)";
                break;
            case CURLE_COULDNT_RESOLVE_HOST:
                error += " (DNS resolution failed - check hostname)";
                break;
            case CURLE_RECV_ERROR:
                error += " (Error receiving data - connection interrupted)";
                break;
            case CURLE_SEND_ERROR:
                error += " (Error sending data)";
                break;
            default:
                break;
        }
        
        throw std::runtime_error(error);
    }
    
    // Check HTTP response code
    long response_code = 0;
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_RESPONSE_CODE, &response_code);
    
    if (response_code != 200) {
        throw std::runtime_error("HTTP error " + std::to_string(response_code) + 
                                " - Server returned error response");
    }
    
    // Validate that we received data
    if (this->data.empty()) {
        throw std::runtime_error("Received empty response from server");
    }
    
    // Get additional info for debugging
    double total_time = 0;
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_TOTAL_TIME, &total_time);
    
    size_t download_size = 0;
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_SIZE_DOWNLOAD_T, &download_size);
    
    // Log debug info if response seems suspicious
    if (this->data.length() > 2700 && this->data.length() < 2800) {
        std::cerr << "Debug - Response size: " << this->data.length() 
                  << ", Download time: " << total_time << "s" 
                  << ", Curl reported size: " << download_size << std::endl;
    }
    
    return res;
}

const std::string CurlHandler::getFetchedData() const { 
    return this->data; 
}

// Additional method to get response info for debugging
void CurlHandler::printDebugInfo() const {
    if (!this->curlptr) return;
    
    long response_code = 0;
    double total_time = 0;
    double download_time = 0;
    size_t download_size = 0;
    char* content_type = nullptr;
    
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_STARTTRANSFER_TIME, &download_time);
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_SIZE_DOWNLOAD_T, &download_size);
    curl_easy_getinfo(this->curlptr.get(), CURLINFO_CONTENT_TYPE, &content_type);
    
    std::cout << "=== Curl Debug Info ===" << std::endl;
    std::cout << "HTTP Response Code: " << response_code << std::endl;
    std::cout << "Total Time: " << total_time << "s" << std::endl;
    std::cout << "Download Time: " << download_time << "s" << std::endl;
    std::cout << "Downloaded Size: " << download_size << " bytes" << std::endl;
    std::cout << "Actual Data Size: " << this->data.length() << " bytes" << std::endl;
    std::cout << "Content Type: " << (content_type ? content_type : "unknown") << std::endl;
    
    if (this->data.length() > 50) {
        std::cout << "First 50 chars: " << this->data.substr(0, 50) << std::endl;
        std::cout << "Last 50 chars: " << this->data.substr(this->data.length() - 50) << std::endl;
    }
    std::cout << "======================" << std::endl;
}