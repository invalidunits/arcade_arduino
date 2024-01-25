
/*
 * Created Date: Saturday, December 23rd 2023, 11:23:33 pm
 * Author: Noah Hewitt
 * 
 * Copyright (c) 2024 Noah Hewitt
 */

#ifndef _ARDUINO_COINSLOT
#define _ARDUINO_COINSLOT



/* 
    This is a helper Header only Library to work HX-196 Universal Coin Slot
*/
namespace Coin {
    #ifndef COIN_VALUE(signal)
        /// @brief The value of each coin per signal
        constexpr PROGMEM uint8_t default_coin_values[] = {
            0,
            5,
            10,
            25,
            100,
            200,
        };
    
    // Because 
    #define COIN_VALUE(signal) pgm_read_byte(default_coin_values + signal) 
    #endif 

    
    constexpr uint8_t max_coin_signal = sizeof(COIN_VALUE)/sizeof(*COIN_VALUE);


    /// @brief 
    /// This is one of the many reasons why I (sometimes) hate C++
    /// Maybe I shouldn't of made it this way... whatever...
    ///
    /// This is a static class that takes in a pin as a template parameter.
    /// Each instance of this classes use with a pin is technically... a different class.
    /// I abuse this functionality to generate a whole sort of variables and modifiers.
    ///
    ///
    /// @tparam coinslot_pin 
    /// The digital pin shorted to the COIN pin of the coinslot you wish to use.
    /// This pin must be a functional interrupt pin.
    template <uint8_t coinslot_pin = 2> class CoinSlot {
        public:
            static constexpr uint8_t interupt = digitalPinToInterrupt(coinslot_pin);
            static constexpr unsigned long timeout = 500; //ms

            static_assert(interupt != NOT_AN_INTERRUPT, "Pin must be an interupt pin to work with Coin Slot");

            /// @brief Setup the coinlots functionality 
            /// @param normally_open 
            // Set if the coinslot is set to normally open or normally closed.
            // This value should match the switch on the physical
            // I would recommnnd normally closed to prevent false signals during the
            // beginning of execution
            static void setup(bool normally_open = false) {
                if (has_setup) return;
                
                pinMode(coinslot_pin, INPUT_PULLUP);
                attachInterrupt(interupt, signal, normally_open? FALLING : RISING);
                has_setup = true;
            }

            /// @brief  Update the coin slot and return the value of coins
            /// @return Returns the value of coins inserted into the coinslot, Otherwise zero. 
            static uint8_t retrieve_coin() {
                uint8_t value = 0;
                if (!has_setup)
                    return value;

                
                if ((millis() - timer) > timeout) {
                    noInterrupts(); // Disable interupts to
                        sig_count %= max_coin_signal;
                        value = COIN_VALUE(sig_count);
                        sig_count = 0;
                  		timer = millis();
                    interrupts();
                    
                }

                return value;
            }

        private:
            CoinSlot();
            static void signal() {
                if (!has_setup)
                    return value;

                sig_count += 1;
                timer = millis();
            }

            inline static bool               has_setup = false;
            inline static volatile uint8_t        sig_count = 0;
            inline static volatile unsigned long timer = 0;

    };
}

#endif // _ARDUINO_COINSLOT