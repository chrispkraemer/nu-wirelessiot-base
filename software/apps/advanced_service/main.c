// BLE Service example app
//
// Creates a BLE service and blinks an LED

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

static simple_ble_service_t counter_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};

static simple_ble_char_t led_state_char = {.uuid16 = 0x1089};
static simple_ble_char_t button_state_char = {.uuid16 = 0x1090};
static simple_ble_char_t printf_state_char = {.uuid16 = 0x1091};
static uint8_t led_state = 0;
static uint8_t num_to_print_state = 0;
static uint8_t button_to_print_state = 0;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {

  // Check LED characteristic
  if (simple_ble_is_char_event(p_ble_evt, &led_state_char)) {
    printf("Got write to LED characteristic!\n");

    // Use value written to control LED
    if (led_state == 1) {
      printf("Turning on LED1!\n");
      nrf_gpio_pin_clear(LED1);
      nrf_gpio_pin_set(LED2);
      nrf_gpio_pin_set(LED3);
      nrf_gpio_pin_set(LED4);
    } else if(led_state == 2){
      printf("Turning on LED2!\n");
      nrf_gpio_pin_set(LED1);
      nrf_gpio_pin_clear(LED2);
      nrf_gpio_pin_set(LED3);
      nrf_gpio_pin_set(LED4);
    } else if(led_state == 3){
      printf("Turning on LED3!\n");
      nrf_gpio_pin_set(LED1);
      nrf_gpio_pin_set(LED2);
      nrf_gpio_pin_clear(LED3);
      nrf_gpio_pin_set(LED4);
    } else if(led_state == 4){
      printf("Turning on LED4!\n");
      nrf_gpio_pin_set(LED1);
      nrf_gpio_pin_set(LED2);
      nrf_gpio_pin_set(LED3);
      nrf_gpio_pin_clear(LED4);
    } else {
      printf("Bad LED input, turning them off\n");
      nrf_gpio_pin_set(LED1);
      nrf_gpio_pin_set(LED2);
      nrf_gpio_pin_set(LED3);
      nrf_gpio_pin_set(LED4);
    }
  }
  if (simple_ble_is_char_event(p_ble_evt, &printf_state_char)) {
    printf("Got write to printf characteristic!\n");
    printf("the written number is: %d\n",num_to_print_state);

  }
}

int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);
  nrf_gpio_pin_set(LED1);
  nrf_gpio_pin_set(LED2);
  nrf_gpio_pin_set(LED3);
  nrf_gpio_pin_set(LED4);
  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);


  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&counter_service);
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(led_state), (uint8_t*)&led_state,
      &counter_service, &led_state_char);
  simple_ble_add_characteristic(1, 0, 1, 0,
      sizeof(button_to_print_state), (uint8_t*)&button_to_print_state,
      &counter_service, &button_state_char);
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(num_to_print_state), (uint8_t*)&num_to_print_state,
      &counter_service, &printf_state_char);

  // Start Advertising
  simple_ble_adv_only_name();

  

  while(1) {
    if (nrf_gpio_pin_read(BUTTON1)) {
      button_to_print_state = 0;
    } else {
      button_to_print_state = 1;
      printf("button1, %d\n",button_to_print_state);
      simple_ble_notify_char(&button_state_char);
      
    } 
    if (nrf_gpio_pin_read(BUTTON2)) {
      button_to_print_state = 0;
    } else {
      button_to_print_state = 2;
      printf("button2\n");
      simple_ble_notify_char(&button_state_char);
    }
    if (nrf_gpio_pin_read(BUTTON3)) {
      button_to_print_state = 0;
    } else {
      button_to_print_state = 3;
      printf("button3\n");
      simple_ble_notify_char(&button_state_char);
    }
    if (nrf_gpio_pin_read(BUTTON4)) {
      button_to_print_state = 0;
    } else{
      button_to_print_state = 4;
      printf("button4\n");
      simple_ble_notify_char(&button_state_char);
    }
    nrf_delay_us(100000);
  }
}

