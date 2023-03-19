:: a replacement for a Makefile for Windows that does not have 'make' available by default
echo off
title Windows does not have 'make'

:: compile '.c' source into '.s' files
c:/cc65/bin/cc65.exe -Oi ogc-lib-os-nrtps/lib-os-nrtps.c --target nes -Iogc-lib-os-nrtps/ -Ic:/cc65/include/ --add-source

:: assemble all '.s' files into object files '.o'
c:/cc65/bin/ca65.exe lib-os-nrtps.s
c:/cc65/bin/ca65.exe crt0.s

:: link all objects '.o' into final '.nes' image
c:/cc65/bin/ld65.exe -C .\nrom_128_horz.cfg crt0.o ogc-nrtps-demo.o -o ogc-nrtps-demo.nes nes.lib

echo You should now have a .nes file to execute if everything worked
:: add 'pause' if you want the window to hang around until user closes it