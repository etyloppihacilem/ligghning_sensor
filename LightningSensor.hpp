/* #####################################################################################################################

               """          LightningSensor.hpp
        -\-    _|__
         |\___/  . \        Created on 31 Jul. 2024 at 11:36
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_LIGHTNING_SENSOR_LIGHTNINGSENSOR_HPP_
#define INCLUDE_LIGHTNING_SENSOR_LIGHTNINGSENSOR_HPP_

#include "hardware/gpio.h"
#include "pico/types.h"
#include <stdint.h>

typedef enum e_afe { // 0x00[5:1]
    indoor  = 0b0010'0100,
    outdoor = 0b0001'1100
} afe;

typedef enum e_event {
    noise_level_too_high = 0b0001,
    disturber            = 0b0100,
    lightning            = 0b1000
} event;

class LightningSensor {
    public:
        LightningSensor(uint miso, uint cs, uint sclk, uint mosi, uint intr);
        ~LightningSensor();

        void set_AFE(afe param);         ///< set AFE to either indoor or outdoor
        void set_enable_sensor(bool on); ///< will set the power of the device to on if true
        void set_threshold(uint8_t val); ///< set threshold (only 4 bits)

        afe      get_AFE();          ///< reads AFE state
        bool     get_sensor_state(); ///< return power state of the sensor
        uint16_t get_est_power();    ///< return estimated power value
        uint8_t  get_threshold();    ///< returns setting of threshold
        uint8_t  get_est_distance(); ///< returns getting distance
        event    get_last_event();   ///< returns last detected event

        void event_callback(gpio_irq_callback_t func);

    private:
        uint _miso;
        uint _cs;
        uint _sclk;
        uint _mosi;
        uint _intr;
        bool
            _state; // tells last known device state to issue warning if function is called when device is powered down.
        LightningSensor(const LightningSensor &);
};

#endif // INCLUDE_LIGHTNING_SENSOR_LIGHTNINGSENSOR_HPP_
