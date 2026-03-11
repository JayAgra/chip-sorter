# i used the arduino-cli for laziness. make sure to (macos):
# brew update
# brew install arduino-cli
# arduino-cli core update-index && arduino-cli lib update-index
# arduino-cli core install arduino:avr@1.8.7
# arduino-cli lib install "LiquidCrystal_I2C"
# arduino-cli lib install "Adafruit_TCS34725"
# arduino-cli lib install "Adafruit_BusIO"

# for older bootloader: arduino:avr:nano:cpu=atmega328old
FQBN = arduino:avr:nano:cpu=atmega328

# set me!!
# find me w ls -lha /dev/tty*
# PORT = /dev/tty.usbserial-110
PORT = /dev/tty.usbserial-10

PROGRAM_DIR = ./

all:
	arduino-cli compile --fqbn $(FQBN) $(PROGRAM_DIR)

upload: all
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(PROGRAM_DIR)

clean:
	rm -rf $(PROGRAM_DIR)/build