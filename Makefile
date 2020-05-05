TARGET_APPLICATION=App
TARGET_DRIVER=ProcessorView
SRC=TestPkg
BUILD=./Build
DIR_EDK2=..
HDA_DISK=fat:rw:$(BUILD)/hda-content

env:
	mkdir -p Build/hda-content

build:
	build -p $(DIR_EDK2)/$(SRC)/$(SRC).dsc
	make copy

COMPILE=NOOPT_GCC5/X64
INIT_BUILD=$(DIR_EDK2)/Build/$(SRC)
copy:
	# Copy application
	cp $(INIT_BUILD)/$(COMPILE)/$(TARGET_APPLICATION).efi $(BUILD)/hda-content/
	# Copy driver
	cp $(INIT_BUILD)/$(COMPILE)/$(TARGET_DRIVER).efi $(BUILD)/hda-content/

start:
	make copy
	sudo qemu-system-x86_64 -bios $(BUILD)/run-ovmf/bios.bin -net none -s -hda $(HDA_DISK) -m 5000

clean:
	rm -rf $(BUILD)/hda-content/*
	rm -r $(INIT_BUILD)
