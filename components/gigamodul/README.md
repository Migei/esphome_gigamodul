## gigamodul

zur kommunikation mit einem Materialfeuchte Gigamodul / Universalsenso von Scanntronik Mugrauer GmbH

Beispiel zum einbinden der Kompnente:

```yaml

external_components:
  - source: components

sensor:
  platform: "gigamodul"
  interrupt_pin: "D1"
  rx_pin: "D2"
  gigasensor1:
    name: "Sensor1"
  gigasensor2:
    name: "Sensor2"
  gigasensor3:
    name: "Sensor3"
  gigasensor4:
    name: "Sensor4"
  gigasensor5:
    name: "Sensor5"
  gigasensor6:
    name: "Sensor6"
  gigasensor7:
    name: "Sensor7"
  gigasensor8:
    name: "Sensor8"   
  update_interval: 60s
```
