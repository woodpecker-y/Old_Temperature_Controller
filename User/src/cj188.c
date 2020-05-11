#include "adf.h"
#include "sysparams.h"
#include "cj188.h"
#include "protocol.h"

/*! \brief 
*      CJ188协议组包
* \param pkg[IN]           - CJ188协议帧定义结构体
* \param out_data[IN]      - 组包后的数据包
* \param out_size[IN]      - 组包后的数据包长度
* 
* \return  
*      0                   - 成功
* \note  
* 
*/
u8 cj188_pack(CJ188Pkg *pkg, u8 *out_data, u8 *out_size)
{
    u8 offset = 0;
    u16 desn = 0x0000;
    
    // assert_param(*out_size > pkg->msg_size+10);
    /* 起始码 */
    out_data[offset] = 0x3A;
    offset += 1;
    /* 版本号 */
    out_data[offset] = 0x70;
    offset += 1;
    /* SN */
    memcpy(out_data+offset, &pkg->sn, sizeof(pkg->sn));
    offset += sizeof(pkg->sn);
    /* DESN */
    memcpy(out_data+offset, &desn, 2);
    offset += 2;
    /* GN */
    memcpy(out_data+offset, &pkg->gn, sizeof(pkg->gn));
    offset += sizeof(pkg->gn);
    /* 控制码 */
    out_data[offset] = pkg->ctrl_code;
    offset += 1;
    /* 数据域长度 */
    out_data[offset] = pkg->msg_size;
    offset += 1;
    /* 数据负载 */
    memcpy(out_data+offset, pkg->msg_data, pkg->msg_size);
    offset += pkg->msg_size;
    /* 校验和 */
    out_data[offset] = check_sum(out_data, offset);
    offset += 1;
    /* 结束符 */
    out_data[offset] = 0xAA;
    offset += 1;
    
    *out_size = offset;
    
    return 0;
    
    
//////    /* 仪表地址 */
//////    memcpy(out_data+offset, pkg->addr, sizeof(pkg->addr));
//////    bytes_reverse(out_data+offset, sizeof(pkg->addr));
//////    offset += sizeof(pkg->addr);
//////    /* 控制码 */
//////    out_data[offset] = pkg->ctrl_code;
//////    offset += 1;
//////    /* 数据域长度 */
//////    out_data[offset] = pkg->msg_size;
//////    offset += 1;
//////    /* 数据负载 */
//////    memcpy(out_data+offset, pkg->msg_data, pkg->msg_size);
//////    offset += pkg->msg_size;
//////    /* 校验和 */
//////    out_data[offset] = check_sum(out_data, offset);
//////    offset += 1;
//////    
//////    *out_size = offset;
//////    
//////    return 0;
}

/*! \brief 
*      CJ188协议解包
* \param pkg[IN]           - 解包后存储数据结构
* \param in_data[IN]       - 原始报文
* \param in_size[IN]       - 原始报文长度
* 
* \return  
*      0                           - 成功
*      CJ188_ERR_INVALID_STX_ETX   - 帧头、帧尾错误
*      CJ188_ERR_CHECKSUM          - 校验和错误
*      CJ188_ERR_LENGTH            - 长度错误
* \note  
* 
*/
u8 cj188_unpack(CJ188Pkg *pkg, u8 *in_data, u8 in_size)
{
    u8 offset = 0;
    u8 sum = 0;
    u32 sn_data = 0;
    u16 gn_data = 0;
    
    /* 获取除了数据区域以外的数据，都放到pkg里面，不是存储起来 */
    /* 校验和检查 */
    sum = check_sum(in_data, in_size-2);
    //printf("sum=%x\r\n",sum);
    //printf("in_data[in_size-1]=%x\r\n",in_data[in_size-1]);
    if (sum != in_data[in_size-2])
    {
        return CJ188_ERR_CHECKSUM;
    }
    
    /* 长度判断 */
    //printf("in_data[6]=%d\r\n",in_data[6]);
    //printf("in_size-8=%d\r\n",in_size-8);
    if (in_data[11] != in_size-14)
    {
        //printf("size1:%d, size2:%d\r\n", in_data[6], in_size-8);
        return CJ188_ERR_LENGTH;
    }
    /* SN前面的开始码和版本号 */
    offset = 2;
    /* SN */
    memcpy(&sn_data, in_data+offset, sizeof(pkg->sn));
    sn_data = LittleANDBig_ModeCON4(&sn_data);
    memcpy(&(pkg->sn), &(sn_data), sizeof(pkg->sn));
    offset += 4;
    /* 忽略DESN，因为一般都是0x0000 */
    offset += 2;
    /* GN */
    memcpy(&gn_data, in_data+offset, sizeof(pkg->gn));
    gn_data = LittleANDBig_ModeCON2(&gn_data);
    memcpy(&(pkg->gn), &(gn_data), sizeof(pkg->gn));
    offset += 2;
    /* 控制码 */
    pkg->ctrl_code = in_data[offset];
    offset += 1;
    /* 数据负载长度 */
    pkg->msg_size = in_data[offset];
    offset += 1;
    /* 数据负载 */
    memcpy(pkg->msg_data, in_data+offset, pkg->msg_size);
    offset += pkg->msg_size;
//    printf("pkg->sn = %lu\r\n", pkg->sn);
//    printf("pkg->gn = %u\r\n", pkg->gn);
//    printf("pkg->ctrl_code = %02x\r\n", pkg->ctrl_code);
//    printf("pkg->msg_size = %02x\r\n", pkg->msg_size);
    
    return 0;
    
    
    
    
//    /* 校验和检查 */
//    sum = check_sum(in_data, in_size-1);
//    //printf("sum=%x\r\n",sum);
//    //printf("in_data[in_size-1]=%x\r\n",in_data[in_size-1]);
//    if (sum != in_data[in_size-1])
//    {
//        return CJ188_ERR_CHECKSUM;
//    }
//    
//    /* 长度判断 */
//    //printf("in_data[6]=%d\r\n",in_data[6]);
//    //printf("in_size-8=%d\r\n",in_size-8);
//    if (in_data[6] != in_size-8)
//    {
//        //printf("size1:%d, size2:%d\r\n", in_data[6], in_size-8);
//        return CJ188_ERR_LENGTH;
//    }
//    
//    offset = 0;
//    
//    /* 地址码 */
//    memcpy(pkg->addr, in_data+offset, sizeof(pkg->addr));
//    bytes_reverse(pkg->addr, sizeof(pkg->addr));
//    offset += sizeof(pkg->addr);
//    /* 控制码 */
//    pkg->ctrl_code = in_data[offset];
//    offset += 1;
//    /* 数据负载长度 */
//    pkg->msg_size = in_data[offset];
//    offset += 1;
//    /* 数据负载 */
//    memcpy(pkg->msg_data, in_data+offset, pkg->msg_size);
//    offset += pkg->msg_size;
//    
//    return 0;
}

