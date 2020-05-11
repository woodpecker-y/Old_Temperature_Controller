#include<stdlib.h>
#include "adf.h"
#include "encrypt.h"
#include "utili.h"
#include "verify.h"
#include "dbg.h"
#include "wdg.h"
#include "utili.h"
#include "uid.h"

__no_init const u8 feature_codes[72] @0x8764;
__no_init const u8 B[6]   @0x8D60;
__no_init const u8 L[2]   @0x9FA0;
__no_init const u8 A[2]    @0x9270;
__no_init const u8 C[1]    @0x9570;
__no_init const u8 K[4]    @0x9890;
//__no_init const u8 A_2[125] @0x8700;
//__no_init const u8 N[178]   @0xA200;
//__no_init const u8 T[26]    @0xB078;
//__no_init const u8 S[156]   @0xC520;

void chip_verify_init(u8 *dat, u16 len, u8 seed)
{
    u16 i = 0;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    /* Unlock flash data eeprom memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Wait until Program Data area unlocked flag is set*/
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
    {}
    
    srand(seed);
    
    // 'B'
    for (i=0; i<sizeof(B); ++i)
    {
        FLASH_ProgramByte((u32)B+i, rand());
    }   
    
    // 'L'    
    for (i=0; i<sizeof(L); ++i)    
    {
        FLASH_ProgramByte((u32)L+i, rand());
    }
    
    // 'A'
//    for (i=0; i<sizeof(A); ++i)
//    {
//        FLASH_ProgramByte((u32)A+i, rand());
//    }
//    
//    // 'C'
//    for (i=0; i<sizeof(C); ++i)
//    {
//        FLASH_ProgramByte((u32)C+i, rand());
//    }    
//    
//    // 'K'    
//    for (i=0; i<sizeof(K); ++i)
//    {
//        FLASH_ProgramByte((u32)K+i, rand());
//    }    
    
    //    // 'A'    
    //    for (i=0; i<sizeof(A_2); ++i)
    //    {
    //        FLASH_ProgramByte((u32)A_2+i, rand());
    //    }
    //    
    //    // 'N'    
    //    for (i=0; i<sizeof(N); ++i)
    //    {
    //        FLASH_ProgramByte((u32)N+i, rand());
    //    }
    //    
    //    // 'T'
    //    for (i=0; i<sizeof(T); ++i)
    //    {
    //        FLASH_ProgramByte((u32)T+i, rand());
    //    }    
    //    
    //    // 'S'
    //    for (i=0; i<sizeof(S); ++i)
    //    {
    //        FLASH_ProgramByte((u32)S+i, rand());
    //    }  
    
    // ÌØÕ÷Âë
    FLASH_ProgramByte(0x8764, 0x2C);
    
    for (i=0; i<len; ++i)
    {
        FLASH_ProgramByte(0x8765+i, dat[i]);
    }    
    
    FLASH_Lock(FLASH_MemType_Program); 
}

void chip_destroy(void)
{
    u16 i = 0;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    /* Unlock flash data eeprom memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Wait until Program Data area unlocked flag is set*/
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
    {}
    
    for (i=0; i<0xFFF; ++i)
    {
        FLASH_ProgramByte(0x9000+i, 0);
    }
    
    FLASH_Lock(FLASH_MemType_Program);         
}

void chip_verify(void)
{
    u8 i = 0;
    u8 uid[16] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xCA, 0xC2, 0xCA, 0xC9};        
    u8 val[72] = {0xC1, 0xC6, 0xCC, 0xC7, 0xC7, 0xDA, 0xA8, 0xFE, 0xB9, 0xA6, 0xB8, 0x85, 0x82, 0xDF, 0xDF, 0xDF, 0xA6, 0xCA, 0xC4, 0xC9, 0xCB, 0xC3, 0xC9, 0xC6, 0xDC, 0xDB, 0xA6, 0xCB, 0xC7, 0xC5, 0xA6, 0xCB, 0xC6, 0x85, 0x82, 0x39, 0x39, 0x36, 0x21, 0x32, 0x52, 0x4A, 0x64, 0x5A, 0x47, 0x35, 0x52, 0x4C, 0x54, 0x34, 0x34, 0x42, 0x7D, 0x5B, 0x58, 0x47, 0x56, 0x31, 0x23, 0x43, 0x36, 0x38, 0x6E, 0x40, 0x20, 0x43, 0x71, 0x5B, 0x58, 0x85, 0x82, 0x88};
    
    uid_get(uid);
    
    //MYLOG_DEBUG_HEXDUMP("uid:", uid, sizeof(uid)); 
    
    for (i=12; i<sizeof(uid); ++i)
    {
        uid[i] ^= (u8)(crc_tab16[224]>>8);    
    }
    
    //MYLOG_DEBUG_HEXDUMP("UID:", uid, sizeof(uid));        
    
    for (i=0; i<sizeof(val); ++i)
    {
        val[i] ^= (u8)(crc_tab16[224]>>8);    
    }    
    
    //MYLOG_DEBUG_HEXDUMP("Ã÷ÎÄ:", val, sizeof(val));    
    
    for (i=0; i<sizeof(val)/8; ++i)
    {
        encrypt(val+8*i, 8, uid, 16);
    }
    
    //MYLOG_DEBUG_HEXDUMP("mi1:", val, sizeof(val)); 
    //MYLOG_DEBUG_HEXDUMP("mi2:", (unsigned char*)feature_codes+1, sizeof(val)); 
    
  
    switch(feature_codes[0])
    {
    case 0x2C:
        if (memcmp(val, feature_codes+1, sizeof(val)) == 0)
        {
           // MYLOG_DEBUG("Verify OK\r\n");
        }
        else
        {
           // MYLOG_DEBUG("Chip destroyed!\r\n");
            chip_destroy();
            while (1);
        }
        break;
    default:
        
        delay_ms(5000);
        //MYLOG_DEBUG("Verify Init2\r\n"); 
        chip_verify_init(val, sizeof(val), uid[5]);
        //MYLOG_DEBUG("Verify Init\r\n");        
        break;        
    }
}



