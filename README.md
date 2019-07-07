# MPM3PM_ESPEasy
ESPEasy with ModBus Interface to Energy Meter MPM3PM

This version adds a Modbus interface to the Energy-Meter MPM3PM [amazon link](https://www.amazon.de/Energiemessger%C3%A4t-Stromz%C3%A4hler-Drehstromz%C3%A4hler-Hutschiene-Dualdisplay/dp/B073K11PPQ/ref=sr_1_1?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&keywords=MPM3PM&qid=1562506907&s=gateway&sr=8-1) to ESPEasy. The MPM3PM is a quality Energy meter made in Austria and suitable for bi-directional energy measurements.

The new MPM3PM library is based on a modified SDM630 library [SDM630](https://github.com/reaper7/SDM_Energy_Meter).

![Picture of MPM3PM](Pics/MPM3PM_Meter.JPG?raw=true "MPM3PM Meter installed")

You find pre-compiled binary file in the Binaries folder. If you need additional sensors you can compile everything by yourself.

To interface the ESP8266 with the ModBus interface of the meter you need an adapter interface from UART to RS-485 like this: [RS-485 Interface](https://www.ebay.de/itm/MAX485-TTL-Schnittstelle-Modul-Adapter-RS-485-RS-485-Arduino-Raspberry-Pi-Module/162384175341?hash=item25ced9e0ed:g:CAUAAOSwZZpdGJe7). If you (as i have done) support the DE pin on you have to short-circuit the DE and RE pin on the linked interface.


![Picture test-setup](Pics/ESP_under_test..JPG?raw=true "Setup during SW development")

![Picture test-setup](Pics/ESP_Installed.JPG?raw=true "Final HW installed")

