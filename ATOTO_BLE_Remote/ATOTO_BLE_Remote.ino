#include <Arduino.h>
#include <SPI.h>
#include <Usb.h>
#include <hidboot.h>
#include <BleKeyboard.h>
#include <BLEAddress.h>
#include <BLEAdvertising.h>
#include <BLEDevice.h>

namespace {

constexpr char kTargetAddress[] = "40:45:DA:C0:79:82";
constexpr uint8_t kNextKey = 0x68;
constexpr uint8_t kPlayPauseKey = 0x69;
constexpr uint8_t kPreviousKey = 0x6A;
constexpr uint8_t kVolumeUpKey = 0x6B;
constexpr uint8_t kVolumeDownKey = 0x6C;
constexpr uint8_t kMuteKey = 0x6D;

class AtotoBleKeyboard final : public BleKeyboard {
 public:
  using BleKeyboard::BleKeyboard;

 protected:
  void onStarted(BLEServer *) override {
    BLEDevice::whiteListAdd(BLEAddress(kTargetAddress));
    BLEDevice::getAdvertising()->setScanFilter(false, true);
  }
};

AtotoBleKeyboard bleRemote("ATOTO Media", "ESP32", 100);
USB usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> usbKeyboard(&usb);

void sendMediaKey(const MediaKeyReport key, const char *label) {
  if (!bleRemote.isConnected()) {
    return;
  }

  bleRemote.write(key);
  Serial.println(label);
}

void sendMappedKey(uint8_t key) {
  if (key == kNextKey) {
    sendMediaKey(KEY_MEDIA_NEXT_TRACK, "Avanti");
  }
  if (key == kPlayPauseKey) {
    sendMediaKey(KEY_MEDIA_PLAY_PAUSE, "Play/Pausa");
  }
  if (key == kPreviousKey) {
    sendMediaKey(KEY_MEDIA_PREVIOUS_TRACK, "Indietro");
  }
  if (key == kVolumeUpKey) {
    sendMediaKey(KEY_MEDIA_VOLUME_UP, "Volume +");
  }
  if (key == kVolumeDownKey) {
    sendMediaKey(KEY_MEDIA_VOLUME_DOWN, "Volume -");
  }
  if (key == kMuteKey) {
    sendMediaKey(KEY_MEDIA_MUTE, "Mute");
  }
}

class MacropadReportParser final : public KeyboardReportParser {
 protected:
  void OnKeyDown(uint8_t, uint8_t key) override {
    sendMappedKey(key);
  }
};

MacropadReportParser macropadParser;

}

void setup() {
  Serial.begin(115200);
  bleRemote.begin();

  if (usb.Init() == -1) {
    Serial.println("Errore: MAX3421E non avviato. Controlla SPI, CS e alimentazione.");
    return;
  }

  usbKeyboard.SetReportParser(0, &macropadParser);
  Serial.println("Host USB pronto. Associa 'ATOTO Media' all'ATOTO A6.");
}

void loop() {
  usb.Task();
  delay(2);
}