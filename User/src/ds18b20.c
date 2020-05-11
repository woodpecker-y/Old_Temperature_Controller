#include <stdio.h>
#define _DS18B20_C_
#include"DS18B20.h"
#include "bsp.h"
#include "sysparams.h"

u8  ReadSlot (void)
{
	u8 i;

	DQ_BIT_W0;
	DQ_BIT_InPut;

        delay_us(5);
	i=DQ_BIT_Read;
//	Delay_5us(60/5);
        delay_us(60);
	DQ_BIT_OutPut;
	DQ_BIT_W1;
	return i;
}



u8 Read_One_Byte(void)       //è¯»å–ä¸€ä¸ªå­—èŠ‚çš„æ•°æ®read a byte date
{
	u8 i,j=0x01,byte=0;
	for(i=0;i<8;i++)
	{
		if(ReadSlot()) byte=byte | j;
		j=j<<1;
	}
	return byte;
}

void WriteSlot (u8 i)
{

	DQ_BIT_W0;
	if(i)
	{
		DQ_BIT_W1;
	}
//	Delay_5us(60/5);
        delay_us(60);
	DQ_BIT_W1;
}
void Write_One_Byte(u8 byte) //å†™å…¥ä¸€ä¸ªå­—èŠ‚çš„æ•°æ®write a byte date
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte & 0x01)
		{
			WriteSlot(1);
		}
		else
		{
			WriteSlot(0);
		}
		byte=byte>>1;
	}
}

u8 DS18B20_Init(void)     //DS18B20åˆå§‹åŒ–send reset and initialization command
{
	u8 i;
	s8 tryTimes;
	u8 backs =0x00;

	/* æ‹‰ä½æ€»çº¿å¤ä½ */
	DQ_BIT_OutPut;
	DQ_BIT_W0;
//	Delay_5us(720/5);
        delay_us(720);

	/* é‡Šæ”¾æ€»çº¿ */
	DQ_BIT_InPut;

    /* æ£€æµ‹480å¾®ç§’å†…æœ‰æ— ä½ç”µå¹³ï¼Œåˆ¤æ–­18B20æ˜¯å¦å­˜åœ¨ */
    tryTimes = 24;
    backs =0x00;
    while(tryTimes--)
    {
		i = DQ_BIT_Read;
		if(i == 0)
		{
			backs =0x01;
			break;
		}
//		Delay_5us(15/5);	//ç²¾ç¡®å»¶æ—¶20us
                delay_us(15);
    }

	/* æ£€æµ‹480å¾®ç§’å†…æœ‰æ— ä½ç”µå¹³ï¼Œåˆ¤æ–­18B20æ˜¯å¦å­˜åœ¨ */
    tryTimes = 24;
    while(tryTimes--)
    {
		i = DQ_BIT_Read;
		if(i == 1)
		{
			break;
		}
//		Delay_5us(15/5);//ç²¾ç¡®å»¶æ—¶20us
                delay_us(15);
    }
	DQ_BIT_OutPut;
	DQ_BIT_W1;
	return backs;

}



void DS18B20_Start(void)
{

//	disableInterrupts();						//æ€»ä¸­æ–­æ§åˆ¶

	DS_18B20Controler.DS18b20Flag =DS18B20_Init();	//åˆå§‹åŒ–æ¸©åº¦ä¼ æ„Ÿå™¨
//        printf("DS_18B20Controler.DS18b20Flag = %d\r\n", DS_18B20Controler.DS18b20Flag);
	Write_One_Byte(0XCC);					//å¿½ç•¥ROMæŒ‡ä»¤
	Write_One_Byte(0x44);					//æ¸©åº¦è½¬æ¢æŒ‡ä»¤
//	enableInterrupts();						//æ€»ä¸­æ–­æ§åˆ¶

}




s16 DS18B20_ReadTemp(void)
{
	s16 Temp;
	s32 TempBuffer =0;
	u8 a =0,b =0;
	s16 TempOutPut =0;
        int i = 0;
        char data[9] = {0};

//	disableInterrupts();					//æ€»ä¸­æ–­æ§åˆ¶	
	DS18B20_Init();
	Write_One_Byte(0XCC);          		//å¿½ç•¥ROMæŒ‡ä»¤
	Write_One_Byte(0XBE);          		//è¯»æš‚å­˜å™¨æŒ‡ä»¤

        for (i = 0; i < 9; i++)
        {
          data[i] = Read_One_Byte();
        }
        a = data[0];
        b = data[1];
//	a = Read_One_Byte();           		//è¯»å–åˆ°çš„è¯¥å­—èŠ‚ä¸ºæ¸©åº¦LSB
//	b = Read_One_Byte();           		//è¯»å–åˆ°çš„è¯¥å­—èŠ‚ä¸ºæ¸©åº¦MSB

	Temp = b<<8;               		//å…ˆæŠŠé«˜å…«ä½æœ‰æ•ˆæ•°æ®èµ‹äºtemp
	Temp = Temp|a;          			//ä¸¤å­—èŠ‚åˆæˆä¸€ä¸ªæ•´å‹å˜é‡æµ‹é‡ç²¾åº¦ä¸º1/16  å³æŠŠå®é™…æ¸©åº¦æ‰©å¤§ä¸º16å€

	TempBuffer =Temp*100l/16l;			//å¾—åˆ°çœŸå®ç²¾ç¡®åˆ°0.01åº¦  æ¸©åº¦ä¼ æ„Ÿå™¨æœ€ä½æœ‰æ•ˆä½æ˜¯0.0625(1/16)


	TempOutPut =(s16)TempBuffer;
        if(TempOutPut<0)
        TempOutPut = 0;

	return TempOutPut;

}




void DS18B20_GetTemp_Server(void)//æ¸©åº¦è·å–æœåŠ¡å‡½æ•°
{
    s16 Temp_Buffer =0;


    DS18B20_Start(); 					//ä¼ æ„Ÿå™¨é…ç½®å‘½ä»¤
    // ÕâÀïĞèÒªÅĞ¶ÏÏÂ³õÊ¼»¯ÊÇ·ñ³É¹¦
        Temp_Buffer =DS18B20_ReadTemp();	//æ¸©åº¦è¯»å–å‘½ä»¤
//        LCD_Temp_Display(Temp_Buffer, INPUT, WRITE);    // ÔİÊ±
        printf("Temp_Buffer:%d\r\n", Temp_Buffer);
        if((Temp_Buffer<0)||(Temp_Buffer>=10000))
        {
                Temp_Buffer= 0;
        }

        g_run_params.temp = Temp_Buffer;




}

/*
	å‡½æ•°åŠŸèƒ½:DS18B20 åŠŸèƒ½æœåŠ¡ æ¨¡å—ä¸Šåˆå§‹åŒ– åŒ…å«å¯¹æ•°æ®åº“çš„è¿æ¥ä¸é…ç½®
	@Para	ScanPara:æ‰«æå‘¨æœŸ(é—´éš”)
	@Para	*Clock:    æ—¶åŸºåœ°å€
*/

void DS18B20_PowerON_Init(void)
{
    s16 Temp_Buffer =0;
    Temp_Buffer =Temp_Buffer;
    DS18B20_Start();
    Temp_Buffer =DS18B20_ReadTemp();
    printf("Temp_Buffer_main = %d\r\n", Temp_Buffer);
    DS_18B20Controler.Sys_Temp =0;
}

/*

	å¹³å‡æ¸©åº¦è®°å½•ç‚¹è¾“å…¥å‡½æ•°ï¼Œå°†æ‰€å¾—åˆ°çš„å½“å‰æ¸©åº¦è¿›è¡Œå­˜å‚¨

	æœåŠ¡å˜é‡ AVE_TempData

*/
void AVETemp_PointInput(s16* Temp)
{
	AVE_TempData.Buffer[AVE_TempData.Count]=*Temp;
	AVE_TempData.Count +=1;
	if(AVE_TempData.Count >=AVEtemp_BufferSize)
	{
		AVE_TempData.Count =0;
		AVE_TempData.StackFull =0X01;
	}
}


/*
	æ±‚å¹³å‡æ¸©åº¦
	å‚æ•°:ç³»ç»Ÿæœ€è¿‘ä¸€æ¬¡è·å–çš„æ¸©åº¦å€¼æŒ‡é’ˆ

*/
s16 AVETemp_Calculate(s16* Temp)
{
	s16 Ave_Value =0;
	u8 loop =0;
	AVE_TempData.Temp_Add =0;
	AVE_TempData.Buffer[AVE_TempData.Count]=*Temp;
	AVE_TempData.Count +=1;

	if(AVE_TempData.StackFull ==0X01)	//å †æ ˆåŒºæ»¡
	{
		for(loop=0;loop<AVEtemp_BufferSize;loop++ )
		{
			AVE_TempData.Temp_Add +=AVE_TempData.Buffer[loop];
		}
		Ave_Value=AVE_TempData.Temp_Add/AVEtemp_BufferSize;
	}
	else
	{
		if(AVE_TempData.Count ==0)
		{
			Ave_Value =*Temp;
		}
		else
		{
			for(loop=0;loop<AVE_TempData.Count;loop++ )
			{
				AVE_TempData.Temp_Add +=AVE_TempData.Buffer[loop];
			}
			Ave_Value=AVE_TempData.Temp_Add/AVE_TempData.Count;
		}
	}
	return Ave_Value;
}

void ds18b20_close(void)
{
//  GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_Out_OD_Low_Slow);       // µÍ¹¦ºÄ²âÊÔ
  GPIO_Init(DS18B20_DQ_PORT,DS18B20_DQ_PIN,GPIO_Mode_Out_PP_High_Slow);       // µÍ¹¦ºÄ²âÊÔ
}

// Ö÷»ú·¢ËÍ¸´Î»Âö³å£¨´óÓÚ480usµÍµçÆ½£©£¬²¢¼ì²é´æÔÚÂö³å
unsigned char ow_reset(void) {
	unsigned char presence;
	DQ_BIT_W0;             //Ö÷»úÀ­µÍDQ×ÜÏß
	delay_us(480);     // À­µÍDQ×ÜÏß³ÖĞø480us
	DQ_BIT_W1;            // Ö÷»úÊÍ·Å×ÜÏß£¬ÓÉÉÏÀ­µç×èÀ­¸ß
        DQ_BIT_InPut;
	delay_us(80);      // µÈ´ı80usºó¼ì²épresence
	presence = DQ_BIT_Read;     // ¼ì²é´æÔÚÂö³å
	delay_us(15);      // Ê±ĞòÄ©Î²µÈ´ı
        printf("presence = %d\r\n", presence);
        printf("GPIO_ReadInputDataBit = %d\r\n", BSP_GPIO_ReadState(PORTF, PIN1));
	return(presence);
} // 0 = MY18E20´æÔÚ, 1 = MY18E20²»´æÔÚ

//// READ_BIT ¨C ´ÓMY18E20¶ÁÈ¡Ò»Î»Êı
unsigned char read_bit(void) {
	unsigned char res;
	DQ_BIT_W0;          //Ö÷»úÀ­µÍDQ×ÜÏß
	delay_us(5);     //À­µÍDQ×ÜÏß³ÖĞø5us
	DQ_BIT_W1;          //Ö÷»úÊÍ·ÅDQ×ÜÏß£¬ÓÉÉÏÀ­µç×èÀ­¸ß
        DQ_BIT_InPut;
	delay_us(2);    // µÈ´ı2usÈ·±£×ÜÏßÉÏÊı¾İÎÈ¶¨
	res = DQ_BIT_Read;
	delay_us(50);    //Ê±ĞòÄ©Î²µÈ´ı
	return(res);    // ·µ»Ø¶ÁÈ¡ÊıÖµ
}

//// WRITE_BIT ¨C Ö÷»úĞ´Ò»Î»Êı¾İµ½MY18E20
void write_bit(char bitval) {
	DQ_BIT_W0;              //Ö÷»úÀ­µÍDQ×ÜÏß
	delay_us(5);         //À­µÍDQ×ÜÏß³ÖĞø5us
	if(bitval==1) DQ_BIT_W1; // Èç¹ûĞ´1£¬´ËÊ±À­¸ßDQ×ÜÏß
	delay_us (60);       // Èç¹ûĞ´0£¬Ôò³ÖĞøÀ­µÍDQ×ÜÏß60us
	DQ_BIT_W1;              //ÊÍ·ÅDQ×ÜÏß
	delay_us (5);        //£¨Ğ´0£©»Ö¸´Ê±¼ä1us
}

//// READ_BYTE - ´ÓMY18E20¶ÁÈ¡Ò»×Ö½ÚÊı¾İ
unsigned char read_byte(void) {
	unsigned char i, j;
	unsigned char value = 0;
	for (i=0;i<8;i++) {
	j = read_bit();
	value = (value) | (j<<i);  //Ã¿´Î¶Á½ø1 Î»(µÍÎ»ÏÈĞĞ), È»ºó×óÒÆ
	}
	return(value);
}

//// WRITE_BYTE - Ö÷»úĞ´Ò»×Ö½ÚÊı¾İµ½MY18E20
void write_byte(char val) {
	unsigned char i;
	unsigned char temp;
	for (i=0; i<8; i++) {  // Ğ´×Ö½Ú,Ã¿´ÎÒ»Î»
		temp = val>>i;
		temp &= 0x01;   // µÍÎ»ÏÈĞĞ
		write_bit(temp);
	}
}

//
void sample_temp(void)
{
	unsigned char get[10];
	unsigned char tpmsb, tplsb, i;
 	short s_tem;
 	float f_tem;

	ow_reset();
	write_byte(0xCC);  //Skip ROM£¬»òÊ¹ÓÃÆ¥ÅäROMĞòÁĞºÅ£¨¼ûºóÎÄ£©
	write_byte(0x44);  // ×ª»»ÎÂ¶È
	delay_ms (500);
        // ÎÂ¶È×ª»»Ê±¼ä£¬ÆäÖĞ¿ìËÙÄ£Ê½Îª15ms£¬ÂıËÙÄ£Ê½Îª500ms

	ow_reset();
	write_byte(0xCC); // Skip ROM£¬»òÊ¹ÓÃÆ¥ÅäROMĞòÁĞºÅ£¨¼ûºóÎÄ£©
	write_byte(0xBE); // ¶ÁÈ¡Scratch PadÖĞÊıÖµ

	for (i=0;i<9;i++){
		get[i]=read_byte();
	}

	tpmsb = get[1];   // ÎÂ¶È¸ß×Ö½Ú
	tplsb = get[0];   // ÎÂ¶ÈµÍ×Ö½Ú
        printf("tpmsb = %0x, tplsb = %0x", tpmsb, tplsb);
	s_tem = tpmsb<<8;
 	s_tem = s_tem | tplsb;
 	s_tem = s_tem & 4095;

 	if( (s_tem & 2048) == 2048 )	{
 		s_tem = (s_tem^4095) & 4095;
 		f_tem = -1*(s_tem+1) * 0.0625;  //ÎÂ¶ÈÎª¸ºÖµ
 	}
 	else
    {
        f_tem = s_tem * 0.0625;
    }
//	printf( "\nTempF= %f degrees C\n", f_tem); //´òÓ¡ÉãÊÏÎÂ¶È
//        printf("temp = %f\r\n", f_tem);
//        printf("temp = %d\r\n", f_tem);
}








