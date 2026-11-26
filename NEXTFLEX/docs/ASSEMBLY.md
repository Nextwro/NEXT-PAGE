# NEXTflex — Assembly and programming

## Bill of materials

| Qty | Part |
|---:|---|
| 1 | ESP32 DevKit V1 (ESP32-WROOM-32) |
| 1 | 3.5-inch 480 x 320 SPI ILI9488 TFT with XPT2046 resistive touch |
| 52 | WS2812B-compatible NeoPixels, two per letter |
| 1 | 200 mm normally-open arcade button |
| 1 | DFPlayer Mini and FAT32 microSD (up to 32 GB) |
| 1 | Compatible 4–8 ohm speaker, up to 3 W |
| 1 | Regulated 5 V power supply, sized for actual load |
| 1 each | Recommended 74AHCT125 level shifter, 330–470 ohm LED data resistor, 1 kΩ DFPlayer RX resistor, 1000 µF LED supply capacitor and 470 µF audio supply capacitor (capacitors rated at least 6.3 V) |
| 1 | USB data cable and Windows laptop |
| As needed | Enclosure, translucent letters, opaque light dividers, wiring and fasteners |

Protection components are recommendations, not a verified inventory of the prototype.

## Wiring

| Signal | ESP32 GPIO |
|---|---:|
| TFT CS / RESET / DC | 5 / 4 / 2 |
| TFT MOSI and touch DIN | 23 |
| TFT SCK and touch CLK | 18 |
| Touch DOUT (MISO) | 19 |
| Touch CS | 21 |
| NeoPixel DIN (level shifter recommended) | 13 |
| Arcade button (other terminal to GND) | 27 |
| DFPlayer TX to ESP32 RX2 | 16 |
| ESP32 TX2 to DFPlayer RX through 1 kΩ | 17 |

Verify display pins against the actual working wiring. Leave TFT SDO disconnected from the shared MISO line; touch DOUT uses MISO. Touch IRQ is unused. Follow the display module's power and backlight specifications. Connect all grounds. Connect the speaker across SPK1 and SPK2, not to ground. Do not power LEDs from the ESP32 3.3 V pin or combine independent 5 V supplies without checking isolation. At a conservative 60 mA per pixel, 52 pixels could draw 3.12 A at full white; allow additional headroom for audio and other loads.

## Setup

1. Install two LEDs per letter in isolated light chambers. A uses indices 0–1, B 2–3, and Z 50–51. Follow the data direction. Secure the screen, button, speaker and wiring; leave USB and SD access. CAD files are not included.
2. Format the microSD as FAT32. Create `/mp3/` and add licensed or original tracks: `0001.mp3` start (about four seconds), `0002.mp3` correct vowel, `0003.mp3` missed vowel, `0004.mp3` wrong press, `0005.mp3` normal finish, `0006.mp3` perfect game. Keep feedback sounds short.
3. Install Arduino IDE, Espressif ESP32 board support, TFT_eSPI, Adafruit NeoPixel and DFRobotDFPlayerMini. Select ESP32 Dev Module or the matching board. Keep your known-good TFT_eSPI setup, or adapt the supplied proposed `User_Setup.h`. The included touch calibration is `{263,3614,187,3601,3}` with rotation 1; recalibrate if needed.
4. Open and upload `firmware/NEXTflex/NEXTflex.ino`. Check the display, LEDs, button and audio. Serial Monitor at 115200 baud shows a CSV result after each game.
5. Install Python on the laptop. In `interface`, run `python -m pip install -r requirements.txt`. Close Arduino Serial Monitor. Connect USB, run `INICIAR_NEXTFLEX.bat` for COM65, or set another port with `set NEXTFLEX_PORT=COM5` and run `python servidor.py` from the same CMD window.
6. Open http://127.0.0.1:5000, play a game and verify that the dashboard updates. Keep the terminal open. Export CSV for analysis; back up `nextflex.db` with the server stopped. Never commit participant data.

## Operation

Four-second countdown, 15 stimuli (four vowels, eleven consonants), 1000 ms response window, 200 ms feedback and randomized 100–250 ms gap. Interrupt-based button capture. Volume 30 and brightness 255. USB sends game summaries to SQLite; the dashboard displays a game leaderboard, country participation map and perfect-game record celebration. No internet is needed during operation. The current implementation does not queue disconnected games or persist individual letter timings.
