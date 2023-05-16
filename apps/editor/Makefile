TARGET=editor
PREFIX=../../stdlib
OPT=2

include $(PREFIX)/Makefile

SRC+=main.c

all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
	riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(TARGET).bin

# riscv64-unknown-elf-objdump -d $(TARGET)
