/* #####################################################################################################################

               """          LightningSensor.cpp
        -\-    _|__
         |\___/  . \        Created on 31 Jul. 2024 at 11:36
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "LightningSensor.hpp"
#include <cstdint>

LightningSensor::LightningSensor(uint miso, uint cs, uint sclk, uint mosi, uint intr):
    _miso(miso),
    _cs(cs),
    _sclk(sclk),
    _mosi(mosi),
    _intr(intr) {
    _state = get_sensor_state();
}

LightningSensor::~LightningSensor() {}

void LightningSensor::set_AFE(afe param) {}

void LightningSensor::set_enable_sensor(bool on) {}

void LightningSensor::set_threshold(uint8_t val) {}

afe LightningSensor::get_AFE() {}

bool LightningSensor::get_sensor_state() {}

uint16_t LightningSensor::get_est_power() {}

uint8_t LightningSensor::get_threshold() {}

uint8_t LightningSensor::get_est_distance() {}

event get_last_event() {}
