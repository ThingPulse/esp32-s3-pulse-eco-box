#include <sps30.h>
#include "settings.h"
// Example arduino sketch, based on 
// https://github.com/Sensirion/embedded-sps/blob/master/sps30-i2c/sps30_example_usage.c


// uncomment the next line to use the serial plotter
// #define PLOTTER_FORMAT

void pmInit() {
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;


  delay(2000);

  //sensirion_i2c_init();
  Wire.begin(SPS_SDA, SPS_SCL);

  /*while (sps30_probe() != 0) {
    SER.print("SPS sensor probing failed\n");
    delay(500);
  }*/

#ifndef PLOTTER_FORMAT
  SER.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    SER.print("error setting the auto-clean interval: ");
    SER.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    SER.print("error starting measurement\n");
  }

#ifndef PLOTTER_FORMAT
  SER.print("measurements started\n");
#endif /* PLOTTER_FORMAT */

#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  SER.print("Your Arduino hardware has a limitation that only\n");
  SER.print("  allows reading the mass concentrations. For more\n");
  SER.print("  information, please check\n");
  SER.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
  SER.print("\n");
  delay(2000);
#endif

  delay(1000);
}

String getPmAsJson() {
  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      SER.print("error reading data-ready flag: ");
      SER.println(ret);
    } else if (!data_ready)
      SER.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    SER.print("error reading measurement\n");
  } else {
    DynamicJsonDocument  doc(200);

    // since all values include particles smaller than X, if we want to create buckets we 
    // need to subtract the smaller particle count. 
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

    doc["mc_1p0"] = m.mc_1p0;
    doc["mc_2p5"] = m.mc_2p5;
    doc["mc_4p0"] = m.mc_4p0;
    doc["mc_10p0"] = m.mc_10p0;
    doc["nc_0p5"] = m.nc_0p5;
    doc["nc_1p0"] = m.nc_1p0 - m.nc_0p5;
    doc["nc_2p5"] = m.nc_2p5 - m.nc_1p0;
    doc["nc_4p0"] = m.nc_4p0 - m.nc_2p5;
    doc["nc_10p0"] = m.nc_10p0 - m.nc_4p0;
    doc["typical_particle_size"] = m.typical_particle_size;
    String message;

    serializeJson(doc, message);
    return message;




  }

}