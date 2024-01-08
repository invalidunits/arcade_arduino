// Noah Hewitt
// Arduino Arcade
// 2023 December 20-XX


// #define COINSLOT_PIN PIN2
// #define COINSLOT_NC_SWITCH

#include "coinslot.h"

// Timing variables
unsigned long           last_time = 0; // The time of the last update.
unsigned long           cs_timer = 0; // elapsed time since the last signal

// This is incremented during a coinslot interupt.
// If we timeout, the amount of signals are processed and sent.
CS_VOLATILE uint8_t cs_sig_count = 0; 

// constant variables
constexpr unsigned long cs_timeout = 100; // The amount of time it takes to 
constexpr auto arcade_baud_rate = 115200; // The baud rate of arcade application 

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(arcade_baud_rate);
    while (!Serial); // Wait for a Serial named Godot

    CS_attachCoinSignal(coinSignal);
    interrupts();
}


void coinSignal() {
    ++cs_sig_count;
    if (((cs_sig_count + 1)<cs_coin_values_size)) 
        cs_sig_count = cs_coin_values_size - 1;

    cs_timer = 0;
}

void loop() { 
    // Determine the time
    auto time = millis();
    cs_timer += time - last_time;
    last_time = time;


    if (cs_timer >= cs_timeout) {
        const auto value = String(CS_VALUE_FROM_SIGNAL(cs_sig_count));
        Serial.print("[C" + value + "]");
        cs_sig_count = 0;
        cs_timer = 0;
    }
}
