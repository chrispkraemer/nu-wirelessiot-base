// BLE Service example app
//
// Creates a BLE environmental sensing service

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "CS397/497", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t environmental_sensing_service = {{
  .uuid128 = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00}
}};

static simple_ble_char_t temperature_state_char = {.uuid16 = 0x2A6E};
static simple_ble_char_t humid_state_char = {.uuid16 = 0x2A6F};
static simple_ble_char_t UV_state_char = {.uuid16 = 0x2A76};
static simple_ble_char_t WChill_state_char = {.uuid16 = 0x2A79};
static simple_ble_char_t twindspeed_state_char = {.uuid16 = 0x2A70};

static int16_t temperature = 0xF5FF;
static uint16_t humidity = 0x3B00;
static uint8_t UV = 0x01;
static int8_t wchill = 0xEC;
static uint16_t twindspeed = 0x0900;




/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  printf("Got write to a characteristic!\n");
}

int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&environmental_sensing_service);
  simple_ble_add_characteristic(1,0,0,0,sizeof(temperature),(uint8_t*)&temperature,&environmental_sensing_service,&temperature_state_char);
  simple_ble_add_characteristic(1,0,0,0,sizeof(humidity),(uint8_t*)&humidity,&environmental_sensing_service,&humid_state_char);
  simple_ble_add_characteristic(1,0,0,0,sizeof(UV),(uint8_t*)&UV,&environmental_sensing_service,&UV_state_char);
  simple_ble_add_characteristic(1,0,0,0,sizeof(wchill),(uint8_t*)&wchill,&environmental_sensing_service,&WChill_state_char);
  simple_ble_add_characteristic(1,0,0,0,sizeof(twindspeed),(uint8_t*)&twindspeed,&environmental_sensing_service,&twindspeed_state_char);

  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

