# mqtt-clients

## Nano + W5500 HW-234
Inspiration that the ATMega 328P could do MQTT:
* https://blog.atx.name/building-avr-board-with-mqtt-support-for-iot/
* https://github.com/atx/avr-mqtt-board
* https://github.com/atx/umqtt

__Setting up Arduino IDE:__
* Set Board to: Arduino Nano
* Set Processor to: ATMega 328P (Old Bootloader)

__Preliminary checks__
* First check that the Arduino IDE + Nano works
  by loading the blinker example or `ta_ti_ti_ta.ino`
* Then check that the connection between Nano and
  the W5500 works, by loading the `ip_basic.ino`
  and in the serial console you should say that it reports
  the IP Address `192.168.1.2`.

* Connect the UTP cable to the network and `ping 192.168.1.2`,
  the shield should respond to the ping.


### Nano V3
* Nano 3.0 CH340G
* ATMega 328P-32TQFP
* https://www.aliexpress.com/item/1950565375.html


### W5500 HW-234
* W5500 Ethernet network module hardware
* SPI interface
* https://www.aliexpress.com/item/32824220397.html
* Arduino forum link:
* WizzNet forum: https://forum.wiznet.io/t/topic/1224
* Alternative library: https://github.com/RoboCore/Ethernet_W5500

__Specifications:__
Chip type: W5500
Supports both 3.3V & 5V.
Hardwired TCP/IP Protocols : TCP, UDP, ICMP, IPv4, ARP, IGMP, PPPoE
10BaseT/100BaseTX Ethernet PHY embedded
Supports automatic response (full duplex / half duplex mode)
Supports 8 independent sockets simultaneously
Internal 32Kbytes Memory for Tx/Rx Buffers
Power off mode: support
Network wake: support
SPI interface (SPI MODE 0, 3),convenient connection with MCU
Size: 55mm x 28mm

__Pins:__

```
 5V  3.3V
 GND MISO
 RST MOSI
 INT SCS
 NC  SCLK
```


[SPI Pins](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)
MISO = Master In Slave Out
MOSI = Master Out Slave In
SCLK = Serial Clock (Alias SCK)
SCS  = Serial Chip Select (Alias "Slave Select" SS)

__Other Pins:__
INT = Interrupt
RST = Reset
NC  = Not Connected

