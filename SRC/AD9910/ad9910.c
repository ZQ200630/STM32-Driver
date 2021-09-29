/**********************************************************
                       康威电子

功能：stm32f103rct6控制
			显示：12864
接口：按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "system.h"
#include "ad9910.h"
/*-----------------------------------------------
 名称：AD9910串行驱动
 编写：Liu
 日期：2014.6
 修改：无
 内容：
------------------------------------------------*/
//高位先行
uchar cfr1[]={0x00,0x40,0x00,0x00};       //cfr1控制字
/* 0x00=0000 0000b,   RAM没有使能
   0x40=0100 0000b,
*/
uchar cfr2[]={0x01,0x00,0x00,0x00};       //cfr2控制字
/* 0x01=0000 0001b,   使能单频调制profile振幅范围使能
*/
const uchar cfr3[]={0x05,0x0F,0x41,0x32};       //cfr3控制字  40M输入  25倍频     ICP=001;
/* 0x05=0000 0101b,   VC0=101
   0x0f=0000 1111b,   ICP=001;(小电流)
   0x41=0100 0001b,   REFCLK PLL使能
   0x32=0011 0010b    最后一位无效位，11001b是25倍频
*/
uchar profile11[8]={0x3f,0xff,0x00,0x00,0x25,0x09,0x7b,0x42};       //profile1控制字 0x25,0x09,0x7b,0x42
//01振幅控制 23相位控制 4567频率调谐字

uchar drgparameter[20]={0x00}; //DRG参数
uchar ramprofile0[8] = {0x00}; //ramprofile0控制字
extern const unsigned char ramdata_Square[4096];

void AD9110_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(MY_AD9910_RST_PORT, MY_AD9910_RST_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_PWR_PORT, MY_AD9910_PWR_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_TXENABLE_PORT, MY_AD9910_TXENABLE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_PF2_PORT, MY_AD9910_PF2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_PF1_PORT, MY_AD9910_PF1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_PF0_PORT, MY_AD9910_PF0_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_IO_UPDATE_PORT, MY_AD9910_IO_UPDATE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_OSK_PORT, MY_AD9910_OSK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_DRCTL_PORT, MY_AD9910_DRCTL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_DRHOLD_PORT, MY_AD9910_DRHOLD_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_SDIO_PORT, MY_AD9910_SDIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_SCLK_PORT, MY_AD9910_SCLK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MY_AD9910_CSB_PORT, MY_AD9910_CSB_PIN, GPIO_PIN_RESET);
    /*Configure GPIO pin : PA7 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = MY_AD9910_RST_PIN;
    HAL_GPIO_Init(MY_AD9910_RST_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_PWR_PIN;
    HAL_GPIO_Init(MY_AD9910_PWR_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_TXENABLE_PIN;
    HAL_GPIO_Init(MY_AD9910_TXENABLE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_PF2_PIN;
    HAL_GPIO_Init(MY_AD9910_PF2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_PF1_PIN;
    HAL_GPIO_Init(MY_AD9910_PF1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_PF0_PIN;
    HAL_GPIO_Init(MY_AD9910_PF0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_IO_UPDATE_PIN;
    HAL_GPIO_Init(MY_AD9910_IO_UPDATE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_OSK_PIN;
    HAL_GPIO_Init(MY_AD9910_OSK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_DRCTL_PIN;
    HAL_GPIO_Init(MY_AD9910_DRCTL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_DRHOLD_PIN;
    HAL_GPIO_Init(MY_AD9910_DRHOLD_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_SDIO_PIN;
    HAL_GPIO_Init(MY_AD9910_SDIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_SCLK_PIN;
    HAL_GPIO_Init(MY_AD9910_SCLK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_CSB_PIN;
    HAL_GPIO_Init(MY_AD9910_CSB_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    GPIO_InitStruct.Pin = MY_AD9910_PLL_LOCK_PIN;
    HAL_GPIO_Init(MY_AD9910_PLL_LOCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_RSO_PIN;
    HAL_GPIO_Init(MY_AD9910_RSO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_PDCLK_PIN;
    HAL_GPIO_Init(MY_AD9910_PDCLK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_SYNC_CLK_PIN;
    HAL_GPIO_Init(MY_AD9910_SYNC_CLK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_DROVER_PIN;
    HAL_GPIO_Init(MY_AD9910_DROVER_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_AD9910_SDO_PIN;
    HAL_GPIO_Init(MY_AD9910_SDO_PORT, &GPIO_InitStruct);
}

//=====================================================================


//======================发送8位数据程序===================================
void txd_8bit(uchar txdat)
{
    uchar i,sbt;
    sbt=0x80;
    MyOperateSCLK(MY_PIN_RESET);
    for (i=0;i<8;i++) {
        if ((txdat & sbt)==0) {
            MyOperateSDIO(MY_PIN_RESET);
        } else {
            MyOperateSDIO(MY_PIN_SET);
        }
        MyOperateSCLK(MY_PIN_SET);
        sbt=sbt>>1;
        MyOperateSCLK(MY_PIN_RESET);
    }
}

//======================ad9910初始化程序================================
void Init_ad9910(void)
{
    uchar k,m;

    //IO初始化
    AD9110_IOInit();

    //拉低PWR
    MyOperatePWR(MY_PIN_RESET);

    //设置PF为000
    MyOperatePF0(MY_PIN_RESET);
    MyOperatePF1(MY_PIN_RESET);
    MyOperatePF2(MY_PIN_RESET);

    MyOperateDRCTL(MY_PIN_RESET);
    MyOperateDRHOLD(MY_PIN_RESET);

    MyOperateRESET(MY_PIN_SET);
    MyDelayMs(5);
    MyOperateRESET(MY_PIN_RESET);

    MyOperateCSB(MY_PIN_RESET);

    txd_8bit(0x00);    //发送CFR1控制字地址

    for (m=0;m<4;m++) {
        txd_8bit(cfr1[m]);
    }
    MyOperateCSB(MY_PIN_SET);

    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);

    txd_8bit(0x01);    //发送CFR2控制字地址
    for (m=0;m<4;m++) {
        txd_8bit(cfr2[m]);
    }

    MyOperateCSB(MY_PIN_SET);

    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);

    txd_8bit(0x02);    //发送CFR3控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr3[m]);

    MyOperateCSB(MY_PIN_SET);

    for (k=0;k<10;k++);


    MyOperateIOUPDATE(MY_PIN_SET);
    
    for(k=0;k<10;k++);

    MyOperateIOUPDATE(MY_PIN_RESET);

    MyDelayMs(1);

}
//=====================================================================

//======================ad9910发送profile值程序===================================
void Txprofile(void)
{
    uchar m,k;

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x0e);    //发送profile1控制字地址
    for (m=0;m<8;m++)
        txd_8bit(profile11[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);


    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}

//===================计算频偏字、频率字和发送程序======================
void Freq_convert(ulong Freq)
{
    ulong Temp;
    Temp=(ulong)Freq*4.294967296;	   //将输入频率因子分为四个字节  4.294967296=(2^32)/1000000000
    profile11[7]=(uchar)Temp;
    profile11[6]=(uchar)(Temp>>8);
    profile11[5]=(uchar)(Temp>>16);
    profile11[4]=(uchar)(Temp>>24);
    Txprofile();
}

//===================计算幅度字和发送程序==============================
void Write_Amplitude(uint Amp)
{
    ulong Temp;
    //Temp= (ulong)Amp*1.000000000;
    //Temp = (ulong)Amp*25.20615385;	   //将输入幅度因子分为两个字节  25.20615385=(2^14)/650
    Temp = (ulong)Amp*21.4731323722;     //将输入幅度因子分为两个字节  25.20615385=(2^14)/763
    if(Temp > 0x3fff)
        Temp = 0x3fff;
    Temp &= 0x3fff;
    profile11[1]=(uchar)Temp;
    profile11[0]=(uchar)(Temp>>8);
    Txprofile();
}
//=====================================================================
void Freq_xiangwei(uint xiangwei)
{
    ulong Temp;
//	  Temp=(ulong)xiangwei*1.000000000;
    Temp=(ulong)xiangwei*182.04444444444; //将输入频率因子分为两个个字节  182.04444444444=(2^16)/360
    xiangwei =  0xffff;
    Temp &= 0xffff;
    profile11[3]=(uchar)Temp;
    profile11[2]=(uchar)(Temp>>8);
    Txprofile();
}

//=====================================================================

//======================发送cfr的程序===============================================

void Txcfr(void)
{
    uchar m,k;

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x00);    //发送CFR1控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr1[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x01);    //发送CFR2控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr2[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x02);    //发送CFR3控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr3[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    
    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}



void Txcfrram(void)
{
    uchar m,k;

    cfr1[0] = 0xc0; //RAM 使能，幅度控制
    cfr2[1] = 0x00; //DRG 失能
    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x00);    //发送CFR1控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr1[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x01);    //发送CFR2控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr2[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x02);    //发送CFR3控制字地址
    for (m=0;m<4;m++)
        txd_8bit(cfr3[m]);
    MyOperateCSB(MY_PIN_SET);
    for (k=0;k<10;k++);

    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}

//=================ad9910发送ramprofile0控制字程序=====================
void Txramprofile(void)
{
    uchar m,k;

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x0e);    //发送ramprofile0控制字地址
    for (m=0;m<8;m++)
        txd_8bit(ramprofile0[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);

    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}
//=====================================================================

//=======================ad9910发送ramdata程序=========================
void Txramdata(void)
{
    uint m,k;

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x16);    //发送ram控制字地址
    for (m=0; m<256; m++)
        txd_8bit(ramdata_Square[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);

    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}
//=====================================================================


//=======================方波参数设置和发送程序========================
void Square_wave(uint Sample_interval)//方波
{
    ulong Temp;
    Temp = Sample_interval/4; //1GHz/4, 采样间隔范围：4*(1~65536)ns
    if(Temp > 0xffff)
        Temp = 0xffff;
//	ramprofile0[7] = 0x24;               //设定为循环模式
//	ramprofile0[6] = 0x00;
    ramprofile0[7] = 0x53;
    ramprofile0[6] = 0x38;	              //双向扫描模式
    ramprofile0[5] = 0x00;
    ramprofile0[4] = 0xc0;
    ramprofile0[3] = 0x0f;
    ramprofile0[2] = (uchar)Temp;        //设定速率，步进率为m
    ramprofile0[1] = (uchar)(Temp>>8);
    ramprofile0[0] = 0x00;
    Txramprofile();
    Txramdata();


    //Txprofile();
}
//=====================================================================

//======================ad9910发送DRG参数程序==========================
void Txdrg(void)
{
    uchar m,k;

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x0b);    //发送数字斜坡限制地址0x0b
    for (m=0;m<8;m++)
        txd_8bit(drgparameter[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x0c);    //发送数字斜坡步长地址0x0c
    for (m=8;m<16;m++)
        txd_8bit(drgparameter[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);

    MyOperateCSB(MY_PIN_RESET);
    txd_8bit(0x0d);    //发送数字斜坡速率地址0x0d
    for (m=16;m<20;m++)
        txd_8bit(drgparameter[m]);
    MyOperateCSB(MY_PIN_SET);
    for(k=0;k<10;k++);

    MyOperateIOUPDATE(MY_PIN_SET);
    for(k=0;k<10;k++);
    MyOperateIOUPDATE(MY_PIN_RESET);
    MyDelayMs(1);
}

void test() {
    cfr1[0] = 0x00; //RAM 失能
    cfr2[1]=0x0e; //DRG 使能
    Txcfr(); //发送cfrx控制字
    Write_Amplitude(500);

    ulong Temp1, Temp2, ITemp3, DTemp3, ITemp4, DTemp4;
    int SweepMinFre = 1000000;
    int SweepMaxFre = 40000000;
    int SweepStepFre = 1000;
    int SweepTime = 65535;
    Temp1 = (ulong)SweepMinFre*4.294967296;
    Temp2 = (ulong)SweepMaxFre*4.294967296;
    ITemp3 = (ulong)SweepStepFre*4.294967296;
    DTemp3 = ITemp3;
    ITemp4 = (ulong)SweepTime; //1GHz/4, 单位：ns
    DTemp4 = ITemp4;

    //扫频上下限
    drgparameter[7]=(uchar)Temp1;
    drgparameter[6]=(uchar)(Temp1>>8);
    drgparameter[5]=(uchar)(Temp1>>16);
    drgparameter[4]=(uchar)(Temp1>>24);
    drgparameter[3]=(uchar)Temp2;
    drgparameter[2]=(uchar)(Temp2>>8);
    drgparameter[1]=(uchar)(Temp2>>16);
    drgparameter[0]=(uchar)(Temp2>>24);
    //频率步进（单位：Hz）
    drgparameter[15]=(uchar)ITemp3;
    drgparameter[14]=(uchar)(ITemp3>>8);
    drgparameter[13]=(uchar)(ITemp3>>16);
    drgparameter[12]=(uchar)(ITemp3>>24);
    drgparameter[11]=(uchar)DTemp3;
    drgparameter[10]=(uchar)(DTemp3>>8);
    drgparameter[9]=(uchar)(DTemp3>>16);
    drgparameter[8]=(uchar)(DTemp3>>24);
    //步进时间间隔（单位：us）
    drgparameter[19]=(uchar)ITemp4;
    drgparameter[18]=(uchar)(ITemp4>>8);
    drgparameter[17]=(uchar)DTemp4;
    drgparameter[16]=(uchar)(DTemp4>>8);
    //发送DRG参数
    Txdrg();
}

void MyOperateRESET(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_RST_PORT, MY_AD9910_RST_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_RST_PORT, MY_AD9910_RST_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperatePWR(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_PWR_PORT, MY_AD9910_PWR_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_PWR_PORT, MY_AD9910_PWR_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

MY_PinState MyGetStatePLLLOCK() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_PLL_LOCK_PORT, MY_AD9910_PLL_LOCK_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}

MY_PinState MyGetStateRSO() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_RSO_PORT, MY_AD9910_RSO_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}

MY_PinState MyGetStatePDCLK() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_PDCLK_PORT, MY_AD9910_PDCLK_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}

void MyOperateTXENABLE(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_TXENABLE_PORT, MY_AD9910_TXENABLE_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_TXENABLE_PORT, MY_AD9910_TXENABLE_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperatePF2(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_PF2_PORT, MY_AD9910_PF2_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_PF2_PORT, MY_AD9910_PF2_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperatePF1(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_PF1_PORT, MY_AD9910_PF1_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_PF1_PORT, MY_AD9910_PF1_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperatePF0(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_PF0_PORT, MY_AD9910_PF0_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_PF0_PORT, MY_AD9910_PF0_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

MY_PinState MyGetStateSYNCLK() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_SYNC_CLK_PORT, MY_AD9910_SYNC_CLK_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}

void MyOperateIOUPDATE(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_IO_UPDATE_PORT, MY_AD9910_IO_UPDATE_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_IO_UPDATE_PORT, MY_AD9910_IO_UPDATE_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateOSK(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_OSK_PORT, MY_AD9910_OSK_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_OSK_PORT, MY_AD9910_OSK_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

MY_PinState MyGetStateDROVER() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_DROVER_PORT, MY_AD9910_DROVER_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}


void MyOperateDRCTL(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_DRCTL_PORT, MY_AD9910_DRCTL_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_DRCTL_PORT, MY_AD9910_DRCTL_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateDRHOLD(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_DRHOLD_PORT, MY_AD9910_DRHOLD_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_DRHOLD_PORT, MY_AD9910_DRHOLD_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateSDIO(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_SDIO_PORT, MY_AD9910_SDIO_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_SDIO_PORT, MY_AD9910_SDIO_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

MY_PinState MyGetStateSDO() {
    GPIO_PinState state = HAL_GPIO_ReadPin(MY_AD9910_SDO_PORT, MY_AD9910_SDO_PIN);
    switch (state) {
        case GPIO_PIN_SET:
            return MY_PIN_SET;
        case GPIO_PIN_RESET:
            return MY_PIN_RESET;
        default:
            break;
    }
}

void MyOperateSCLK(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_SCLK_PORT, MY_AD9910_SCLK_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_SCLK_PORT, MY_AD9910_SCLK_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateCSB(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9910_CSB_PORT, MY_AD9910_CSB_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9910_CSB_PORT, MY_AD9910_CSB_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
//=====================================================================


