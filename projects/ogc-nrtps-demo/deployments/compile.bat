:: a replacement for a Makefile for Windows that does not have 'make' available by default
echo off
title Windows does not have 'make'

:: compile '.c' source into '.s' files
c:/cc65/bin/cc65.exe -Oi ../software/libraries/ogc_lib_os_nrtps/ogc_lib_os_nrtps.c --target nes -I../software/librarires/ogc_lib_os_nrtps/ -I../firmware/nes/ -I../hardware/nes/ -Ic:/cc65/include/ --add-source
c:/cc65/bin/cc65.exe -Oi ../software/applications/ogc_app_demo.c --target nes -I../software/libraries/ogc_lib_os_nrtps/ -Ic:/cc65/include/ --add-source

:: assemble all '.s' files into object files '.o'
c:/cc65/bin/ca65.exe ../software/libraries/ogc_lib_os_nrtps/ogc_lib_os_nrtps.s
c:/cc65/bin/ca65.exe ../software/applications/ogc_app_demo.s
c:/cc65/bin/ca65.exe ../firmware/nes/crt0.s
c:/cc65/bin/ca65.exe ../firmware/nes/nmi.s
c:/cc65/bin/ca65.exe ../firmware/nes/irq.s
c:/cc65/bin/ca65.exe ../firmware/nes/map.s

:: link all objects '.o' into final '.nes' image
c:/cc65/bin/ld65.exe -C ../hardware/nes/nrom_128_horz.cfg ../firmware/nes/crt0.o ../firmware/nes/irq.o ../firmware/nes/nmi.o ../software/applications/ogc_app_demo.o ../software/libraries/ogc_lib_os_nrtps/ogc_lib_os_nrtps.o -o ogc-nrtps-demo.nes

echo You should now have a .nes file to execute if everything worked
:: add 'pause' if you want the window to hang around until user closes it
pause
