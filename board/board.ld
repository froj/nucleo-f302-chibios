/*
 * STM32F303xC memory setup.
 * to use with bootloader change the LDSCRIPT to board_with_bootloader.ld in the Makefile
 */
MEMORY
{
    flash : org = 0x08000000, len = 32K
    ram : org = 0x20000000, len = 16k
    ccmram : org = 0x10000000, len = 8k
}

INCLUDE ChibiOS/os/common/ports/ARMCMx/compilers/GCC/rules.ld
