// Noah Hewitt
// Arduino Arcade
// 2023 December 20-XX


#include "coinslot.hpp"


using CSLOT = Coin::CoinSlot<2>;
constexpr auto baud_rate =      115200; // The baud rate of arcade application 
constexpr bool normally_open =  false;

void setup() {
    Serial.begin(baud_rate);
    CSLOT::setup(normally_open);
}

void loop() {
    if (auto coin = CSLOT::retrieve_coin()) {
        Serial.print("[C" + String(coin) + "]"); 
    }
}