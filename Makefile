CC := gcc
CFLAGS := -g -O2

LD := ld
LDFLAGS := 

# NOT the kernel or mmload version.
MMOS_FULL_VERSION := 0.1.0

# The path is hardcoded in the bootloader. Ensure you change it there too.
SRC_FONT_PATH := assets/fonts/zap-ext-light20.psf

BUILD_DIR := bin
BUILD_LOADER_NAME := BOOTX64.EFI
BUILD_KERNEL_NAME := mmkernel
BUILD_BIMAGE_NAME := mmos_v$(MMOS_FULL_VERSION)_$(shell date +%Y-%b-%d).img

OVMF_SYSTEM_DIR := /usr/share/ovmf/x64
OVMF_LOCAL_DIR := ovmf
OVMF_CODE := OVMF_CODE.4m.fd
OVMF_VARS := OVMF_VARS.4m.fd

override LOADER_SRC_ALL := $(shell cd src/bootloader && find -L . -type f | LC_ALL=C sort)
override LOADER_SRC_C := $(filter %.c,$(LOADER_SRC_ALL))
override LOADER_OBJ := $(addprefix obj/bootloader/,$(LOADER_SRC_C:.c=.o))

override KERNEL_SRC_ALL := $(shell cd src/kernel && find -L . -type f | LC_ALL=C sort)
override KERNEL_SRC_C := $(filter %.c,$(KERNEL_SRC_ALL))
override KERNEL_OBJ := $(addprefix obj/kernel/,$(KERNEL_SRC_C:.c=.o))

all: bootloader kernel bootimg

prepare: clean copy-ovmf make-gnuefi

make-gnuefi:
	make -C gnu-efi

clean:
	rm -rf $(BUILD_DIR) obj

.PHONY: bootloader
bootloader: $(BUILD_DIR)/$(BUILD_LOADER_NAME)
$(BUILD_DIR)/$(BUILD_LOADER_NAME): $(LOADER_OBJ) loader_link

.PHONY: kernel
kernel: $(BUILD_DIR)/$(BUILD_KERNEL_NAME)
$(BUILD_DIR)/$(BUILD_KERNEL_NAME): $(KERNEL_OBJ) kernel_link

obj/bootloader/%.o: src/bootloader/%.c
	@echo " ===> Compiling loader file $<"
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c $< -o $@

obj/kernel/%.o: src/kernel/%.c
	@echo " ===> Compiling kernel file $<"
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -ffreestanding -fshort-wchar -fno-stack-protector -fno-stack-check -mno-red-zone -c $< -o $@

loader_link:
	@echo " ===> Linking loader"
	mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -shared -Bsymbolic -Lgnu-efi/x86_64/gnuefi -Lgnu-efi/x86_64/lib -Tgnu-efi/gnuefi/elf_x86_64_efi.lds gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o $(LOADER_OBJ) -o obj/mmload.so -lgnuefi -lefi
	objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 obj/mmload.so $(BUILD_DIR)/$(BUILD_LOADER_NAME)

kernel_link:
	@echo " ===> Linking kernel"
	mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -Bsymbolic -nostdlib -T src/kernel/linker.ld -o $(BUILD_DIR)/$(BUILD_KERNEL_NAME) $(KERNEL_OBJ)

bootimg:
	@echo " ===> Creating bootable image ($(BUILD_DIR)/$(BUILD_BIMAGE_NAME))"
	mkdir -p $(BUILD_DIR)
	dd if=/dev/zero of=$(BUILD_DIR)/$(BUILD_BIMAGE_NAME) bs=512 count=93750
	mkfs.fat -F 32 $(BUILD_DIR)/$(BUILD_BIMAGE_NAME)
	
	mmd -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) ::/EFI
	mmd -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) ::/EFI/BOOT
	mmd -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) ::/kernel_resources
	mmd -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) ::/kernel_resources/fonts

	mcopy -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) $(BUILD_DIR)/$(BUILD_LOADER_NAME) ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) $(BUILD_DIR)/$(BUILD_KERNEL_NAME) ::
	mcopy -i $(BUILD_DIR)/$(BUILD_BIMAGE_NAME) $(SRC_FONT_PATH) ::/kernel_resources/fonts

copy-ovmf:
	@echo " ===> Copying OVMF vars from $(OVMF_SYSTEM_DIR) to $(OVMF_LOCAL_DIR)"
	mkdir -p ovmf
	cp -v $(OVMF_SYSTEM_DIR)/$(OVMF_VARS) $(OVMF_LOCAL_DIR)

qemurun:
	qemu-system-x86_64 \
		-drive file=$(BUILD_DIR)/$(BUILD_BIMAGE_NAME),if=none,id=drive0 \
		-device ahci,id=ahci0 \
		-device ide-hd,drive=drive0,bus=ahci0.0 \
		-m 256M \
		-machine q35 \
		-cpu qemu64 \
		-drive if=pflash,format=raw,unit=0,file="$(OVMF_SYSTEM_DIR)/$(OVMF_CODE)",readonly=on \
		-drive if=pflash,format=raw,unit=1,file="$(OVMF_LOCAL_DIR)/$(OVMF_VARS)" \
		-net none

qemurun-gdb:
	qemu-system-x86_64 \
		-s -S \
		-drive file=$(BUILD_DIR)/$(BUILD_BIMAGE_NAME),if=none,id=drive0 \
		-device ahci,id=ahci0 \
		-device ide-hd,drive=drive0,bus=ahci0.0 \
		-m 256M \
		-machine q35 \
		-cpu qemu64 \
		-drive if=pflash,format=raw,unit=0,file="$(OVMF_SYSTEM_DIR)/$(OVMF_CODE)",readonly=on \
		-drive if=pflash,format=raw,unit=1,file="$(OVMF_LOCAL_DIR)/$(OVMF_VARS)" \
		-net none
