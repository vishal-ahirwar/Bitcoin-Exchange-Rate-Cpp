//Copyright(c)2022 Vishal Ahirwar.
#include <cstddef>
#define NOMINMAX  // 👈 prevent Windows macro pollution
#include <windows.h>
#include <algorithm> // for std::min
#include <string>

#include"../include/bitcoin.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Helper function to validate and clean JSON response
std::string BitCoin::validateAndCleanJson(const std::string& rawData) {
    if (rawData.empty()) {
        throw std::runtime_error("Empty response from API");
    }
    
    // Remove any leading/trailing whitespace
    std::string cleaned = rawData;
    cleaned.erase(0, cleaned.find_first_not_of(" \t\n\r"));
    cleaned.erase(cleaned.find_last_not_of(" \t\n\r") + 1);
    
    // Check if response starts and ends with expected JSON characters
    if (cleaned.empty() || (cleaned.front() != '{' && cleaned.front() != '[')) {
        throw std::runtime_error("Response doesn't start with valid JSON character: " + 
                                cleaned.substr(0, std::min(static_cast<size_t>(50UL), cleaned.length())));
    }
    
    if (cleaned.back() != '}' && cleaned.back() != ']') {
        size_t start_pos = (cleaned.length() >= 50) ? cleaned.length() - 50 : 0;
        throw std::runtime_error("Response appears truncated (doesn't end with } or ]). Last 50 chars: " + 
                                cleaned.substr(start_pos));
    }
    
    // Check for common malformed patterns
    size_t pos = 0;
    while ((pos = cleaned.find("{{", pos)) != std::string::npos) {
        throw std::runtime_error("Found double braces '{{' at position " + std::to_string(pos) + 
                                " - possible template rendering issue");
    }
    
    // Check for concatenated JSON objects
    pos = 0;
    while ((pos = cleaned.find("}{", pos)) != std::string::npos) {
        throw std::runtime_error("Found concatenated objects '}{' at position " + std::to_string(pos));
    }
    
    // Log response details for debugging if it's around the problematic length
    if (cleaned.length() > 2735 && cleaned.length() < 2800) {
        std::cerr << "Debug: Response length is " << cleaned.length() 
                  << ", chars around 2735: '" << cleaned.substr(2730, 20) << "'" << std::endl;
    }
    
    return cleaned;
}

BitCoin::json BitCoin::fetch()
{
    try {
        // Perform the HTTP fetch
        CURLcode result = this->curlHandle.fetch();
        
        // Get the raw response data
        std::string rawData = this->curlHandle.getFetchedData();
        
        // If we're having issues, print debug info
        if (rawData.length() > 2700 && rawData.length() < 2800) {
            std::cerr << "Debug: Suspicious response length, printing curl debug info:" << std::endl;
            this->curlHandle.printDebugInfo();
        }
        
        // Validate and clean the JSON
        std::string cleanedJson = validateAndCleanJson(rawData);
        
        // Attempt to parse the JSON with detailed error reporting
        try {
            return json::parse(cleanedJson);
        } catch (const nlohmann::json::parse_error& e) {
            // Print debug info on parse errors
            std::cerr << "JSON parse error occurred, printing debug info:" << std::endl;
            this->curlHandle.printDebugInfo();
            
            // Enhanced error reporting for parse errors
            std::string errorMsg = "JSON parsing failed: " + std::string(e.what());
            
            // Add context around the error position
            if (e.byte < cleanedJson.length()) {
                size_t start = (e.byte >= 20) ? e.byte - 20 : 0;
                size_t contextLength = std::min(static_cast<size_t>(40), cleanedJson.length() - start);
                std::string context = cleanedJson.substr(start, contextLength);
                
                errorMsg += "\nContext around error position " + std::to_string(e.byte) + ": '" + context + "'";
                
                // Highlight the problematic character
                if (e.byte >= start && (e.byte - start) < context.length()) {
                    errorMsg += "\nProblem character at position " + std::to_string(e.byte - start) + 
                               " in context: '" + std::string(1, context[e.byte - start]) + "'";
                }
            }
            
            // If the error is specifically around position 2735, provide more details
            if (e.byte >= 2730 && e.byte <= 2750) {
                errorMsg += "\nThis appears to be the recurring error at position ~2735";
                errorMsg += "\nResponse length: " + std::to_string(cleanedJson.length());
                
                // Show more context around this specific problematic area
                if (cleanedJson.length() > 2750) {
                    errorMsg += "\nExtended context (2720-2760): '" + 
                               cleanedJson.substr(2720, 40) + "'";
                }
            }
            
            throw std::runtime_error(errorMsg);
        }
        
    } catch (const std::exception& e) {
        // Re-throw with additional context
        throw std::runtime_error("BitCoin::fetch() failed: " + std::string(e.what()));
    }
}

BitCoin::BitCoin():curlHandle({})
{
    this->curlHandle.setUrl(this->URL);
}