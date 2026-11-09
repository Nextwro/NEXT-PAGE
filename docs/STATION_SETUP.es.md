# Guía de instalación — Estación base NEXTpage

**[🇬🇧 English version](./STATION_SETUP.md) · [Volver al README](../README.md#es)**

Esta guía documenta cómo reproducir la estación base NEXTpage en una **segunda Raspberry Pi 5**. La estación 2 es un **repuesto para la final internacional** y debe ejecutar los mismos archivos que la estación 1.

> **Estado:** instalación de respaldo en curso. La estación 1 dispone de sensor UART y dashboard probados. Las pruebas de la estación 2 deben verificarse sobre el hardware real. No sustituir el código probado por versiones reconstruidas.

## 1. Material necesario

- Raspberry Pi 5 con fuente USB-C adecuada y tarjeta microSD o SSD.
- Pantalla compatible con la Raspberry Pi.
- Sensor radar mmWave **DFRobot SEN0609 / C4001**.
- Cables para conectar alimentación y UART.

**Desconecta la alimentación antes de cablear.** Los GPIO de la Raspberry Pi trabajan con niveles lógicos de 3,3 V.

### Conexiones UART

| SEN0609 | Raspberry Pi 5 | Número de pin físico |
|---|---|---|
| VIN | 3V3 | 1 |
| GND | GND | 6 |
| TX | GPIO15 (RX) | 10 |
| RX | GPIO14 (TX) | 8 |
| OUT | Sin conectar | — |

Los pines **TX y RX van cruzados**. Si se cambia la alimentación a 5 V, verifica la compatibilidad de niveles de las señales UART; no apliques nunca 5 V a un GPIO de la Raspberry.

## 2. Activar el puerto serie

Abre una terminal:

```bash
sudo raspi-config
```

En **Interface Options → Serial Port**:

- ¿Usar el puerto serie para iniciar sesión? **No**.
- ¿Habilitar hardware serie? **Sí**.

En la Raspberry Pi 5 puede ser necesario activar UART0 en GPIO14/15 con la configuración compatible con su versión de Raspberry Pi OS. Una opción habitual es `dtoverlay=uart0-pi5` en `/boot/firmware/config.txt`. Evita configurar varios overlays incompatibles sobre el mismo puerto.

Reinicia:

```bash
sudo reboot
```

Comprueba el puerto:

```bash
ls -l /dev/ttyAMA0
```

En la estación original usamos **`/dev/ttyAMA0`, 9600 baudios** y recibimos tramas que comienzan por `$DFDMD`. La existencia del puerto no confirma, por sí sola, que se reciban datos del sensor.

## 3. Crear la carpeta de NEXTpage

```bash
mkdir -p ~/NEXTpage/station/static
cd ~/NEXTpage/station
sudo apt update
sudo apt install -y python3-serial python3-venv
```

Registra las versiones para documentar la instalación:

```bash
python3 --version
cat /etc/os-release
```

Copia desde la estación 1 el programa **exacto que allí funciona**:

```text
next_mmwave_uart.py
```

Si dispones de ambos archivos, compara sus hashes SHA-256 para detectar cambios accidentales:

```bash
sha256sum next_mmwave_uart.py
```

## 4. Prueba del SEN0609

Ejecuta desde la carpeta del proyecto:

```bash
cd ~/NEXTpage/station
python3 next_mmwave_uart.py
```

Comprueba que se muestran valores reales de presencia, distancia, velocidad radial y energía. Acércate y aléjate del radar para observar si cambian las lecturas.

El programa guarda el estado en:

```text
mmwave_state.json
```

En otra terminal:

```bash
cd ~/NEXTpage/station
cat mmwave_state.json
```

**Importante:** la puntuación `focus_level` de este prototipo es una estimación experimental del movimiento observado, no una medición directa de la concentración.

## 5. Configurar el dashboard FastAPI

Copia también desde la estación 1 los archivos probados:

```text
next_station_api.py
static/logo_next.png
```

Crea un entorno virtual e instala las dependencias:

```bash
cd ~/NEXTpage/station
python3 -m venv .venv
source .venv/bin/activate
pip install fastapi "uvicorn[standard]"
```

Con el programa del sensor funcionando en una terminal, inicia el servidor en otra:

```bash
cd ~/NEXTpage/station
source .venv/bin/activate
uvicorn next_station_api:app --host 0.0.0.0 --port 8000
```

Abre en el navegador:

```text
http://localhost:8000
```

Para inspeccionar los datos de la API:

```text
http://localhost:8000/api/state
```

Comprueba que se ve el logo, que el dashboard se actualiza y que los valores corresponden a las lecturas reales.

## 6. Pantalla completa / modo kiosko

Solo después de comprobar que ambos procesos funcionan manualmente, abre Chromium a pantalla completa:

```bash
chromium --kiosk --noerrdialogs --no-first-run http://localhost:8000
```

Después configuraremos el inicio automático según el entorno de escritorio instalado en la Raspberry. Los scripts de arranque deben entrar en **`~/NEXTpage/station`**, esperar a que el servidor esté disponible y evitar procesos duplicados.

## 7. Lista de comprobación — Estación 2

- [ ] Sensor conectado y alimentación verificada.
- [ ] UART habilitado; `/dev/ttyAMA0` accesible.
- [ ] Tramas `$DFDMD` reales recibidas.
- [ ] Presencia y ausencia detectadas en pruebas.
- [ ] Archivo `mmwave_state.json` actualizado.
- [ ] Endpoint `/api/state` con datos recientes.
- [ ] Dashboard y logotipo visibles en pantalla.
- [ ] Arranque en modo kiosko comprobado tras reiniciar.
- [ ] Comparación con la estación 1 realizada.
- [ ] MicroSD o SSD de respaldo preparado.

## Estado de futuras integraciones

La comunicación con el marcapáginas, la música, la carga inalámbrica y la sincronización con la app forman parte de otras fases. No son requisitos para validar esta primera instalación del radar y del dashboard.
