# Telecomando BLE per ATOTO A6

Sketch Arduino per ESP32 classico: legge il macropad USB HID tramite controller host MAX3421E e invia comandi multimediali BLE HID all'ATOTO A6.

## Requisiti

- ESP32 classico con USB-C (la USB-C della scheda non funziona da USB Host).
- Modulo USB Host basato su MAX3421E, con segnali SPI compatibili con logica 3,3 V, e alimentazione VBUS a 5 V per il macropad.
- Libreria Arduino `USB Host Shield Library 2.0` di felis.
- Libreria `ESP32 BLE Keyboard` di T-vK (`BleKeyboard.h`).

Collegamenti predefiniti MAX3421E -> ESP32 classico:

| MAX3421E | ESP32 |
| --- | --- |
| SCK | GPIO 18 |
| MISO | GPIO 19 |
| MOSI | GPIO 23 |
| CS / SS | GPIO 5 |
| INT | GPIO 17 |
| GND | GND |

Il macropad si collega alla presa USB host del modulo MAX3421E, non alla presa USB-C della scheda ESP32. Controlla che il modulo non porti segnali SPI a 5 V direttamente sui GPIO dell'ESP32.
- Libreria `ESP32 BLE Keyboard` di T-vK (`BleKeyboard.h`).

## Configurazione del macropad

Nel software di configurazione del macropad assegna questi tasti HID:

| Controllo | Tasto HID |
| --- | --- |
| Primo tasto | F13 |
| Tasto centrale | F14 |
| Terzo tasto | F15 |
| Rotella, senso orario | F16 |
| Rotella, senso antiorario | F17 |
| Pressione della rotella | F18 |

Lo sketch traduce F13/F14/F15 in traccia successiva/play-pausa/traccia precedente e F16/F17/F18 in volume+/volume-/mute.

## Flash e pairing

Installa le due librerie elencate sopra, apri `ATOTO_BLE_Remote/ATOTO_BLE_Remote.ino`, seleziona la scheda ESP32 Dev Module in Arduino IDE e carica lo sketch. Collega il macropad al MAX3421E, poi cerca `ATOTO Media` nelle impostazioni Bluetooth dell'ATOTO A6 e abbinalo come tastiera.

La whitelist accetta connessioni BLE soltanto dall'indirizzo `40:45:DA:C0:79:82`. Il codice usa il tipo di indirizzo BLE pubblico; se l'ATOTO usa un indirizzo privato/randomizzato, la whitelist dovra' essere adattata al suo indirizzo di identita'.