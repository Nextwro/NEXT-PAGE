# NEXTflex — Montaje y programación

## Componentes

| Cantidad | Componente | Función |
|---:|---|---|
| 1 | ESP32 DevKit V1 / ESP32-WROOM-32 | Control principal |
| 1 | TFT SPI ILI9488 3,5 pulgadas, 480 x 320, táctil XPT2046 | Registro y resultados |
| 52 | NeoPixels WS2812B compatibles | Dos por letra, A–Z |
| 1 | Pulsador arcade de 200 mm, contacto normalmente abierto | Respuesta |
| 1 | DFPlayer Mini | Audio |
| 1 | microSD FAT32, hasta 32 GB | Seis pistas MP3 |
| 1 | Altavoz compatible 4–8 ohmios, hasta 3 W | Sonido |
| 1 | Fuente regulada de 5 V | LEDs y audio; dimensionar según consumo real |
| 1 | Adaptador lógico 74AHCT125 o equivalente | Recomendado para datos NeoPixel |
| 1 | Resistencia 330–470 ohmios | Recomendada en datos NeoPixel |
| 1 | Resistencia 1 kΩ | Recomendada hacia RX del DFPlayer |
| 1 | Condensador 1000 µF, mínimo 6,3 V | Recomendado en alimentación LED |
| 1 | Condensador 470 µF, mínimo 6,3 V | Recomendado cerca del DFPlayer |
| 1 | Cable USB de datos y portátil Windows | Ranking local |
| Varios | Cableado, conectores, tornillos, carcasa y letras translúcidas | Montaje mecánico |

Los elementos de protección y alimentación son recomendaciones; no se ha verificado que todos estén instalados en el prototipo.

## Conexiones

| Señal | GPIO ESP32 |
|---|---:|
| TFT CS | 5 |
| TFT RESET | 4 |
| TFT DC | 2 |
| TFT MOSI / táctil T_DIN | 23 |
| TFT SCK / táctil T_CLK | 18 |
| Táctil T_DO (MISO) | 19 |
| Táctil T_CS | 21 |
| NeoPixel DIN (mediante adaptador lógico recomendado) | 13 |
| Pulsador (otro terminal a GND) | 27 |
| DFPlayer TX hacia ESP32 RX2 | 16 |
| ESP32 TX2 hacia DFPlayer RX (1 kΩ) | 17 |

La configuración TFT es la reconstruida del montaje descrito: confirmar con el cableado real. No conectar SDO de la ILI9488 al MISO compartido; el táctil sí utiliza MISO. IRQ táctil no conectado. Alimentar pantalla y retroiluminación según las especificaciones de su módulo. Todas las masas deben ser comunes. Conectar el altavoz entre SPK1 y SPK2, nunca a masa. No alimentar los LEDs desde el pin 3,3 V de la ESP32 ni unir dos fuentes de 5 V sin comprobar su aislamiento. Para 52 píxeles, un límite conservador de 60 mA por píxel a blanco máximo supone 3,12 A solo en LEDs; aunque el juego enciende dos por letra, dimensionar la fuente y protecciones con margen para audio y posibles estados de prueba.

## Montaje y carga

1. Montar las 26 letras en cámaras opacas separadas, con dos LEDs por letra. A utiliza índices 0 y 1, B 2 y 3, hasta Z 50 y 51. Respetar el sentido DIN/DOUT.
2. Instalar pantalla, pulsador y altavoz en la carcasa; asegurar cables y dejar acceso a USB y microSD. No se incluyen archivos CAD de fabricación.
3. Formatear microSD FAT32 y crear carpeta `mp3` con `0001.mp3` (inicio), `0002.mp3` (acierto), `0003.mp3` (vocal perdida), `0004.mp3` (error), `0005.mp3` (final), `0006.mp3` (perfecta). Usar pistas propias o autorizadas, preferiblemente cortas; inicio de aproximadamente cuatro segundos.
4. En Arduino IDE instalar el paquete de placas ESP32 de Espressif y las librerías TFT_eSPI, Adafruit NeoPixel y DFRobotDFPlayerMini. Seleccionar ESP32 Dev Module o la placa correspondiente. Conservar la configuración TFT que ya funciona; si es un montaje nuevo, adaptar `firmware/config/User_Setup.h` a la librería TFT_eSPI y verificar pines. La calibración táctil incluida es `{263,3614,187,3601,3}` con rotación 1; recalibrar si cambia la pantalla.
5. Abrir `firmware/NEXTflex/NEXTflex.ino`, seleccionar puerto y cargar. Probar pantalla, letras, pulsador y audio. El monitor serie a 115200 muestra el resultado CSV tras cada partida.
6. En el portátil instalar Python, abrir CMD en `interface`, ejecutar `python -m pip install -r requirements.txt`. Cerrar el monitor serie. Ejecutar `INICIAR_NEXTFLEX.bat` (COM65) o configurar otro puerto con `set NEXTFLEX_PORT=COM5` y ejecutar `python servidor.py`. Abrir http://127.0.0.1:5000 y jugar una partida. Mantener la ventana de comandos abierta.
7. Comprobar que el ranking se actualiza y exportar una prueba CSV. Conservar la base de datos local fuera de GitHub. Para una instalación existente, hacer copia de seguridad de `nextflex.db` antes de actualizar.

## Funcionamiento

Cuenta atrás de 4 s; 15 estímulos, cuatro vocales y once consonantes. Ventana de reacción de 1 s, feedback de 200 ms e intervalo aleatorio de 100–250 ms. El pulsador usa interrupciones. Volumen 30 y brillo 255. La interfaz recibe resultados por USB, guarda en SQLite y muestra ranking de partidas, mapa de participación y confeti al batir récord perfecto. No necesita internet durante el juego. No almacena resultados si el USB está desconectado ni tiempos individuales por letra en la base de datos.
