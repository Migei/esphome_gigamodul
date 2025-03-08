import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome import pins
from esphome.const import (
    CONF_CURRENT_RESISTOR,
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_OHM,
    CONF_RX_PIN,
    CONF_INTERRUPT_PIN,
    CONF_NUMBER,
    # CONF_VARIANT,
)
from esphome.core import CORE

CONF_SENS1 = "gigasensor1"
CONF_SENS2 = "gigasensor2"
CONF_SENS3 = "gigasensor3"
CONF_SENS4 = "gigasensor4"
CONF_SENS5 = "gigasensor5"
CONF_SENS6 = "gigasensor6"
CONF_SENS7 = "gigasensor7"
CONF_SENS8 = "gigasensor8"


gigamodul_ns = cg.esphome_ns.namespace("gigamodul")
GigamodulComponent = gigamodul_ns.class_("GigamodulComponent", cg.PollingComponent)

def validate_rx_pin(value):
    value = pins.internal_gpio_input_pin_schema(value)
    if CORE.is_esp8266 and value[CONF_NUMBER] >= 16:
        raise cv.Invalid("Pins GPIO16 and GPIO17 cannot be used as RX pins on ESP8266.")
    return value

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(GigamodulComponent),
            
            cv.Optional(CONF_SENS1): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS2): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS3): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS4): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS5): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS6): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS7): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SENS8): sensor.sensor_schema( 
                unit_of_measurement="MΩ",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
                
            cv.Optional(CONF_RX_PIN): validate_rx_pin,
            cv.Optional(CONF_INTERRUPT_PIN): pins.internal_gpio_output_pin_schema,
        }
    ).extend(cv.polling_component_schema("10s"))
)

async def to_code(config):
    # Erzeuge die Instanz deiner Komponente.
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var)

    if CONF_RX_PIN in config:
        rx_pin = await cg.gpio_pin_expression(config[CONF_RX_PIN])
        cg.add(var.set_rx_pin(rx_pin))
    
    if CONF_INTERRUPT_PIN in config:
        interrupt_pin = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN])
        cg.add(var.set_interrupt_pin(interrupt_pin))

    # Erzeuge acht Sensoren mit festen Einstellungen
    if CONF_SENS1 in config:
        sensor1 = await sensor.new_sensor(config[CONF_SENS1])
        cg.add(var.set_sensor(0, sensor1))
    if CONF_SENS2 in config:
        sensor2 = await sensor.new_sensor(config[CONF_SENS2])
        cg.add(var.set_sensor(1, sensor2))
    if CONF_SENS3 in config:
        sensor3 = await sensor.new_sensor(config[CONF_SENS3])
        cg.add(var.set_sensor(2, sensor3))
    if CONF_SENS4 in config:
        sensor4 = await sensor.new_sensor(config[CONF_SENS4])
        cg.add(var.set_sensor(3, sensor4))
    if CONF_SENS5 in config:
        sensor5 = await sensor.new_sensor(config[CONF_SENS5])
        cg.add(var.set_sensor(4, sensor5))
    if CONF_SENS6 in config:
        sensor6 = await sensor.new_sensor(config[CONF_SENS6])
        cg.add(var.set_sensor(5, sensor6))
    if CONF_SENS7 in config:
        sensor7 = await sensor.new_sensor(config[CONF_SENS7])
        cg.add(var.set_sensor(6, sensor7))
    if CONF_SENS8 in config:
        sensor8 = await sensor.new_sensor(config[CONF_SENS8])
        cg.add(var.set_sensor(7, sensor8))
