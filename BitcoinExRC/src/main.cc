// Copyright(c)2022 Vishal Ahirwar.
#include <fmt/color.h>
#include <fmt/core.h>

#include "../include/bitcoin.h"


using json = nlohmann::json;
void printColoredTable(const nlohmann::json& data) {
  using fmt::color;
  using fmt::fg;

  fmt::print(fg(color::yellow), "1 BTC =\n\n");

  fmt::print(fg(color::cyan), "{:<8}│ {:>12} │ {:>12} │ {:>12} │ {:>12}\n",
             "Symbol", "15m", "Last", "Buy", "Sell");
  fmt::print(fg(color::light_blue), "{:-<65}\n", "");

  for (const auto& [symbol, info] : data.items()) {
    fmt::print(fg(color::green), "{:<8}", symbol);
    fmt::print("│ ");
    fmt::print(fg(color::white),
               "{:>12.2f} │ {:>12.2f} │ {:>12.2f} │ {:>12.2f}\n",
               info["15m"].get<double>(), info["last"].get<double>(),
               info["buy"].get<double>(), info["sell"].get<double>());
  }
}
int main(void) {
  using namespace std;
  try {
    BitCoin bitcoin;
    json bitCoinData = bitcoin.fetch();
    printColoredTable(bitCoinData);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
};
