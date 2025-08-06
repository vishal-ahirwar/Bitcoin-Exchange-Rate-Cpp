//Copyright(c)2022 Vishal Ahirwar.
#include"../include/bitcoin.h"
int main(void)
{
  using namespace std;
  using json=nlohmann::json;
  try
  {
    BitCoin bitcoin;
    json bitCoinData=bitcoin.fetch();
    std::cout<<"1 BTC = \n";
    for(auto it=bitCoinData.begin();it!=bitCoinData.end();++it)
    {
      printf
      (
        "\t(%3s) %10d %s\n",
        it.key().c_str(),
        it.value()["last"].get<int>(),
        it.value()["symbol"].get<string>().c_str()
      );
    };

  }
  catch(...)
  {
    std::cerr<<"Failed to Fetch Bitcoin Exchange rate!\n";
  }
  
  return 0;
};
