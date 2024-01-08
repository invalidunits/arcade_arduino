#ifndef _ARDUINO_COINSLOT
#define _ARDUINO_COINSLOT

/* 
    This is a helper Header only Library to work HX-196 coin slot
        Requires the available interupt pin defined COINSLOT_PIN.
    






*/

#ifndef COINSLOT_PIN 
#define COINSLOT_PIN PIN2
#endif

// Interupts can only be assigned to certain pins. I did not know this
static_assert(digitalPinToInterrupt(COINSLOT_PIN) != NOT_AN_INTERRUPT, "Coin slot pin must be have use as a valid interupt.");
constexpr uint8_t           cs_interupt = digitalPinToInterrupt(COINSLOT_PIN);

#ifdef COINSLOT_NC_SWITCH
#define COINSLOT_SWITCH RISING
#else
#define COINSLOT_SWITCH FALLING
#endif

constexpr uint16_t          cs_coin_values[] = {
    005,    // nickel 
    010,    // dime
    025,    // quarter
    100,    // dollar
    200     // toonie
};
constexpr auto              cs_coin_values_size = sizeof(cs_coin_values)/sizeof(cs_coin_values[0]);
#define CS_VALUE_FROM_SIGNAL(x) (((x + 1)<cs_coin_values_size)? \
    cs_coin_values[x + 1] : uint16_t(0))

// All variables changed in coin interrupt must be marked as volatile to avoid invalid compiler optimization.
#define CS_VOLATILE volatile 

#define CS_attachCoinSignal(x) pinMode(COINSLOT_PIN, INPUT_PULLUP), digitalWrite(COINSLOT_PIN, HIGH), \
            attachInterrupt(cs_interupt, x, COINSLOT_SWITCH)
#endif // _ARDUINO_COINSLOT