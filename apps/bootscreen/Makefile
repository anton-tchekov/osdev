TARGET=bootscreen
PREFIX=../../stdlib
OPT=2

include $(PREFIX)/Makefile

all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

#riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin
#	rm -f $(TARGET)

# riscv64-unknown-elf-objdump -d $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(TARGET).bin
