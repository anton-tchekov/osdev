TARGET=test
OPT=2

CFLAGS=\
	-march=rv32im \
	-mabi=ilp32 \
	-Wl,-verbose \
	-nostdlib \
	-ffreestanding \
	-O$(OPT)

SRC=\
	main.c


all:
	riscv64-unknown-elf-gcc $(CFLAGS) $(SRC) -o $(TARGET) -T$(TARGET).ld
	riscv64-unknown-elf-objdump -d $(TARGET)
	riscv64-unknown-elf-objcopy -O binary $(TARGET) $(TARGET).bin

clean:
	rm -f test

