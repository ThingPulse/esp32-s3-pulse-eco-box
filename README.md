# Pulse.Eco Box


Example project for the Pulse.Eco Box.

![Pulse.Eco Box](/resources/PulseEcoBox.jpeg)

## Hardware

- ESP32-S3-WROOM-1 N8R8
- WS2812E
- Sensirion SPS30: Particulate Matter Sensor [Datasheet](https://sensirion.com/media/documents/8600FF88/616542B5/Sensirion_PM_Sensors_Datasheet_SPS30.pdf)
- Bosch BME280: Temperature, Humidity and Pressure [Datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf)
- INGHAi GMA4030H11: PDM Microphone [Datasheet](https://datasheet.lcsc.com/lcsc/2005071903_INGHAi-GMA4030H11-F26_C498193.pdf)
- QST QMA7981: 3-Axis Accelerometer [Datasheet](https://datasheet.lcsc.com/lcsc/2004281102_QST-QMA7981_C457290.pdf)

## Pin Usage


- Sensirion SPS30:
    - SPS_SCL: IO6
    - SPS_SDA: IO7
- Accelerometer:
    - I2C_SCL: IO5
    - I2C_SDA: IO4
    - IMU_INT1: IO3
- SDS011
    - SDS_TXD: IO15
    - SDS_RXD: IO16
- Microphone:
    - PDM_CLK: IO41
    - PDM_DATA: IO2
- BME280:
    - I2C_SCL: IO5
    - I2C_SDA: IO4
- WS2812E
    - LED_PIN: IO8
- E5 Lora
    - E5_TXD: IO17
    - E5_RXD: IO18

![PCB](/resources/PCB.png)

## Documents

- [Schematic](/resources/Schematic_pulse.eco_box.pdf)
