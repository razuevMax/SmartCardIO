# SmartCardIO
This project offers WinSCard API in C++11 with STL, which supports PC/SC reader in Windows/Linux platform

This library is built on top of the WinSCard native library (Windows Smart Card SDK) for Windows, on top of the pcsc-lite library for Linux.

Documentation available on GitHub Pages: https://razuevmax.github.io/SmartCardIO/

A detailed description of all WinSCard API functions is available in the MSDN Library Documentation (https://msdn.microsoft.com/en-us/library/aa374731(v=VS.85).aspx).

A detailed description of pcsc-lite library is available on site https://pcsclite.alioth.debian.org/

This library allows you to transmit and receive application protocol data units (APDUs) specified by ISO/IEC 7816-3 to a smart card.

Compilation of verified -
Windows:
MS Visual Studio 2015

Qt Creator 4.1.0/mingw 5.3 32

Linux (tested on SUSE Linux 14):

Qt Creator 4.1.0/gcc 4

# Project directory structure

/windows/ contains the relevant windows project

/windows/SmartCardIO/ contains the relevant Qt Creator pro file, compatible with mingw32 compiler

/linux/ contains the relevant linux version files (temporary only sources, without makefile or pro-file)

# Requirements
c++11 compiler (desirable C++14)

STL c++11

pcsc-lite library for linux/mac
