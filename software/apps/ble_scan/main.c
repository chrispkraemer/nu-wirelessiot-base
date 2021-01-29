// BLE Scanning app
//
// Receives BLE advertisements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "simple_ble.h"

#include "nrf52840dk.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:4e:00:02
        .platform_id       = 0x4E,    // used as 4th octet in device BLE address
        .device_id         = 0x0002,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "CS397/497", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;


// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  // extract the fields we care about
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
  uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
  uint16_t adv_len = adv_report->data.len; // length of advertisement payload data

  //printf("Received an advertisement!\n");
  uint8_t i,j,offset,p;
  //printf("ble_addr: %x\n",ble_addr);
  //printf("adv_len: %d\n",adv_len);
  //printf("adv_buf:");
  //for(i = 0; i < adv_len; i++){
  //  printf("%02x",adv_buf[i]);
  //}
  //printf("\n");
  uint8_t sub_length = 0;
  p = 0;
  //printf("%d\n",adv_buf[0]);
  while(p < adv_len){
    sub_length = adv_buf[p];
    //printf("substring length: %d\n",sub_length);
    offset = p+1;
    for(j = 0; j < sub_length; j++){
      //printf("%02x",adv_buf[j+offset]);
    }
    //printf("\n");
    switch (adv_buf[offset])
      {
      case 0x01:
        //printf("flags:");
        for(i = 0; i < sub_length-1; i++){
          //printf("%02x",adv_buf[i+offset+2]);
        }
        //printf("\n");
        break;
      case 0x02:
        break;
      case 0x03:
        break;
      case 0x04:
        break;
      case 0x05:
        break;
      case 0x06:
        break;
      case 0x07:
        break;
      case 0x08:
        break;
      case 0x09:
        printf("local name: ");
        char local_name[31];
        for(i = 0; i < sub_length-1; i++){
          local_name[i] = adv_buf[i+offset+1];
        }
        //local_name[i] = "\0";

        printf("%s\n",local_name);
        break;
      case 0x0a:
        break;
      case 0x0b:
        break;
      case 0x0c:
        break;
      case 0x0d:
        break;
      case 0x0e:
        break;
      case 0x0f:
        break;
      case 0x10:
        printf("device id: ");
        for(i = 0; i < sub_length-1; i++){
          printf("%02x",adv_buf[i+offset+1]);
        }
        printf("\n");
        break;
      default:
        break;
      }
    //printf("\n");
    p += sub_length + 1;
  }
  //printf("leaving adv_scan\n");
  
}


int main(void) {


  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);
  advertising_stop();

  // Start scanning
  scanning_start();
  

  // go into low power mode
  while(1) {
    power_manage();
  }
}



