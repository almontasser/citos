GPP_PARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_PARAMS = --32
LD_PARAMS = -melf_i386

objects = loader.o gdt.o port.o interruptstubs.o interrupts.o keyboard.o kernel.o

%.o: %.cpp
	g++ $(GPP_PARAMS) -o $@ -c $<

%.o: %.s
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
	/mnt/d/Program\ Files/qemu/qemu-system-x86_64.exe -hda kernel.iso
#	("/mnt/c/Program Files/Oracle/VirtualBox/VBoxManage.exe" controlvm "CITOS" poweroff && sleep 1) || true
#	"/mnt/c/Program Files/Oracle/VirtualBox/VirtualBoxVM.exe" --startvm "CITOS" & 

.PHONY: clean
clean:
	rm -rf *.o kernel.bin kernel.iso