// ======================================================
// NEXTFLEX V3.3 - Arcade Sound Edition
// MMM Academy / Team NEXT
// ESP32 WROOM-32, ILI9488, XPT2046, NeoPixels, DFPlayer
// ======================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include <esp_system.h>

// Pines y configuracion del juego
#define LED_PIN 13
#define BUTTON_PIN 27
#define DF_VOLUME 30
const uint32_t COUNTDOWN_MS=4000, LETTER_TIME_US=1000000, FEEDBACK_MS=200;

// Objetos de hardware
TFT_eSPI tft;
Adafruit_NeoPixel leds(52,LED_PIN,NEO_GRB+NEO_KHZ800);
HardwareSerial mp3Serial(2);
DFRobotDFPlayerMini mp3;
bool audioReady=false;

// Calibracion tactil: conservar estos valores
uint16_t calData[5]={263,3614,187,3601,3};

// Paleta de colores RGB565
const uint16_t C_BG=0xFFFF,C_PANEL=0xF79E,C_KEY=0xEF5D,C_ACCENT=0xF819,C_TEXT=0x18C3,C_MUTED=0x7BEF,C_GREEN=0x07E0,C_RED=0xF800;

// Base de paises y territorios: nombre y codigo ISO
const char* const countries[]={
  "Afghanistan:AF",
  "Aland Islands:AX",
  "Albania:AL",
  "Algeria:DZ",
  "American Samoa:AS",
  "Andorra:AD",
  "Angola:AO",
  "Anguilla:AI",
  "Antarctica:AQ",
  "Antigua and Barbuda:AG",
  "Argentina:AR",
  "Armenia:AM",
  "Aruba:AW",
  "Australia:AU",
  "Austria:AT",
  "Azerbaijan:AZ",
  "Bahamas:BS",
  "Bahrain:BH",
  "Bangladesh:BD",
  "Barbados:BB",
  "Belarus:BY",
  "Belgium:BE",
  "Belize:BZ",
  "Benin:BJ",
  "Bermuda:BM",
  "Bhutan:BT",
  "Bolivia:BO",
  "Bonaire, Sint Eustatius and Saba:BQ",
  "Bosnia and Herzegovina:BA",
  "Botswana:BW",
  "Bouvet Island:BV",
  "Brazil:BR",
  "British Indian Ocean Territory:IO",
  "Brunei:BN",
  "Bulgaria:BG",
  "Burkina Faso:BF",
  "Burundi:BI",
  "Cabo Verde:CV",
  "Cambodia:KH",
  "Cameroon:CM",
  "Canada:CA",
  "Cayman Islands:KY",
  "Central African Republic:CF",
  "Chad:TD",
  "Chile:CL",
  "China:CN",
  "Christmas Island:CX",
  "Cocos (Keeling) Islands:CC",
  "Colombia:CO",
  "Comoros:KM",
  "Congo:CG",
  "Cook Islands:CK",
  "Costa Rica:CR",
  "Cote d'Ivoire:CI",
  "Croatia:HR",
  "Cuba:CU",
  "Curacao:CW",
  "Cyprus:CY",
  "Czechia:CZ",
  "Democratic Republic of the Congo:CD",
  "Denmark:DK",
  "Djibouti:DJ",
  "Dominica:DM",
  "Dominican Republic:DO",
  "Ecuador:EC",
  "Egypt:EG",
  "El Salvador:SV",
  "Equatorial Guinea:GQ",
  "Eritrea:ER",
  "Estonia:EE",
  "Eswatini:SZ",
  "Ethiopia:ET",
  "Falkland Islands (Malvinas):FK",
  "Faroe Islands:FO",
  "Fiji:FJ",
  "Finland:FI",
  "France:FR",
  "French Guiana:GF",
  "French Polynesia:PF",
  "French Southern Territories:TF",
  "Gabon:GA",
  "Gambia:GM",
  "Georgia:GE",
  "Germany:DE",
  "Ghana:GH",
  "Gibraltar:GI",
  "Greece:GR",
  "Greenland:GL",
  "Grenada:GD",
  "Guadeloupe:GP",
  "Guam:GU",
  "Guatemala:GT",
  "Guernsey:GG",
  "Guinea:GN",
  "Guinea-Bissau:GW",
  "Guyana:GY",
  "Haiti:HT",
  "Heard Island and McDonald Islands:HM",
  "Holy See:VA",
  "Honduras:HN",
  "Hong Kong:HK",
  "Hungary:HU",
  "Iceland:IS",
  "India:IN",
  "Indonesia:ID",
  "Iran:IR",
  "Iraq:IQ",
  "Ireland:IE",
  "Isle of Man:IM",
  "Israel:IL",
  "Italy:IT",
  "Jamaica:JM",
  "Japan:JP",
  "Jersey:JE",
  "Jordan:JO",
  "Kazakhstan:KZ",
  "Kenya:KE",
  "Kiribati:KI",
  "Kuwait:KW",
  "Kyrgyzstan:KG",
  "Laos:LA",
  "Latvia:LV",
  "Lebanon:LB",
  "Lesotho:LS",
  "Liberia:LR",
  "Libya:LY",
  "Liechtenstein:LI",
  "Lithuania:LT",
  "Luxembourg:LU",
  "Macao:MO",
  "Madagascar:MG",
  "Malawi:MW",
  "Malaysia:MY",
  "Maldives:MV",
  "Mali:ML",
  "Malta:MT",
  "Marshall Islands:MH",
  "Martinique:MQ",
  "Mauritania:MR",
  "Mauritius:MU",
  "Mayotte:YT",
  "Mexico:MX",
  "Micronesia:FM",
  "Moldova:MD",
  "Monaco:MC",
  "Mongolia:MN",
  "Montenegro:ME",
  "Montserrat:MS",
  "Morocco:MA",
  "Mozambique:MZ",
  "Myanmar:MM",
  "Namibia:NA",
  "Nauru:NR",
  "Nepal:NP",
  "Netherlands:NL",
  "New Caledonia:NC",
  "New Zealand:NZ",
  "Nicaragua:NI",
  "Niger:NE",
  "Nigeria:NG",
  "Niue:NU",
  "Norfolk Island:NF",
  "North Korea:KP",
  "North Macedonia:MK",
  "Northern Mariana Islands:MP",
  "Norway:NO",
  "Oman:OM",
  "Pakistan:PK",
  "Palau:PW",
  "Palestine:PS",
  "Panama:PA",
  "Papua New Guinea:PG",
  "Paraguay:PY",
  "Peru:PE",
  "Philippines:PH",
  "Pitcairn:PN",
  "Poland:PL",
  "Portugal:PT",
  "Puerto Rico:PR",
  "Qatar:QA",
  "Reunion:RE",
  "Romania:RO",
  "Russia:RU",
  "Rwanda:RW",
  "Saint Barthelemy:BL",
  "Saint Helena, Ascension and Tristan da Cunha:SH",
  "Saint Kitts and Nevis:KN",
  "Saint Lucia:LC",
  "Saint Martin (French part):MF",
  "Saint Pierre and Miquelon:PM",
  "Saint Vincent and the Grenadines:VC",
  "Samoa:WS",
  "San Marino:SM",
  "Sao Tome and Principe:ST",
  "Saudi Arabia:SA",
  "Senegal:SN",
  "Serbia:RS",
  "Seychelles:SC",
  "Sierra Leone:SL",
  "Singapore:SG",
  "Sint Maarten (Dutch part):SX",
  "Slovakia:SK",
  "Slovenia:SI",
  "Solomon Islands:SB",
  "Somalia:SO",
  "South Africa:ZA",
  "South Georgia and the South Sandwich Islands:GS",
  "South Korea:KR",
  "South Sudan:SS",
  "Spain:ES",
  "Sri Lanka:LK",
  "Sudan:SD",
  "Suriname:SR",
  "Svalbard and Jan Mayen:SJ",
  "Sweden:SE",
  "Switzerland:CH",
  "Syria:SY",
  "Taiwan:TW",
  "Tajikistan:TJ",
  "Tanzania:TZ",
  "Thailand:TH",
  "Timor-Leste:TL",
  "Togo:TG",
  "Tokelau:TK",
  "Tonga:TO",
  "Trinidad and Tobago:TT",
  "Tunisia:TN",
  "Turkey:TR",
  "Turkmenistan:TM",
  "Turks and Caicos Islands:TC",
  "Tuvalu:TV",
  "Uganda:UG",
  "Ukraine:UA",
  "United Arab Emirates:AE",
  "United Kingdom:GB",
  "United States:US",
  "United States Minor Outlying Islands:UM",
  "Uruguay:UY",
  "Uzbekistan:UZ",
  "Vanuatu:VU",
  "Venezuela:VE",
  "Vietnam:VN",
  "Virgin Islands, British:VG",
  "Virgin Islands, U.S.:VI",
  "Wallis and Futuna:WF",
  "Western Sahara:EH",
  "Yemen:YE",
  "Zambia:ZM",
  "Zimbabwe:ZW",
};


// ======================================================
// Variables del jugador y estados del juego
// ======================================================
const int COUNTRY_COUNT = sizeof(countries) / sizeof(countries[0]);
int matches[COUNTRY_COUNT], matchCount = 0, page = 0;
String playerName, countrySearch, selectedCountry, selectedISO;
enum Screen {
  WELCOME, NAME_ENTRY, COUNTRY_ENTRY, READY, COUNTDOWN, PLAYING, RESULTS
};
Screen screen = WELCOME;
const char*keys[] = {
  "QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"
};
struct VocalResult {
  char letter;
  bool hit;
  uint32_t us;
};
VocalResult vocals[4];
char sequence[15];
uint8_t roundIndex = 0, vocalIndex = 0, hits = 0, misses = 0, falsePresses = 0;
uint32_t countdownStart = 0, letterStart = 0, phaseStart = 0, gapDuration = 0;
int lastNumber = 0;
enum Phase {
  WAIT_RELEASE, GAP, ACTIVE, FEEDBACK
};
Phase phase = WAIT_RELEASE;
bool releaseTracking = false, touchWasDown = false;
uint32_t releaseStart = 0;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool armed = false, pending = false;
volatile uint32_t pressUs = 0;
void showResults();

// ======================================================
// Interrupcion del pulsador: captura el primer flanco valido
// ======================================================
void IRAM_ATTR buttonISR() {
  uint32_t now = micros();
  portENTER_CRITICAL_ISR(&mux);
  if (armed && !pending) {
    pressUs = now;
    pending = true;
    armed = false;
  }
  portEXIT_CRITICAL_ISR(&mux);
}

// ======================================================
// Desactiva la captura y limpia eventos pendientes
// ======================================================
void disarm() {
  portENTER_CRITICAL(&mux);
  armed = false;
  pending = false;
  portEXIT_CRITICAL(&mux);
}

// ======================================================
// Audio: pistas 1 inicio, 2 acierto, 3 vocal perdida, 4 error, 5 final, 6 perfecto
// ======================================================
void playSound(uint8_t track) {
  if (audioReady)mp3.playMp3Folder(track);
}

// ======================================================
// Inicializacion del DFPlayer por UART2 (RX16, TX17)
// ======================================================
void setupAudio() {
  mp3Serial.begin(9600, SERIAL_8N1, 16, 17);
  delay(1000);
  audioReady = mp3.begin(mp3Serial, false, true);
  if (audioReady) {
    delay(300);
    mp3.volume(DF_VOLUME);
    delay(100);
  }
  Serial.println(audioReady ? "DFPLAYER CONFIGURED" : "DFPLAYER NOT AVAILABLE");
}

// ======================================================
// Funciones graficas de la pantalla TFT
// ======================================================
void center(const String&s, int x, int y, int font, uint16_t fg, uint16_t bg = C_BG) {
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(fg, bg);
  tft.drawString(s, x, y, font);
}
String fit(String s, int w, int font) {
  if (tft.textWidth(s, font) <= w)return s;
  while (s.length() > 1 && tft.textWidth(s + "...", font) > w)s.remove(s.length() - 1);
  return s + "...";
}
bool inside(int x, int y, int a, int b, int w, int h) {
  return x >= a && x < a + w && y >= b && y < b + h;
}
void button(int x, int y, int w, int h, const String&s, uint16_t bg = C_PANEL, uint16_t fg = C_TEXT, int font = 2) {
  tft.fillRoundRect(x, y, w, h, 7, bg);
  tft.drawRoundRect(x, y, w, h, 7, C_ACCENT);
  center(s, x + w / 2, y + h / 2, font, fg, bg);
}
void header(const String&s) {
  tft.fillScreen(C_BG);
  tft.fillRect(0, 0, 480, 40, C_PANEL);
  tft.drawFastHLine(0, 39, 480, C_ACCENT);
  center("NEXTFLEX", 240, 19, 4, C_ACCENT, C_PANEL);
  center(s, 240, 56, 2, C_TEXT);
}

// ======================================================
// Teclado tactil y deteccion de teclas
// ======================================================
void keyboard(int y) {
  for (int r = 0; r < 3; r++) {
    int n = strlen(keys[r]), x = (480 - (n*42 - 4)) / 2;
    for (int i = 0; i < n; i++)button(x + i*42, y + r*33, 38, 29, String(keys[r][i]), C_KEY);
  }
}
char keyAt(int x, int y, int top) {
  for (int r = 0; r < 3; r++) {
    int n = strlen(keys[r]), a = (480 - (n*42 - 4)) / 2;
    for (int i = 0; i < n; i++)if (inside(x, y, a + i*42, top + r*33, 38, 29))return keys[r][i];
  }
  return 0;
}

// ======================================================
// Busqueda de paises por nombre o codigo ISO
// ======================================================
String countryName(int i) {
  String s = countries[i];
  return s.substring(0, s.lastIndexOf(':'));
}
String countryCode(int i) {
  String s = countries[i];
  return s.substring(s.lastIndexOf(':') + 1);
}
void searchCountries() {
  matchCount = 0;
  String q = countrySearch;
  q.toUpperCase();
  for (int i = 0; i < COUNTRY_COUNT; i++) {
    String s = countryName(i);
    s.toUpperCase();
    if (!q.length() || s.indexOf(q) >= 0 || countryCode(i) == q)matches[matchCount++] = i;
  }
}

// ======================================================
// Pantalla de bienvenida
// ======================================================
void showWelcome() {
  screen = WELCOME;
  tft.fillScreen(C_BG);
  tft.drawRoundRect(24, 24, 432, 272, 15, C_ACCENT);
  center("REFLEX CHALLENGE", 240, 72, 4, C_ACCENT);
  center("NEXTFLEX", 240, 122, 4, C_TEXT);
  center("FOCUS. REACT. COMPETE.", 240, 160, 4, C_TEXT);
  button(130, 197, 220, 59, "START", C_ACCENT, C_BG, 4);
  center("GLOBAL PLAYER CHALLENGE", 240, 278, 2, C_MUTED);
}

// ======================================================
// Registro del nombre del jugador
// ======================================================
void updateName() {
  tft.fillRoundRect(36, 83, 408, 42, 6, C_PANEL);
  center(playerName.length() ? playerName : "YOUR NAME", 240, 104, 4, playerName.length() ? C_TEXT : C_MUTED, C_PANEL);
}
void showName() {
  screen = NAME_ENTRY;
  header("ENTER YOUR NAME");
  tft.drawRoundRect(34, 81, 412, 46, 7, C_ACCENT);
  updateName();
  keyboard(145);
  button(35, 283, 125, 32, "DELETE");
  button(177, 283, 125, 32, "BACK");
  button(319, 283, 125, 32, "NEXT", C_ACCENT, C_BG);
}

// ======================================================
// Seleccion de pais y paginacion
// ======================================================
void updateCountryResults() {
  tft.fillRect(0, 113, 480, 70, C_BG);
  if (!matchCount) {
    center("NO COUNTRIES FOUND", 240, 140, 2, C_RED);
    return;
  }
  for (int i = 0; i < 2; i++) {
    int pos = page*2 + i;
    if (pos < matchCount)button(20 + i*230, 120, 210, 39, fit(countryName(matches[pos]), 195, 2));
  }
  center(String(page + 1) + "/" + String((matchCount + 1) / 2) + " | " + String(matchCount) + " MATCHES", 240, 174, 2, C_MUTED);
}
void updateSearch() {
  tft.fillRoundRect(36, 77, 408, 33, 6, C_PANEL);
  tft.drawRoundRect(35, 76, 410, 35, 7, C_ACCENT);
  center(countrySearch.length() ? countrySearch : "SEARCH COUNTRY...", 240, 93, 2, C_TEXT, C_PANEL);
}
void refreshSearch() {
  page = 0;
  searchCountries();
  updateSearch();
  updateCountryResults();
}
void showCountry() {
  screen = COUNTRY_ENTRY;
  header("SELECT YOUR COUNTRY");
  searchCountries();
  updateSearch();
  updateCountryResults();
  keyboard(185);
  button(4, 285, 92, 31, "DELETE");
  button(100, 285, 85, 31, "BACK");
  button(189, 285, 85, 31, "PREV");
  button(278, 285, 85, 31, "NEXT");
  button(367, 285, 109, 31, "CLEAR", C_ACCENT, C_BG);
}

// ======================================================
// Confirmacion del jugador
// ======================================================
void showReady() {
  screen = READY;
  header("PLAYER CONFIRMED");
  tft.fillRoundRect(35, 85, 410, 113, 10, C_PANEL);
  tft.drawRoundRect(35, 85, 410, 113, 10, C_ACCENT);
  center("PLAYER", 240, 105, 2, C_MUTED, C_PANEL);
  center(playerName, 240, 133, 4, C_TEXT, C_PANEL);
  center(fit(selectedCountry + " [" + selectedISO + "]", 390, 2), 240, 174, 2, C_ACCENT, C_PANEL);
  button(115, 217, 250, 59, "START GAME", C_ACCENT, C_BG, 4);
  button(15, 284, 110, 31, "BACK");
}

// ======================================================
// Control de letras y NeoPixels
// ======================================================
bool vowel(char c) {
  return c && strchr("AEIOU", c) != nullptr;
}
void off() {
  leds.clear();
  leds.show();
}
void light(char c, uint32_t color) {
  leds.clear();
  int i = (c - 'A')*2;
  leds.setPixelColor(i, color);
  leds.setPixelColor(i + 1, color);
  leds.show();
}

// ======================================================
// Genera 15 letras: cuatro vocales distintas y once consonantes
// ======================================================
void generateSequence() {
  char v[] = "AEIOU";
  const char*c = "BCDFGHJKLMNPQRSTVWXYZ";
  for (int i = 4; i > 0; i--) {
    int j = random(i + 1);
    char t = v[i];
    v[i] = v[j];
    v[j] = t;
  }
  for (int i = 0; i < 15; i++)sequence[i] = i < 4 ? v[i] : c[random(21)];
  for (int i = 14; i > 0; i--) {
    int j = random(i + 1);
    char t = sequence[i];
    sequence[i] = sequence[j];
    sequence[j] = t;
  }
}

// ======================================================
// Pantalla de juego y contadores
// ======================================================
void stats() {
  int xs[] = {
    95, 240, 385
  };
  String values[] = {
    String(roundIndex + 1) + "/15", String(hits), String(misses + falsePresses)
  };
  uint16_t colors[] = {
    C_TEXT, C_GREEN, C_RED
  };
  for (int i = 0; i < 3; i++) {
    tft.fillRect(xs[i] - 55, 278, 110, 20, C_PANEL);
    center(values[i], xs[i], 289, 2, colors[i], C_PANEL);
  }
}
void clearLetter() {
  tft.fillRoundRect(35, 110, 410, 128, 10, C_BG);
}
void drawLetter(char c, uint16_t color) {
  clearLetter();
  tft.setTextSize(3);
  center(String(c), 240, 174, 4, color);
  tft.setTextSize(1);
}
void drawGameFrame() {
  header("REFLEX CHALLENGE");
  center(fit(playerName + " | " + selectedCountry, 450, 2), 240, 83, 2, C_MUTED);
  tft.drawRoundRect(25, 103, 430, 143, 12, C_ACCENT);
  const char*labels[] = {
    "ROUND", "HITS", "ERRORS"
  };
  for (int i = 0; i < 3; i++) {
    tft.fillRoundRect(30 + i*145, 255, 130, 48, 7, C_PANEL);
    center(labels[i], 95 + i*145, 266, 2, C_MUTED, C_PANEL);
  }
  stats();
}

// ======================================================
// Reinicia la partida y reproduce la melodia de inicio durante 4 segundos
// ======================================================
void startGame() {
  disarm();
  off();
  hits = misses = falsePresses = roundIndex = vocalIndex = 0;
  memset(vocals, 0, sizeof(vocals));
  generateSequence();
  screen = COUNTDOWN;
  header("GET READY");
  center("PRESS ONLY FOR VOWELS", 240, 112, 4, C_TEXT);
  center("A  E  I  O  U", 240, 170, 4, C_ACCENT);
  lastNumber = (COUNTDOWN_MS + 999) / 1000;
  center(String(lastNumber), 240, 246, 6, C_ACCENT);
  playSound(1);
  countdownStart = millis();
}

// ======================================================
// Espera a que el pulsador quede liberado
// ======================================================
void waitRelease() {
  phase = WAIT_RELEASE;
  releaseTracking = false;
}

// ======================================================
// Enciende la letra e inicia la medicion en microsegundos
// ======================================================
void beginLetter() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    waitRelease();
    return;
  }
  disarm();
  light(sequence[roundIndex], leds.Color(255, 0, 204));
  letterStart = micros();
  portENTER_CRITICAL(&mux);
  pending = false;
  armed = true;
  portEXIT_CRITICAL(&mux);
  phase = ACTIVE;
  drawLetter(sequence[roundIndex], C_ACCENT);
}

// ======================================================
// Registra la respuesta, muestra feedback y reproduce el efecto correspondiente
// ======================================================
void resolveLetter(bool pressed, uint32_t elapsed) {
  char c = sequence[roundIndex];
  bool v = vowel(c), hit = v && pressed;
  disarm();
  phase = FEEDBACK;
  uint8_t sound = 0;
  if (v) {
    vocals[vocalIndex++] = {
      c, hit, hit ? elapsed : 0
    };
    if (hit) {
      hits++;
      sound = 2;
    } else {
      misses++;
      sound = 3;
    }
  } else if (pressed) {
    falsePresses++;
    sound = 4;
  }
  if (hit)light(c, leds.Color(0, 255, 0));
  else if (sound)light(c, leds.Color(255, 0, 0));
  else off();
  phaseStart = millis();
  if (sound)playSound(sound);
  if (hit)drawLetter(c, C_GREEN);
  else if (sound)drawLetter(c, C_RED);
  else clearLetter();
  stats();
  Serial.printf("ROUND %u | %c | %s | %lu us\n", roundIndex + 1, c, hit ? "HIT" : v ? "MISS" : pressed ? "WRONG PRESS" : "IGNORED", (unsigned long)elapsed);
}

// ======================================================
// Motor del juego: cuenta atras, liberacion, intervalo, respuesta y feedback
// ======================================================
void updateGame() {
  if (screen == COUNTDOWN) {
    uint32_t e = millis() - countdownStart;
    if (e >= COUNTDOWN_MS) {
      screen = PLAYING;
      drawGameFrame();
      waitRelease();
      return;
    }
    int n = (COUNTDOWN_MS - e + 999) / 1000;
    if (n != lastNumber) {
      lastNumber = n;
      tft.fillRect(160, 210, 160, 70, C_BG);
      center(String(n), 240, 246, 6, C_ACCENT);
    }
    return;
  }
  if (screen != PLAYING)return;
  // Exige 20 ms de liberacion estable antes de continuar.
  if (phase == WAIT_RELEASE) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      releaseTracking = false;
      return;
    }
    if (!releaseTracking) {
      releaseTracking = true;
      releaseStart = millis();
    }
    if (millis() - releaseStart >= 20) {
      phase = GAP;
      phaseStart = millis();
      gapDuration = random(100, 251);
    }
    return;
  }
  // Intervalo aleatorio sin letras iluminadas.
  if (phase == GAP) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      waitRelease();
      return;
    }
    if (millis() - phaseStart >= gapDuration)beginLetter();
    return;
  }
  if (phase == FEEDBACK) {
    if (millis() - phaseStart >= FEEDBACK_MS) {
      off();
      clearLetter();
      if (++roundIndex >= 15) {
        showResults();
        return;
      }
      stats();
      waitRelease();
    }
    return;
  }
  // Lee la interrupcion y comprueba el limite de 1000 ms.
  if (phase == ACTIVE) {
    bool got = false, expired = false;
    uint32_t stamp = 0;
    portENTER_CRITICAL(&mux);
    if (pending) {
      got = true;
      stamp = pressUs;
      pending = false;
      armed = false;
    } else if (micros() - letterStart >= LETTER_TIME_US) {
      expired = true;
      armed = false;
    }
    portEXIT_CRITICAL(&mux);
    if (got) {
      uint32_t e = stamp - letterStart;
      resolveLetter(e < LETTER_TIME_US, e < LETTER_TIME_US ? e : 0);
    } else if (expired)resolveLetter(false, 0);
  }
}

// ======================================================
// Resultados individuales, media y melodia final
// ======================================================
void showResults() {
  screen = RESULTS;
  disarm();
  off();
  bool perfect = hits == 4 && misses == 0 && falsePresses == 0;
  playSound(perfect ? 6 : 5);
  uint32_t sum = 0;
  for (int i = 0; i < 4; i++)if (vocals[i].hit)sum += vocals[i].us;
  uint32_t avg = hits ? sum / hits : 0;
  header(perfect ? "PERFECT GAME!" : "GAME RESULTS");
  center(playerName + " | " + selectedISO, 240, 82, 2, C_TEXT);
  tft.fillRoundRect(25, 99, 430, 139, 10, C_PANEL);
  center("AVERAGE REACTION TIME", 240, 118, 2, C_MUTED, C_PANEL);
  center(hits ? String(avg / 1000.0f, 1) + " ms" : "NO VALID TIME", 240, 147, 4, C_ACCENT, C_PANEL);
  for (int i = 0; i < 4; i++) {
    int x = 89 + i*101;
    uint16_t col = vocals[i].hit ? C_GREEN : C_RED;
    center(String(vocals[i].letter), x, 186, 4, col, C_PANEL);
    center(vocals[i].hit ? String(vocals[i].us / 1000.0f, 1) + " ms" : "MISS", x, 216, 2, col, C_PANEL);
  }
  center("HITS: " + String(hits) + "/4", 118, 252, 2, C_GREEN);
  center("ERRORS: " + String(misses + falsePresses), 358, 252, 2, C_RED);
  button(135, 269, 210, 43, "NEW PLAYER", C_ACCENT, C_BG);
  Serial.printf("CSV,%s,%s,%u,%u,%u,%lu\n", playerName.c_str(), selectedISO.c_str(), hits, misses, falsePresses, (unsigned long)avg);
}

// ======================================================
// Gestion de pulsaciones en la pantalla tactil
// ======================================================
void handleTouch(int x, int y) {
  char c;
  switch (screen) {
    case WELCOME:
    if (inside(x, y, 130, 197, 220, 59)) {
      playerName = "";
      countrySearch = "";
      selectedCountry = "";
      selectedISO = "";
      page = 0;
      showName();
    }
    break;
    case NAME_ENTRY:
    c = keyAt(x, y, 145);
    if (c) {
      if (playerName.length() < 12) {
        playerName += c;
        updateName();
      }
    } else if (inside(x, y, 35, 283, 125, 32)) {
      if (playerName.length())playerName.remove(playerName.length() - 1);
      updateName();
    } else if (inside(x, y, 177, 283, 125, 32))showWelcome();
    else if (inside(x, y, 319, 283, 125, 32) && playerName.length()) {
      countrySearch = "";
      page = 0;
      showCountry();
    }
    break;
    case COUNTRY_ENTRY:
    for (int i = 0; i < 2; i++) {
      if (inside(x, y, 20 + i*230, 120, 210, 39)) {
        int p = page*2 + i;
        if (p < matchCount) {
          selectedCountry = countryName(matches[p]);
          selectedISO = countryCode(matches[p]);
          showReady();
        }
        return;
      }
    }
    c = keyAt(x, y, 185);
    if (c) {
      if (countrySearch.length() < 20) {
        countrySearch += c;
        refreshSearch();
      }
    } else if (inside(x, y, 4, 285, 92, 31)) {
      if (countrySearch.length())countrySearch.remove(countrySearch.length() - 1);
      refreshSearch();
    } else if (inside(x, y, 100, 285, 85, 31))showName();
    else if (inside(x, y, 189, 285, 85, 31) && page > 0) {
      page--;
      updateCountryResults();
    } else if (inside(x, y, 278, 285, 85, 31) && page + 1 < (matchCount + 1) / 2) {
      page++;
      updateCountryResults();
    } else if (inside(x, y, 367, 285, 109, 31)) {
      countrySearch = "";
      refreshSearch();
    }
    break;
    case READY:
    if (inside(x, y, 15, 284, 110, 31))showCountry();
    else if (inside(x, y, 115, 217, 250, 59))startGame();
    break;
    case RESULTS:
    if (inside(x, y, 135, 269, 210, 43))showWelcome();
    break;
    default:
    break;
  }
}

// ======================================================
// Inicializacion general del hardware
// ======================================================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
  leds.begin();
  leds.setBrightness(255);
  off();
  tft.init();
  tft.setRotation(1);
  tft.setTouch(calData);
  tft.setTextSize(1);
  setupAudio();
  randomSeed(esp_random());
  showWelcome();
  Serial.println("NEXTFLEX V3.3 READY");
}

// ======================================================
// Bucle principal: sin lectura tactil durante la partida
// ======================================================
void loop() {
  if (screen == PLAYING || screen == COUNTDOWN) {
    updateGame();
    delay(1);
    return;
  }
  uint16_t x, y;
  bool touching = tft.getTouch(&x, &y);
  if (touching && !touchWasDown)handleTouch(x, y);
  touchWasDown = touching;
  delay(2);
}
