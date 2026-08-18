
#include "flash.h"
#include "stm32f407xx.h"

#define FLASH_ADDR_BASE 0x40023C00
/*
int Flash_Erase_Sector(char sector)
{


    return -1; // erase failed
    return 0;// erase success
}
*/

void Flash_Erase_Sector(char sector)
{
    // check that no Flash memory operation is ofigoing, wait bit BSY
    u32 *FLASH_SR = (u32*)(FLASH_ADDR_BASE + 0X0C);
    u32 *FLASH_CR = (u32*)(FLASH_ADDR_BASE +  0x10);

    u32 *FLASH_KEYR = (u32*)(FLASH_ADDR_BASE + 0x04);
    if(((*FLASH_CR >> 31) & 1) == 1)
    {
        // unlock CR 
        *FLASH_KEYR =  0x45670123;
        *FLASH_KEYR = 0xCDEF89AB;
    }
    while(((*FLASH_SR >> 16)& 1) == 1);



    *FLASH_CR |= (1 << 1) | (sector << 3);
    *FLASH_CR |= (1 << 16); // start erase operation

    while(((*FLASH_SR >> 16) & 1) == 1); // WAIT BSY is clean

    *FLASH_CR &= ~(1 << 1);

}



void Flash_Program(u8 *addr, u8 value )
{
    u32 *FLASH_SR = (u32*)(FLASH_ADDR_BASE + 0X0C);
    u32 *FLASH_CR = (u32*)(FLASH_ADDR_BASE +  0x10);

    u32 *FLASH_KEYR = (u32*)(FLASH_ADDR_BASE + 0x04);
    if(((*FLASH_CR >> 31) & 1) == 1)
    {
        // unlock CR 
        *FLASH_KEYR =  0x45670123;
        *FLASH_KEYR = 0xCDEF89AB;
    }
    // check that no Flash memory operation is ofigoing, wait bit BSY

    while(((*FLASH_SR >> 16)& 1) == 1);

    // set Flash programming bit resgter FLASH_CR 
    *FLASH_CR |= 1 << 0;
    *addr = value;
    while(((*FLASH_SR >> 16) & 1) == 1);
    *FLASH_CR &= ~(1 << 0);
}