# NEXTpage home station — setup and replication guide

[🇪🇸 Versión en español](./STATION_SETUP.es.md) · [Back to README](../README.md#en)

**Purpose:** replicate the working Raspberry Pi 5 station on a second Raspberry Pi 5 without changing the code that was tested on Station 1. This guide covers the **SEN0609 sensor and dashboard**; bookmark, Spotify and wireless-charging integrations are tracked separately.

> This installation is still under replication. The original station was tested; the second station must be verified on its own hardware. The definitive source files must be copied from the original Raspberry Pi, not rewritten from memory.

## 1. Hardware

- Raspberry Pi 5 with reliable USB-C power supply
- DFRobot SEN0609 / C4001 24 GHz mmWave radar
- Connected display
- Four jumper cables

Disconnect the power before wiring. Raspberry Pi GPIO uses **3.3 V signalling**.

| SEN0609 | Pi 5 physical pin | Pi GPIO |
|---|---|---|
| VIN | 1 | 3V3 |
| GND | 6 | Ground |
| TX | 10 | GPIO15 / UART RX |
| RX | 8 | GPIO14 / UART TX |
| OUT | — | Leave disconnected |

TX and RX are crossed between devices. Check the exact module voltage and logic-level requirements before using any 5 V supply.

## 2. Enable UART

Run:

```bash
sudo raspi-config
```

Under **Interface Options → Serial Port**:

- Serial login shell: **No**
- Serial hardware: **Yes**

On Raspberry Pi 5, UART0 may need a specific overlay to route to **GPIO14/15**, for example `dtoverlay=uart0-pi5` in `/boot/firmware/config.txt` where supported by the installed Raspberry Pi OS. Confirm the official instructions and avoid duplicating conflicting overlays.

Restart and check:

```bash
sudo reboot
# After reboot:
ls -l /dev/ttyAMA0
```

A listed serial device does **not** prove the radar is communicating. The working Station 1 program used `/dev/ttyAMA0`, **9600 baud** and received frames beginning with `$DFDMD`.

## 3. Organise the working directory

```bash
mkdir -p ~/NEXTpage/station/static
cd ~/NEXTpage/station
sudo apt update
sudo apt install -y python3-serial python3-venv
```

Record local versions:

```bash
python3 --version
cat /etc/os-release
```

Copy the **verified** Station 1 sensor program, `next_mmwave_uart.py`, into this directory. Check the transfer with a file hash if possible:

```bash
sha256sum next_mmwave_uart.py
```

Run from the station directory:

```bash
python3 next_mmwave_uart.py
```

Look for live observations of presence, distance, velocity and signal energy. Test presence and absence, and note that the sensor cannot reliably infer a person's emotions.

The program creates `mmwave_state.json` in the working directory. Check:

```bash
cat mmwave_state.json
```

## 4. Dashboard with FastAPI

Copy the actual tested `next_station_api.py` and its logo into the station directory. Install the dependencies in an isolated Python environment:

```bash
cd ~/NEXTpage/station
python3 -m venv .venv
source .venv/bin/activate
pip install fastapi "uvicorn[standard]"
```

Once it is verified, commit the **dependency list** (not the `.venv` directory).

Leave the radar program running in one terminal, then open another:

```bash
cd ~/NEXTpage/station
source .venv/bin/activate
uvicorn next_station_api:app --host 0.0.0.0 --port 8000
```

Open `http://localhost:8000` in Chromium and test the JSON endpoint `http://localhost:8000/api/state`.

Check that the **logo, current values and update timestamps** work on Station 2.

## 5. Full-screen display / kiosk mode

Only after both services work manually should the station be set to open Chromium in kiosk mode:

```bash
chromium --kiosk --noerrdialogs --no-first-run http://localhost:8000
```

For automatic startup, configure the desktop environment appropriately (for example labwc autostart on compatible Raspberry Pi OS versions). The scripts must use the correct `~/NEXTpage/station` path, avoid duplicate background processes and wait until the API is ready.

## 6. Acceptance checklist — Station 2

- [ ] Correct UART wiring
- [ ] `/dev/ttyAMA0` accessible
- [ ] Live, valid `$DFDMD` frames from the real sensor
- [ ] Presence and absence verified
- [ ] `mmwave_state.json` updates
- [ ] FastAPI `/api/state` returns live data
- [ ] Dashboard opens and logo displays
- [ ] Kiosk startup tested after reboot
- [ ] Both stations produce comparable outputs
- [ ] A backup boot drive is available

## Known limitation

The prototype's *focus level* is a deliberately simple movement heuristic and must not be described as a validated concentration or enjoyment score. The system needs user testing and voluntary feedback before conclusions can be drawn about reading experience.
