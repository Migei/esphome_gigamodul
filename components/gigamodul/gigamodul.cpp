#include "gigamodul.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
//#include math.h

namespace esphome {
namespace gigamodul {

//#define RX_PIN 4  // Beispiel: Definiere hier den Pin, der für die Kommunikation genutzt wird
//#define INTERRUPT_PIN 5

#define BIT_DURATION 52       // Bitdauer in Mikrosekunden (1/19200 ≈ 52µs)
#define PACKET_LENGTH 17      // Anzahl an zu lesenden Bytes

static const char *const TAG = "gigamodulComp";

//float GigamodulComponent::get_setup_priority() const {
    //return setup_priority::IO;
//}

void GigamodulComponent::setup() {
  // Initialisierung: RX-Pin als INPUT_PULLUP und Interrupt-Pin als Ausgang konfigurieren, um den Interrupt auszulösen

  pinMode(rx_pin_->get_pin(), INPUT_PULLUP);
  pinMode(interrupt_pin_->get_pin(), INPUT);
  ESP_LOGE(TAG, "GigamodulComponent setup complete");
}

void GigamodulComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "dump_config");
}

//void GigamodulComponent::loop() {
//        ESP_LOGI(TAG, "loop");
//        delay(200);
//}


void GigamodulComponent::update() {
  // Beim Update-Zyklus:
  // 1. Interrupt auslösen
  send_interrupt();
  ESP_LOGW(TAG, "Interrupt gesendet, warte auf 10ms LOW-Puls...");
  
  // 2. Warten, bis das Modul den 10ms LOW-Puls sendet
  // Warten, bis der Bus erst LOW und dann wieder HIGH wird.
  unsigned long start = millis();
  // Warte auf LOW-Zustand (module signal)
  while (millis() - start < 200){
      if (digitalRead(rx_pin_->get_pin()) == LOW){
        delay(8);
        read_data();
        return;
      }
  }
  ESP_LOGW(TAG, "Timeout: Kein LOW-Zustand empfangen.");
  return ;
}

void GigamodulComponent::send_interrupt() {
  // Sende Interrupt: Ziehe den Bus für 40 µs auf LOW und dann 40 µs auf HIGH
  pinMode(interrupt_pin_->get_pin(), OUTPUT);
  digitalWrite(interrupt_pin_->get_pin(), LOW);
  //delay(40);
  delayMicroseconds(40);
  //delayMicroseconds(40);
  // Danach schalten wir in den Empfangsmodus:
  pinMode(interrupt_pin_->get_pin(), INPUT);
}

uint8_t GigamodulComponent::read_byte() {
  // Warte, bis das Startbit (HIGH) erkannt wird. 
  unsigned long start_time = micros();
  while (digitalRead(rx_pin_->get_pin()) != HIGH) {
    if (micros() - start_time > 1000) {  // Timeout nach 1000 µs
      ESP_LOGW("gigamodul", "Timeout beim Warten auf Startbit");
      return 0;
    }
  }
  
  // Verzögerung, um in die Mitte des Startbits zu gelangen
  //delayMicroseconds(BIT_DURATION / 2);
  
  uint8_t result = 0;
  // Lese 8 Datenbits (LSB zuerst)
  for (int i = 0; i < 8; i++) {
    delayMicroseconds(BIT_DURATION);
    int bit = (digitalRead(rx_pin_->get_pin()) == LOW) ? 1 : 0;
    result |= (bit << i);
  }
  
  // Lese die zwei Stopbits (erwartet LOW)
  delayMicroseconds(BIT_DURATION);
  int stop_bit1 = digitalRead(rx_pin_->get_pin());
  delayMicroseconds(BIT_DURATION);
  int stop_bit2 = digitalRead(rx_pin_->get_pin());
  
  if (stop_bit1 != LOW || stop_bit2 != LOW) {
    ESP_LOGW("gigamodul", "Fehler bei den Stopbits: %d / %d", stop_bit1, stop_bit2);
  }
  
  return result;
}

static void byte_to_bit_string(uint8_t byte, char *buffer) {
  for (int i = 0; i < 8; i++) {
    buffer[i] = (byte & (1 << (7 - i))) ? '1' : '0';
  }
  buffer[8] = '\0';
}


void GigamodulComponent::read_data() {
    uint8_t packet[PACKET_LENGTH];
    
    // Lies nacheinander alle erwarteten Bytes ein.
    for (int i = 0; i < PACKET_LENGTH; i++) {
      packet[i] = read_byte();
    }
    
    // Debug-Ausgabe des empfangenen Pakets
    ESP_LOGD(TAG, "Empfangenes Paket:");
    for (int i = 0; i < PACKET_LENGTH; i++) {
        char bits[9];
        byte_to_bit_string(packet[i], bits);
        //ESP_LOGD(TAG, "Byte %d: 0x%02X, Bits: %s", i, packet[i], bits);
    }
    
    // Auswertung der Sensorwerte: Es gibt 8 Sensoren, jeder mit 2 Bytes
    for (int sensor_index = 0; sensor_index < 8; sensor_index++) {
      int offset = sensor_index * 2;
      uint8_t low_byte = packet[offset];       // 0. Byte, enthält die niederwertigen 8 Bits
      uint8_t high_byte = packet[offset + 1];    // 1. Byte, aus dem nur die niederwertigen 4 Bits genutzt werden

      // Die niederwertigen 4 Bits des High-Bytes werden als obere 4 Bits an das Low-Byte angehängt.
      uint16_t raw_value = ((high_byte & 0x0F) << 8) | low_byte;
      float sensor_value = raw_value / 10.0;
        ESP_LOGI(TAG, "Sensor %d: raw value = %d", sensor_index, sensor_value);
      float resistance = pow(10, sensor_value /10);
      this->sensors_[sensor_index]->publish_state(resistance/1000000);
    
      //ESP_LOGI(TAG, "Sensor %d: raw value = %d, umgerechnet = %.1f", sensor_index, raw_value, resistance);
    
    }
  }

}  // namespace gigamodul
}  // namespace esphome
