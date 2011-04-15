avrdude -b115200 -F -p ATMEGA32 -P /dev/ttyUSB1  -c avr910 -V -U lfuse:w:0xFF:m -v
avrdude -b115200 -F -p ATMEGA32 -P /dev/ttyUSB1  -c avr910 -V -U hfuse:w:0xD9:m -v
avrdude -b115200 -F -p ATMEGA32 -P /dev/ttyUSB1  -c avr910 -V -U efuse:w:0xFF:m -v
echo "Programmer fuses set"
avrdude -b115200 -F -p ATMEGA32 -P /dev/ttyUSB1 -c avr109 -V -U flash:w:ff_prog_m324p.hex
echo "Programmer hex loaded"
