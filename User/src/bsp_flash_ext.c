#ifdef HAVE_FLASH_EXT

#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "bsp_flash_ext.h"
#include "bsp_gpio.h"

static FlashConfig *s_flash_cfg_handler		= NULL;
static u32   s_flash_max_phy_pages	= 0;

/*! \brief  SPI管脚初始化 */
static void BSP_FLASH_EXT_SPI_PortInit(void);
/*! \brief 设置模拟SPI口NSS引脚 */
static void BSP_FLASH_EXT_SPI_NssSet(u8 val);
/*! \brief 设置模拟SPI口Sck引脚 */
static void BSP_FLASH_EXT_SPI_SckSet(u8 val);
/*! \brief 设置模拟SPI口MOSI引脚 */
static void BSP_FLASH_EXT_SPI_MosiSet(u8 val);
/*! \brief 读模拟SPI口miso引脚数据 */
static u8 BSP_FLASH_EXT_SPI_MisoRead(void);
/*! \brief 通过模拟SPI口写数据 */
static void BSP_FLASH_EXT_SPI_WriteData(unsigned char uData);
/*! \brief 通过模拟SPI口读数据 */
static unsigned char BSP_FLASH_EXT_SPI_ReadData(void);
/*! \brief flash操作延时 */
static void BSP_FLASH_EXT_Delay(void);
/*! \brief flash芯片写操作使能 */
static void BSP_FLASH_EXT_WriteEnable(void);
/*! \brief flash芯片写操作禁止 */
static void BSP_FLASH_EXT_WriteDisable(void);
/*! \brief flash读状态寄存器 */
static u8  BSP_FLASH_EXT_ReadRegStatus(void);
/*! \brief flash等待写操作完成 */
static void BSP_FLASH_EXT_WaitWriteComplete(void);

/*! \brief
*       记载FLASH配置参数
* \param cfg[IN]			- flash配置参数
*/
void BSP_FLASH_EXT_LoadConfig(FlashConfig *cfg)
{
    s_flash_cfg_handler = cfg;
}

/*! \brief
*       初始化Flash
*/
void BSP_FLASH_EXT_Init(void)
{
    BSP_FLASH_EXT_SPI_PortInit();
    // 延时等待
    //BSP_FLASH_EXT_Delay();
}

/*! \brief
*       读Flash类型ID
* \return
*		value						- 返回Flash类型码
*		- E_FLASH_TYPE_M25PE16		- ST 2M
*		- E_FLASH_TYPE_M25PE32		- ST 4M
*		- E_FLASH_TYPE_M25PE64		- ST 8M
*		- E_FLASH_TYPE_W25Q40BV		- Winbond 512K
*		- E_FLASH_TYPE_W25Q32FV		- Winbond 4M
*		- E_FLASH_TYPE_W25Q64FV		- Winbond 8M
*		- E_FLASH_TYPE_W25Q128FV	- Winbond 16M		
*		- E_FLASH_TYPE_GD25Q16B		- GD 2M
*		- E_FLASH_TYPE_GD25Q32B		- GD 4M
*/		
u32 BSP_FLASH_EXT_ReadID(void)
{
    u32 flash_id = 0;
    
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_READ_ID);
    // 读数据
    flash_id = BSP_FLASH_EXT_SPI_ReadData();
    flash_id = (flash_id<<8) + BSP_FLASH_EXT_SPI_ReadData();
    flash_id = (flash_id<<8) + BSP_FLASH_EXT_SPI_ReadData();
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    
//    if (E_FLASH_TYPE_M25PE16 == flash_id)
//    {
//        s_flash_max_phy_pages = 8192;
//    }
//    else if (E_FLASH_TYPE_M25PE32 == flash_id)
//    {
//        s_flash_max_phy_pages = 8192*2;
//    }
//    else if (E_FLASH_TYPE_M25PE64 == flash_id)
//    {
//        s_flash_max_phy_pages = 32768;
//    }
//    else if (E_FLASH_TYPE_W25Q40BV == flash_id)
//    {
//        s_flash_max_phy_pages = 2048;
//    }
//    else if (E_FLASH_TYPE_W25Q32FV == flash_id)
//    {
//        s_flash_max_phy_pages = 8192*2;
//    }
//    else if (E_FLASH_TYPE_W25Q64FV == flash_id)
//    {
//        s_flash_max_phy_pages = 32768;
//    }
//    else if (E_FLASH_TYPE_W25Q128FV == flash_id)
//    {
//        s_flash_max_phy_pages = 65536;
//    }   
//    else if (E_FLASH_TYPE_W25Q128FV == flash_id)
//    {
//        s_flash_max_phy_pages = 8192;
//    }  
//    else if (E_FLASH_TYPE_GD25Q16B == flash_id)
//    {
//        s_flash_max_phy_pages = 8192*2;
//    }  
//    else
//    {
//        s_flash_max_phy_pages = 0;
//    }      
    
    switch(flash_id)
    {
    case E_FLASH_TYPE_M25PE16:
        s_flash_max_phy_pages = 8192;
        break;
    case E_FLASH_TYPE_M25PE32:
        s_flash_max_phy_pages = 8192*2;
        break;
    case E_FLASH_TYPE_M25PE64:
        s_flash_max_phy_pages = 32768;
        break;
    case E_FLASH_TYPE_W25Q40BV:
        s_flash_max_phy_pages = 2048;
        break;
    case E_FLASH_TYPE_W25Q32FV:
        s_flash_max_phy_pages = 8192*2;
        break;
    case E_FLASH_TYPE_W25Q64FV:
        s_flash_max_phy_pages = 32768;
        break;
    case E_FLASH_TYPE_W25Q128FV:
        s_flash_max_phy_pages = 65536;
        break;
    case E_FLASH_TYPE_GD25Q16B:
        s_flash_max_phy_pages = 8192;
        break;
    case E_FLASH_TYPE_GD25Q32B:
        s_flash_max_phy_pages = 8192*2;
        break;
    default:
        s_flash_max_phy_pages = 0;
        break;
    }
    
    return flash_id;
}

/*! \brief
*       获得芯片的最大页数
* \return
*		pages count
*/
u32 BSP_FLASH_EXT_GetMaxPhyPages(void)
{
    return s_flash_max_phy_pages;
}

/*! \brief
*       向Flash写入字节数据
* \param addr[IN]			- 地址码		
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		Writen Bytes Count	- 已写入的字节数
*			>0		- 写入成功
*			=0		- 写入失败
* \notes
*		1.该函数最大支持写1页数据，即256字节；
*		2.该函数不支持跨页写数据，如果从中间开始写的话，数据长度需要使用256-Addr%256计算能够写入的长度，否则会写入失败，反馈0.
*/
u32 BSP_FLASH_EXT_WriteBytes(u32 addr, u8 *dat, u32 len)
{
    u32 i = 0;
    u8  tmp[256] = {0};
    u32 size = 0;
    
    // 写数据不能超过256字节且不能跨页
    if ((len > 256) || (256-addr%256 < len) || (len==0))
    {
        return 0;
    }
    
    // 使能写操作
    BSP_FLASH_EXT_WriteEnable();
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_PAGE_PROGRAM);	
    // 写地址
    BSP_FLASH_EXT_SPI_WriteData((addr>>16)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr>>8)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr&0xff));
    // 写数据
    for (i=0; i<len; ++i)
    {
        BSP_FLASH_EXT_SPI_WriteData(dat[i]);
    }
    
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    // 延时等待
    BSP_FLASH_EXT_WaitWriteComplete();
    // 禁止写操作
    BSP_FLASH_EXT_WriteDisable();
    
    // 验证写入的数据
    size = BSP_FLASH_EXT_ReadBytes(addr, tmp, len);
    if (len != size)
        return 0;
    
    // 验证写入的数据是否正确
    if (memcmp(tmp, dat, len) != 0)
        return 0;
    
    return len;
}

/*! \brief
*       向Flash写入字节数据
* \param page_start[IN]		- 起始页		
* \param pages[IN]			- 写入的页数
* \param dat[IN]			- 数据
* \param len[IN]			- 长度
* \return
*		Writen Pages Count	- 已写入数据的页数
* \notes
*		1.一页数据的长度是256字节，因此要写入的数据长度必须是pages*256大小；
*/
uint32_t BSP_FLASH_EXT_Write_MultiPages_Data(u32 page_start, u32 pages, u8 *dat, u32 len)
{
    u32 i = 0;
    u32 rv = 0;
    u32 addr = 0;
    
    // 判断数据缓冲区大小是否足够
    if (len < pages*FLASH_EXT_PAGE_SIZE)
    {
        return 0;
    }
    
    addr = page_start * FLASH_EXT_PAGE_SIZE;
    
    for (i=0; i<pages; ++i)
    {
        rv = BSP_FLASH_EXT_WriteBytes(addr+i*FLASH_EXT_PAGE_SIZE, dat+i*FLASH_EXT_PAGE_SIZE, FLASH_EXT_PAGE_SIZE);
        if (rv != FLASH_EXT_PAGE_SIZE)
        {
            break;
        }
    }
    
    return pages;
}

/*! \brief
*       从Flash读取字节数据
* \param addr[IN]			- 起始地址	
* \param dat[OUT]			- 数据
* \param len[IN]			- 长度
* \return
*		已读取的数据长度	
*			>0		- 读取成功
*			=0		- 读取失败
* \notes
*		1.该函数最大支持读1页数据，即256字节；
*		2.该函数不支持跨页读数据，如果从中间开始写的话，数据长度需要使用256-Addr%256计算能够读取的长度，否则会读取失败，反馈0.
*/
u32 BSP_FLASH_EXT_ReadBytes(u32 addr, u8 *dat, u32 len)
{
    u32 i = 0;
    
    // 写数据不能超过256字节且不能跨页
    if ((len > 256) || (256-addr%256 < len) || (len==0))
    {
        return 0;
    }
    
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_READ_DATA);	
    // 写地址
    BSP_FLASH_EXT_SPI_WriteData((addr>>16)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr>>8)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr&0xff));
    // 读数据
    for (i=0; i<len; ++i)
    {
        dat[i] = BSP_FLASH_EXT_SPI_ReadData();
    }
    
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    
    return i;
}

/*! \brief
*       从Flash读取字节数据
* \param page_start[IN]		- 起始页	
* \param pages[OUT]			- 页数
* \param dat[IN]			- 数据
* \param len[IN]			- 长度
* \return
*		已读取的数据页数	
* \notes
*		1.一页数据的长度是256字节，因此要读取的数据长度必须是pages*256大小，数据缓冲区必须定义好大小
*/
u32 BSP_FLASH_EXT_Read_MultiPages_Data(u32 page_start, u32 pages, u8 *dat, u32 len)
{
    u32 i = 0;
    u32 rv = 0;
    u32 addr = 0;
    
    // 判断数据缓冲区大小是否足够
    if (len < pages*FLASH_EXT_PAGE_SIZE)
    {
        return 0;
    }
    
    addr = page_start * FLASH_EXT_PAGE_SIZE;
    
    for (i=0; i<pages; ++i)
    {
        rv = BSP_FLASH_EXT_ReadBytes(addr+i*FLASH_EXT_PAGE_SIZE, dat+i*FLASH_EXT_PAGE_SIZE, FLASH_EXT_PAGE_SIZE);
        if (rv != FLASH_EXT_PAGE_SIZE)
        {
            break;
        }
    }
    
    return pages;
}

/*! \brief
*       擦除4K数据
* \param page[IN]		- 页地址		
*/
void BSP_FLASH_EXT_Erase4K(u32 page)
{
    u32 addr = 0;
    
    // 计算FLASH擦除地址
    addr = page * FLASH_EXT_PAGE_SIZE;
    
    // 使能写操作
    BSP_FLASH_EXT_WriteEnable();
    
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_BLOCK_ERASE_4K);	
    // 写地址
    BSP_FLASH_EXT_SPI_WriteData((addr>>16)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr>>8)&0xff);
    BSP_FLASH_EXT_SPI_WriteData((addr&0xff));
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 等待执行结束
    BSP_FLASH_EXT_WaitWriteComplete();
    // 禁止写操作
    BSP_FLASH_EXT_WriteDisable();
    
    return;
}

/*! \brief
*       擦除整个FLASH芯片	
*/
void BSP_FLASH_EXT_EraseChip(void)
{
    // 使能写操作
    BSP_FLASH_EXT_WriteEnable();
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_CHIP_ERASE);	
    
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    // 等待处理结束
    BSP_FLASH_EXT_WaitWriteComplete();
    // 禁止写操作
    BSP_FLASH_EXT_WriteDisable();
}

/*! \brief
*      SPI管脚初始化
*/
static void BSP_FLASH_EXT_SPI_PortInit(void)
{
    assert_param(s_flash_cfg_handler != NULL);
    
    BSP_GPIO_Init(s_flash_cfg_handler->nss.port,	s_flash_cfg_handler->nss.pin,	GPIO_Mode_Out_PP_Low_Fast);
    BSP_GPIO_Init(s_flash_cfg_handler->sck.port,	s_flash_cfg_handler->sck.pin,	GPIO_Mode_Out_PP_Low_Fast);
    BSP_GPIO_Init(s_flash_cfg_handler->mosi.port,	s_flash_cfg_handler->mosi.pin,	GPIO_Mode_Out_PP_Low_Fast);
    BSP_GPIO_Init(s_flash_cfg_handler->miso.port,	s_flash_cfg_handler->miso.pin,	GPIO_Mode_In_PU_No_IT);
}

/*! \brief
*      SPI NSS引脚设置
* \param[IN]val			- 设置值
*		0		- 置低
*		1		- 置高
*/
static void BSP_FLASH_EXT_SPI_NssSet(u8 val)
{
    if (val)
    {
        BSP_GPIO_SetHigh(s_flash_cfg_handler->nss.port,	s_flash_cfg_handler->nss.pin);
    }
    else
    {
        BSP_GPIO_SetLow(s_flash_cfg_handler->nss.port,	s_flash_cfg_handler->nss.pin);
    }
}

/*! \brief
*      SPI Sck引脚设置
* \param[IN]val			- 设置值
*		0		- 置低
*		1		- 置高
*/
static void BSP_FLASH_EXT_SPI_SckSet(u8 val)
{
    if (val)
    {
        BSP_GPIO_SetHigh(s_flash_cfg_handler->sck.port, s_flash_cfg_handler->sck.pin);
    }
    else
    {
        BSP_GPIO_SetLow(s_flash_cfg_handler->sck.port, s_flash_cfg_handler->sck.pin);
    }
}

/*! \brief
*      SPI MOSI引脚设置
* \param[IN]val			- 设置值
*		0		- 置低
*		1		- 置高
*/
static void BSP_FLASH_EXT_SPI_MosiSet(u8 val)
{
    if (val)
    {
        BSP_GPIO_SetHigh(s_flash_cfg_handler->mosi.port, s_flash_cfg_handler->mosi.pin);
    }
    else
    {
        BSP_GPIO_SetLow(s_flash_cfg_handler->mosi.port,	s_flash_cfg_handler->mosi.pin);
    }
}

/*! \brief
*      SPI miso引脚读数据
* \return
*		value		- SPI从设备返回的数据
*/
static u8 BSP_FLASH_EXT_SPI_MisoRead(void)
{
    return BSP_GPIO_ReadState(s_flash_cfg_handler->miso.port, s_flash_cfg_handler->miso.pin);
}

/*! \brief
*       通过模拟SPI口写数据
* \param uData[IN]      - data
*/
static void BSP_FLASH_EXT_SPI_WriteData(unsigned char uData)
{
    u8	u8Index = 0;
    
    // 写寄存器地址
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        if((uData << u8Index) & 0x80)
        {
            BSP_FLASH_EXT_SPI_MosiSet(1);
        }
        else
        {
            BSP_FLASH_EXT_SPI_MosiSet(0);
        }
        
        BSP_FLASH_EXT_SPI_SckSet(0);
        BSP_FLASH_EXT_SPI_SckSet(1);	
    }
}

/*! \brief
*       通过模拟SPI口读数据
* \return
*       register value
*/
static unsigned char BSP_FLASH_EXT_SPI_ReadData(void)
{
    u8	u8Index = 0;
    u8 	u8Value = 0;
    
    u8Value = 0;
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        BSP_FLASH_EXT_SPI_SckSet(1);
        BSP_FLASH_EXT_SPI_SckSet(0);
        
        u8Value = u8Value << 1;
        if(1 == BSP_FLASH_EXT_SPI_MisoRead())
        {
            u8Value |= 0x01;		
        }
    }
    
    return u8Value;
}

/*! \brief
*       flash操作延时
*/
static void BSP_FLASH_EXT_Delay(void) 
{
    u8 i = 50;
    while(i--);
    //delay_us(50);
    /* 改成看标志 */
}

/*! \brief
*       flash芯片写操作使能
*/
static void BSP_FLASH_EXT_WriteEnable(void)
{
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 设置FLASH写使能
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_WRITE_ENABLE);		// write instruction
    // 延时等待
    BSP_FLASH_EXT_Delay();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
}

/*! \brief
*       flash芯片写操作禁止
*/
static void BSP_FLASH_EXT_WriteDisable(void)
{
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 设置FLASH写使能
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_WRITE_DISABLE);		// write instruction
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    // 延时等待
    BSP_FLASH_EXT_Delay();
}

/*! \brief
*       flash读状态寄存器
*/
static u8  BSP_FLASH_EXT_ReadRegStatus(void)
{
    unsigned char state;
    
    // 片选拉低
    BSP_FLASH_EXT_SPI_NssSet(0);
    // 写命令
    BSP_FLASH_EXT_SPI_WriteData(FLASH_INS_READ_STATUS_REG);		
    // 读状态
    state = BSP_FLASH_EXT_SPI_ReadData();
    // 片选拉高
    BSP_FLASH_EXT_SPI_NssSet(1);
    
    return state;
}

/*! \brief
*       flash等待写操作完成
*/
static void BSP_FLASH_EXT_WaitWriteComplete(void)
{
    do {
    }while (BSP_FLASH_EXT_ReadRegStatus() & 0x01);
}

#endif





