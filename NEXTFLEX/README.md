# NEXTflex — World Reflex Challenge

Team NEXT · NEXTpage · MMM Academy

WRO International Final 2026 · San Juan, Puerto Rico · 8–10 December

NEXTflex is an interactive brand experience developed to engage visitors with NEXTpage. Players enter an alias and country, then press a large arcade button only when a vowel lights up. The game demonstrates selective attention and reaction time; it is not a validated reading-comprehension or diagnostic test.

## Technical overview

- ESP32 firmware V3.3: touchscreen, 52 NeoPixels, arcade button and DFPlayer Mini audio.
- Local Python/Flask dashboard based on V4.3.2: SQLite, live game leaderboard, participation map and record celebrations.
- USB serial at 115200 baud; no internet required during play.
- Fifteen letters per game, four vowels, eleven consonants; one second per stimulus and four-second countdown.

## Documentation

- [English assembly guide and bill of materials](docs/ASSEMBLY.md)
- [Montaje y componentes en español](docs/MONTAJE.md)
- [Arduino firmware](firmware/NEXTflex/NEXTflex.ino)
- [Display configuration](firmware/config/User_Setup.h)
- [Python server](interface/servidor.py)
- [Dashboard](interface/ranking.html)
- [Local asset setup](interface/ASSETS.md)
- [Original alphabet machine STL](../3D%20DESIGN/juego%20abecedario.stl) (earlier design; not necessarily the final enclosure).

## Quick start

Install the ESP32 Arduino board package, TFT_eSPI, Adafruit NeoPixel and DFRobotDFPlayerMini. Compare the display configuration against the working hardware before uploading. In the interface directory, run:

```sh
python -m pip install -r requirements.txt
python servidor.py
```

Close Arduino Serial Monitor first. Default port COM65; override with NEXTFLEX_PORT (Windows CMD: set NEXTFLEX_PORT=COM5). Open http://127.0.0.1:5000. The database is created locally. Copy the prototype's static assets as described above.

## Data and limitations

The firmware sends CSV,ALIAS,ES,4,0,0,245200 (alias, ISO code, hits, misses, incorrect presses, mean reaction time in microseconds). The ranking orders games by hits, incorrect presses and mean reaction time. The map counts games by country. This is not a unique-player ranking. No offline USB queue or individual letter timing persistence is implemented.

Use aliases and voluntary participation. Never commit the database or exports. Audio and external image assets are excluded pending rights confirmation. Embedded map provenance needs review. No new license is assigned. The packaged firmware and separated dashboard need a final hardware and runtime check before merging.

## Español

NEXTflex es una experiencia interactiva complementaria a NEXTpage para atraer visitantes al stand. El participante identifica vocales iluminadas y su tiempo de reacción aparece en una clasificación local. No mide directamente la comprensión lectora. Consulta la [guía de montaje en español](docs/MONTAJE.md).
