# wsprbeacon
Band hopping WSPR beacon for Arduino and Si5351

This program will let you turn your Arduino, GPS and Si5351 module into a low power QRP WSPR beacon that hops over multiple bands.

What you need:
1. Arduino (Uno, Nano, Pro Mini 8MHz or 16MHz, or compatible)
2. GPS module (I recommend the ATGM336H but a NEO6, NEO7, or NEO8 will also do the job)
3. Si5351 breakout board

You need to dowload these libraries:

https://github.com/etherkit/Si5351Arduino

https://github.com/mikalhart/TinyGPSPlus

https://github.com/etherkit/JTEncode

The first thing you should do is edit the settings in the settings.h file.
All instructions are in the settings.h file.

If your call was received by other stations, you can find out the details here: http://wsprnet.org/drupal/wsprnet/spotquery

Have fun!
