#include <Arduino.h>
#include "qma7981.h"

QMA7981 Accel;

void initQma7981(TwoWire *twoWire) {
  Accel.initialize_default(twoWire);           // setup acceleromter with default settings
  Accel.set_full_scale_range(RANGE_4G); // set maximum range (+- 4G)
  Accel.set_bandwidth(MCLK_DIV_BY_975); // set bandwidth (samples per sec)
                                        // = CLK / DIV_BY = 50k / 975 = 50 samples / sec
}

String getQma7981AsJson() {
  DynamicJsonDocument  doc(200);

  doc["x"] = Accel.get_accel_x();
  doc["y"] = Accel.get_accel_y();
  doc["z"] = Accel.get_accel_z();

  String message;

  serializeJson(doc, message);

  return message;

}