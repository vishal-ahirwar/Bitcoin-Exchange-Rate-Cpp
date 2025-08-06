// Copyright(c)2022 Vishal Ahirwar.
#include <barkeep/barkeep.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <csignal>
#include <atomic>
#include <ctime>
#include <iomanip>
#include "../include/bitcoin.h"

using namespace std::chrono_literals;
namespace bk = barkeep;
using json = nlohmann::json;

// Global flag for graceful shutdown
std::atomic<bool> running{true};

void signalHandler(int /* signal */) {
    running = false;
    fmt::print("\n\n{}\n", fmt::styled("Goodbye! 👋", fmt::fg(fmt::color::yellow)));
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
#ifdef _WIN32
    // Use localtime_s on Windows
    std::tm tm_buf;
    localtime_s(&tm_buf, &time_t);
    return fmt::format("{:02d}:{:02d}:{:02d}", tm_buf.tm_hour, tm_buf.tm_min, tm_buf.tm_sec);
#else
    // Use localtime on Unix-like systems
    std::tm* tm_ptr = std::localtime(&time_t);
    return fmt::format("{:02d}:{:02d}:{:02d}", tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
#endif
}

void printColoredTable(const nlohmann::json& data, int updateCount = 0) {
    using fmt::color;
    using fmt::fg;
    
    // Print header with update info
    fmt::print(fg(color::yellow), "🟡 ");
    fmt::print(fg(color::orange), "LIVE Bitcoin Rates ");
    fmt::print(fg(color::gray), "(Update #{} at {})\n", 
               updateCount, getCurrentTimeString());
    fmt::print(fg(color::yellow), "1 BTC =\n\n");
    
    // Table header
    fmt::print(fg(color::cyan), "{:<8}│ {:>12} │ {:>12} │ {:>12} │ {:>12}\n",
               "Symbol", "15m", "Last", "Buy", "Sell");
    fmt::print(fg(color::light_blue), "{:-<65}\n", "");
    
    // Table data
    for (const auto& [symbol, info] : data.items()) {
        fmt::print(fg(color::green), "{:<8}", symbol);
        fmt::print("│ ");
        fmt::print(fg(color::white),
                   "{:>12.2f} │ {:>12.2f} │ {:>12.2f} │ {:>12.2f}\n",
                   info["15m"].get<double>(), info["last"].get<double>(),
                   info["buy"].get<double>(), info["sell"].get<double>());
    }
    
    // Footer with instructions
    fmt::print("\n");
    fmt::print(fg(color::gray), "Press Ctrl+C to exit • Auto-refresh every 30s\n");
    fmt::print(fg(color::dark_gray), "{:-<65}\n", "");
}

void printWelcomeMessage() {
    using fmt::color;
    using fmt::fg;
    
    fmt::print(fg(color::yellow), "🚀 ");
    fmt::print(fg(color::white), "Bitcoin Real-Time Tracker Started\n");
    fmt::print(fg(color::gray), "Fetching live data every 30 seconds...\n\n");
}

int main(int argc, char* argv[]) {
    // Set up signal handler for graceful shutdown
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    // Parse command line arguments
    bool realTimeMode = true;
    int refreshInterval = 5; // seconds
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--once" || arg == "-1") {
            realTimeMode = false;
        } else if (arg == "--interval" || arg == "-i") {
            if (i + 1 < argc) {
                try {
                    refreshInterval = std::stoi(argv[++i]);
                    if (refreshInterval < 5) refreshInterval = 5; // Minimum 5 seconds
                } catch (const std::exception&) {
                    fmt::print(fg(fmt::color::red), "Invalid interval value\n");
                    return 1;
                }
            }
        } else if (arg == "--help" || arg == "-h") {
            fmt::print("Usage: {} [options]\n", argv[0]);
            fmt::print("Options:\n");
            fmt::print("  --once, -1              Fetch data once and exit\n");
            fmt::print("  --interval, -i <secs>   Refresh interval (default: 30, min: 5)\n");
            fmt::print("  --help, -h              Show this help\n");
            return 0;
        }
    }
    
    try {
        BitCoin bitcoin;
        int updateCount = 0;
        
        if (!realTimeMode) {
            // Single fetch mode (original behavior)
            auto anim = bk::Animation({.message = "Fetching latest data"});
            json bitCoinData = bitcoin.fetch();
            anim->done();
            printColoredTable(bitCoinData);
            return 0;
        }
        
        // Real-time mode
        printWelcomeMessage();
        
        while (running) {
            try {
                // Show loading animation
                auto anim = bk::Animation({
                    .message = fmt::format("Updating... ({}s interval)", refreshInterval)
                });
                
                // Fetch data
                json bitCoinData = bitcoin.fetch();
                anim->done();
                
                // Clear screen and display updated data
                clearScreen();
                printColoredTable(bitCoinData, ++updateCount);
                
                // Wait for next update (with interruption check)
                for (int i = 0; i < refreshInterval && running; ++i) {
                    std::this_thread::sleep_for(1s);
                    
                    // Show countdown in the last 10 seconds
                    if (running && refreshInterval - i <= 10) {
                        fmt::print("\r{}", fmt::styled(
                            fmt::format("Next update in {}s...", refreshInterval - i),
                            fmt::fg(fmt::color::gray)
                        ));
                        std::cout.flush();
                    }
                }
                
                if (running) {
                    fmt::print("\r{:<30}\r", ""); // Clear countdown
                }
                
            } catch (const std::exception& e) {
                fmt::print(fg(fmt::color::red), 
                          "⚠️  Error fetching data: {}\n", e.what());
                fmt::print(fg(fmt::color::gray), 
                          "Retrying in {}s...\n", refreshInterval);
                
                // Wait before retry
                for (int i = 0; i < refreshInterval && running; ++i) {
                    std::this_thread::sleep_for(1s);
                }
            }
        }
        
    } catch (const std::exception& e) {
        fmt::print(fg(fmt::color::red), "Fatal error: {}\n", e.what());
        return 1;
    }
    
    return 0;
}
