GPP_PARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
AS_PARAMS = --32
LD_PARAMS = -melf_i386

objects = obj/loader.o \
					obj/gdt.o \
					obj/drivers/driver.o \
					obj/hardware/port.o \
					obj/hardware/interruptstubs.o \
					obj/hardware/interrupts.o \
					obj/hardware/pci.o \
					obj/drivers/keyboard.o \
					obj/drivers/mouse.o \
					obj/drivers/vga.o \
					obj/gui/widget.o \
					obj/gui/window.o \
					obj/gui/desktop.o \
					obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPP_PARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(AS_PARAMS) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(LD_PARAMS) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "CITOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: kernel.iso
	/mnt/d/Program\ Files/qemu/qemu-system-x86_64.exe -hda kernel.iso -net nic,model=ne2k_pci
	# ("/mnt/c/Program Files/Oracle/VirtualBox/VBoxManage.exe" controlvm "CITOS" poweroff && sleep 1) || true
	# "/mnt/c/Program Files/Oracle/VirtualBox/VirtualBoxVM.exe" --startvm "CITOS" & 

.PHONY: clean
clean:
	rm -rf obj kernel.bin kernel.iso