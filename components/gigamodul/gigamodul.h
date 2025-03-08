#pragma once

#include <utility>
#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace gigamodul {

class GigamodulComponent : public PollingComponent {
 public:
  //float get_setup_priority() const override;
  void setup() override;
  void update() override;
  void dump_config() override;
  //void loop() override;
  
  // Methode zum Senden des Interrupt-Pulses
  void send_interrupt();
  
  uint8_t read_byte();
  // Methode zum Empfangen und Auswerten der Daten vom Universalmodul
  void read_data();
  


  // FUNKTIONEN ZUR KOMMUNIKATION MIT DER CONFIG.yaml
  // Sets the RX (receive) pin for the UART bus.
  // @param rx_pin Pointer to the internal GPIO pin used for reception.
  void set_rx_pin(InternalGPIOPin *rx_pin) { this->rx_pin_ = rx_pin; }
  void set_interrupt_pin(InternalGPIOPin *interrupt_pin) { this->interrupt_pin_ = interrupt_pin;}

  void set_sensor(uint8_t index, sensor::Sensor *sens) {
    if (index < sensors_.size()) {
      sensors_[index] = sens;
    }
  }

 protected:
  //Array mit 8 leeren Sensoren anlegen.
  std::array<sensor::Sensor*, 8> sensors_{nullptr};


  InternalGPIOPin *rx_pin_;
  InternalGPIOPin *interrupt_pin_;
  unsigned read_count_{};
  void read_data_();
  void restart_read_();
  uint32_t start_time_{};
  
};

}  // namespace gigamodul
}  // namespace esphome
