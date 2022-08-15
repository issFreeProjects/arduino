## you must change, depending on your hardware
MCU=atmega328p
F_CPU=16000000L
BRATE=57600
DEV=/dev/ttyUSB0
_OBJ=ssprintf.o utils.o serial.o ssh_I.o ssh.o


## you dont need to change
#  only when you wanna add library or another source directory
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I .
BD=build
OBJ=$(patsubst %,$(BD)/%,$(_OBJ))


$(BD)/%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


main: $(OBJ) main.c
	$(CC) $(CFLAGS) -c main.c -o $(BD)/main.o
	$(CC) $(CFLAGS) -o $(BD)/main.bin $(OBJ) $(BD)/main.o
	$(OBJCOPY) -j .text -j .data -O ihex $(BD)/main.bin $(BD)/main.hex



flash:
	avrdude -p $(MCU) -c arduino -b $(BRATE) -U flash:w:$(BD)/main.hex:i -F -P $(DEV)



screen:
	screen $(DEV) $(BRATE)



clean:
	rm -f $(BD)/*.bin $(BD)/*.hex
