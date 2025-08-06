# brt - Bitcoin Exchange Rate Tool

[![Build Status](https://github.com/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp/actions)
[![Release](https://img.shields.io/github/release/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp.svg)](https://github.com/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp/releases)

Live real-time Bitcoin prices in 30+ currencies from your terminal.

**Made by [Vishal Ahirwar](https://github.com/vishal-ahirwar)**

## Usage

```bash
# Live real-time updates (default)
brt

# Single fetch and exit
brt --once

# Custom refresh interval (minimum 5 seconds)
brt --interval 60

# Help
brt --help
```

**Live Mode Output:**
```
🟡 LIVE Bitcoin Rates (Update #3 at 14:32:15)
1 BTC =

Symbol |         15m |        Last |         Buy |        Sell
-------|-------------|-------------|-------------|-------------
ARS    | 153390084.45| 153390084.45| 153390084.45| 153390084.45
AUD    |    177148.79|    177148.79|    177148.79|    177148.79
USD    |    115182.17|    115182.17|    115182.17|    115182.17

Press Ctrl+C to exit • Auto-refresh every 30s
Next update in 5s...
```

## Installation

Download: [Releases](https://github.com/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp/releases)

## Build

```bash
pip install cppsage
git clone https://github.com/vishal-ahirwar/Bitcoin-Exchange-Rate-Cpp.git
cd Bitcoin-Exchange-Rate-Cpp
sage compile
```

## License

MIT
