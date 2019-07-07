# MPM3PM_ESPEasy
ESPEasy with ModBus Interface to Energy Meter MPM3PM

This version adds a Modbus interface for the Energy-Meter MPM3PM [amazon link](https://www.amazon.de/Energiemessger%C3%A4t-Stromz%C3%A4hler-Drehstromz%C3%A4hler-Hutschiene-Dualdisplay/dp/B073K11PPQ/ref=sr_1_1?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&keywords=MPM3PM&qid=1562506907&s=gateway&sr=8-1) to ESPEasy. The MPM3PM is a quality Energy meter made in Austria and suitable for bi-directional energy measurements.

The new MPM3PM library is based on the SDM630 library [SDM630](https://github.com/reaper7/SDM_Energy_Meter).

![Picture of MPM3PM](Pics/MPM3PM_Meter.JPG?raw=true "MPM3PM Meter installed")

You find pre-compiled binaries in the Binaries folder (for 4M Flash). If you need additional not supported sensors you have to compile everything by yourself.

For the hardware interface between ESP8266 and the meter you need an adapter interface from UART to RS-485 like this: [RS-485 Interface](https://www.ebay.de/itm/MAX485-TTL-Schnittstelle-Modul-Adapter-RS-485-RS-485-Arduino-Raspberry-Pi-Module/162384175341?hash=item25ced9e0ed:g:CAUAAOSwZZpdGJe7). If DE pin support is enabled you have to short-circuit the DE and RE pin on the linked interface board and connect it to the selected GPIO.


![Picture test-setup](Pics/ESP_under_test..JPG?raw=true "Setup during SW development")

![Picture test-setup](Pics/ESP_Installed.JPG?raw=true "Final HW installed")

After installing ESPEasy configure the Energy Meter in the Device Setup tab, see example:

![Picture test-setup](Pics/ESPEasy_DeviceEdit.png?raw=true "MPM3PM Device Configuration")

After setup the measurement data of the selected sources is shown in the device overview:

![Picture test-setup](Pics/ESPEasy_DeviceOverview.png?raw=true "MPM3PM Device Configuration")

To transmit the measurement data to the outside of ESPEasy a Controller needs to be configured. The example below shows a MQTT interface:

![Picture test-setup](Pics/ESPEasy_MQTT_Controller.png?raw=true "MQTT Controller in ESPEasy")

Have fun!
