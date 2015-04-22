PROGRAM = flashlight
MCU = attiny85
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS += -Wall -g -Os -mmcu=$(MCU)
LDFLAGS +=
OBJS = $(PROGRAM).o

PROGRAMMER = usbasp -B 10
#PROGRAMMER = avrisp -P /dev/ttyUSB0

#FUSES = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
FUSES = -F -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

AVRSIZE = avr-size
 
# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif
 
all: $(PROGRAM).hex
 
$(PROGRAM).elf: $(PROGRAM).o
	@printf "  LD      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
 
$(PROGRAM).hex: $(PROGRAM).elf
	@printf "  OBJCOPY $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(OBJCOPY) -O ihex $< $@
	@$(AVRSIZE) -C --mcu=$(MCU) $(PROGRAM).elf
 
%.o: %.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<
 
flash: $(PROGRAM).hex
	@printf "  FLASH   $(PROGRAM).hex\n"
	$(Q)avrdude -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(PROGRAM).hex $(FUSES)

clean:
	@printf "  CLEAN   $(subst $(shell pwd)/,,$(OBJS))\n"
	$(Q)rm -f $(OBJS)
	@printf "  CLEAN   $(PROGRAM).elf\n"
	$(Q)rm -f *.elf
	@printf "  CLEAN   $(PROGRAM).hex\n"
	$(Q)rm -f *.hex
