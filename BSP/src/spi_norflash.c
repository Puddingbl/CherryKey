
#include "spi_norflash.h"
#include "spi.h"
#include "stdio.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
static void SPI_Flash_ReadFlashID(void);                          //读取FlashID
static void SPI_Flash_EraseSector(uint32_t);                      //擦除扇区(4kB)
static void SPI_Flash_EraseTotal(void);                           //擦除全部
static void SPI_Flash_WriteUnfixed(uint8_t*,uint32_t,uint32_t);   //写入不固定长度数据
static void SPI_Flash_ReadUnfixed(uint8_t*,uint32_t,uint32_t);    //读取不固定长度数据

/* Public variables-----------------------------------------------------------*/
SPI_Flash_t  SPI_Flash = 
{
	0,
		
	SPI_Flash_ReadFlashID,
	SPI_Flash_EraseSector,
	SPI_Flash_EraseTotal,
	SPI_Flash_WriteUnfixed,
	SPI_Flash_ReadUnfixed
};

/* Private function prototypes------------------------------------------------*/
static uint8_t SPI_Flash_ReadByte(void);      //从Flash读1个字节
static void    SPI_Flash_WriteByte(uint8_t);  //给Flash写1个字节

static void SPI_Flash_WriteEnable(void);      //Flash写使能
static void SPI_Flash_WaitForWriteEnd(void);  //等待Flash写入完成

static void SPI_Flash_WritePage(uint8_t*,uint32_t,uint16_t);      //写入页(256Bytes),写入长度不超过256字节

/*
	* @name   SPI_Flash_ReadByte
	* @brief  从Flash读1个字节
	* @param  None
	* @retval 读取到的数据     
*/
static uint8_t SPI_Flash_ReadByte()
{	
	uint8_t ReceiveByte;
	//等待模式读出1个字节
	if(HAL_SPI_Receive(&hspi2,&ReceiveByte,1,0x0A) != HAL_OK)
		ReceiveByte = Dummy_Byte;
	//返回字节
	return ReceiveByte;
}

/*
	* @name   SPI_Flash_ReadByte
	* @brief  给Flash写1个字节
	* @param  Byte -> 待写入的字节
	* @retval 读取到的数据     
*/
static void SPI_Flash_WriteByte(uint8_t Byte)
{	
	uint8_t SendByte = Byte;
	//等待模式写入1个字节
	HAL_SPI_Transmit(&hspi2,&SendByte,1,0x0A);
}

/*
	* @name   SPI_Flash_WriteEnable
	* @brief  Flash写使能
	* @param  None
	* @retval None
*/
static void SPI_Flash_WriteEnable()
{
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	//发送命令：写使能
	SPI_Flash_WriteByte(W25X_WriteEnable);
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_WaitForWriteEnd
	* @brief  等待Flash写入完成
	* @param  None
	* @retval None
*/
void SPI_Flash_WaitForWriteEnd()
{
	uint8_t Flash_Status1 = 0;
	
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	//发送命令：读状态寄存器1
	SPI_Flash_WriteByte(W25X_ReadStatusReg1);
	
	//连续读取状态寄存器1，直到WEL为低电?
//	Timer6.usDelay_Timer = 0;
	do
	{
		Flash_Status1 = SPI_Flash_ReadByte();
		//防止意外，概率极低
//		if(Timer6.usDelay_Timer >= TIMER0_10S)
//			break;
	}
	while((Flash_Status1 & Flash_Status1_BUSY) == Flash_Status1_BUSY);
/*个人总结：在数据手册13页有框图，状态寄存器第0位是写入busy标志位*/	
	//禁用Flash芯片: CS输出高电平

	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_WaitForWriteEnd
	* @brief  等待Flash写入完成
	* @param  None
	* @retval None
*/
void SPI_Flash_writeReg()
{
	uint8_t Flash_Status1 = 0;
	
	SPI_Flash_WriteEnable();	
	
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	//发送命令：读状态寄存器1
	SPI_Flash_WriteByte(0x15);
	
	Flash_Status1 = SPI_Flash_ReadByte();
	printf("Flash_Status1=%d\n",Flash_Status1);
/*个人总结：在数据手册13页有框图，状态寄存器第0位是写入busy标志位*/	
	//禁用Flash芯片: CS输出高电平

	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_ReadFlashID
	* @brief  读取FlashID
	* @param  None
	* @retval None
*/
static void SPI_Flash_ReadFlashID()
{
	uint8_t buf[3];
	
	//检测flash是否处于忙碌状态
	SPI_Flash_WaitForWriteEnd();
	
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	//发送命令：读取JEDEC ID(设备标识符 -> 制造商+内存类型+容量)
	SPI_Flash_WriteByte(W25X_ReadJedecID);
	buf[0] = SPI_Flash_ReadByte();
	buf[1] = SPI_Flash_ReadByte();
	buf[2] = SPI_Flash_ReadByte();
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
	
	SPI_Flash.JedecID = (buf[0] << 16) + (buf[1] << 8) + buf[2];
	printf("The JEDEC ID of SPI flash is 0x%.6X\r\n",SPI_Flash.JedecID);
	
	/***
	JEDEC ID      IC型号             存储空间大小         
  0xEF4015	    W25Q16JV-IQ/JQ     16M-bit /2M-byte
  0XEF4017      W25Q64JV-IQ/JQ     64M-bit /8M-byte
  0XEF4018      W25Q128JV-IQ/JQ    128M-bit/16M-byte
	***/
	switch(SPI_Flash.JedecID)
	{
		case 0xEF4015: printf("Flash:W25Q16JV-IQ/JQ, 16M-bit /2M-byte\r\n"); break;
		case 0xEF4017: printf("Flash:W25Q64JV-IQ/JQ, 64M-bit /8M-byte\r\n"); break;	 
    case 0xEF4018: printf("Flash:W25Q128JV-IQ/JQ,128M-bit/16M-byte\r\n"); break;
    default: printf("Flash:aren't know\r\n");	
	}
}

/*
	* @name   SPI_Flash_EraseSector
	* @brief  擦除扇区(4kB)
	* @param  SectorAddr：待擦除扇区地址，要求为4k倍数
	* @retval None
*/
static void SPI_Flash_EraseSector(uint32_t SectorAddr)
{
	//检测flash是否处于忙碌状态
	SPI_Flash_WaitForWriteEnd();
	
	//Flash写使能，允许擦除
	SPI_Flash_WriteEnable();
	
	/* 擦除扇区 */
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	
	//发送命令：扇区擦除
	SPI_Flash_WriteByte(W25X_SectorErase);
	//发送擦除扇区地址的高字节
	SPI_Flash_WriteByte((SectorAddr & 0xFF0000) >> 16);
	//发送擦除扇区地址的中字节
	SPI_Flash_WriteByte((SectorAddr & 0xFF00) >> 8);
	//发送擦除扇区地址的低字节
	SPI_Flash_WriteByte(SectorAddr & 0xFF);	
	
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
	
	//等待擦除完毕
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_EraseTotal
	* @brief  擦除全部
	* @param  None
	* @retval None
*/
static void SPI_Flash_EraseTotal()
{
	//检测flash是否处于忙碌状态
	SPI_Flash_WaitForWriteEnd();
	
	//Flash写使能，允许擦除
	SPI_Flash_WriteEnable();
	
	/* 擦除整片 */
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	
	//发送命令：整片擦除
	SPI_Flash_WriteByte(W25X_ChipErase);	
	
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
	
	//等待擦除完毕
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_WritePage
	* @brief  写入页(256Bytes),写入长度不超过256字节
	* @param  pWriteBuffer：待写入数据的指针
  *         WriteAddr   ：写入地址
  *         WriteLength ：写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize(256Bytes)
	* @retval None
*/
static void SPI_Flash_WritePage(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint16_t WriteLength)
{
	//检测flash是否处于忙碌状态
	SPI_Flash_WaitForWriteEnd();
	
	//Flash写使能，允许写入
	SPI_Flash_WriteEnable();
	
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	
	//发送命令：页面编程
	SPI_Flash_WriteByte(W25X_PageProgram);	
	//发送地址高字节
	SPI_Flash_WriteByte((WriteAddr & 0xFF0000) >> 16);
	//发送地址中字节
	SPI_Flash_WriteByte((WriteAddr & 0xFF00) >> 8);
	//发送地址低字节
	SPI_Flash_WriteByte(WriteAddr & 0xFF);
	
	if(WriteLength > SPI_FLASH_PageSize)
  {
     WriteLength = SPI_FLASH_PageSize;
		printf("Error: Flash:can't more than 256!\n");
  }
	
	//开始写入数据
	while (WriteLength--)
  {
     /* 读取一个字节*/
    SPI_Flash_WriteByte(*pWriteBuffer);
    /* 指向下一个字节缓冲区 */
    pWriteBuffer++;
  }
	
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
	
	//等待写入完毕
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_WriteUnfixed
	* @brief  写入不固定长度数据
	* @param  pWriteBuffer：待写入数据的缓存指针
  *         WriteAddr   ：写入地址
  *         WriteLength ：写入数据长度
	* @retval None
*/
static void SPI_Flash_WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
	uint32_t PageNumofWirteLength     = WriteLength / SPI_FLASH_PageSize;            //待写入页数
	uint8_t  NotEnoughNumofPage       = WriteLength % SPI_FLASH_PageSize;            //不足一页的数量
	uint8_t  WriteAddrPageAlignment   = WriteAddr % SPI_FLASH_PageSize;              //如果取余为0，则地址页对齐，可以写连续写入256字节
	uint8_t  NotAlignmentNumofPage    = SPI_FLASH_PageSize - WriteAddrPageAlignment; //地址不对齐部分，最多可以写入的字节数
	
	//写入地址页对齐
	if(WriteAddrPageAlignment == 0)
	{
		//待写入数据不足一页
		if(PageNumofWirteLength == 0)
		{
			SPI_Flash_WritePage(pWriteBuffer,WriteAddr,WriteLength);
		}
		//待写入数据超过一页
		else
		{
			//先写入整页
			while(PageNumofWirteLength--)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,SPI_FLASH_PageSize);
				pWriteBuffer += SPI_FLASH_PageSize;
				WriteAddr    += SPI_FLASH_PageSize;
			}
			//再写入不足一页的数据
			if(NotEnoughNumofPage > 0)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
	//写入地址与页不对齐
	else
	{
		//待写入数据不足一页
		if(PageNumofWirteLength == 0)
		{
			//不足一页的数据 <= 地址不对齐部分
			if(NotEnoughNumofPage <= NotAlignmentNumofPage)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,WriteLength);
			}
			//不足一页的数据 > 地址不对齐部分
			else
			{
				//先写地址不对齐部分允许写入的最大长度
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
				pWriteBuffer += NotAlignmentNumofPage;
				WriteAddr    += NotAlignmentNumofPage;
				
				//再写没写完的数据
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage-NotAlignmentNumofPage);
			}
		}
		//待写入数据超过一页
		else
		{
			//先写地址不对齐部分允许写入的最大长度，地址此时对齐了
		  SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
			pWriteBuffer += NotAlignmentNumofPage;
			WriteAddr    += NotAlignmentNumofPage;
			
			//地址对其后，重新计算写入页数与不足一页的数量
			WriteLength           -= NotAlignmentNumofPage;
			PageNumofWirteLength   = WriteLength / SPI_FLASH_PageSize;            //待写入页数
	    NotEnoughNumofPage     = WriteLength % SPI_FLASH_PageSize; 
			
			//先写入整页
			while(PageNumofWirteLength--)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,SPI_FLASH_PageSize);
				pWriteBuffer += SPI_FLASH_PageSize;
				WriteAddr    += SPI_FLASH_PageSize;
			}
			//再写入不足一页的数据
			if(NotEnoughNumofPage > 0)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
}

/*
	* @name   SPI_Flash_ReadUnfixed
	* @brief  读取不固定长度数据
	* @param  pReadBuffer：存放读取数据的缓存指针
  *         ReadAddr   ：读取地址
  *         ReadLength ：读取数据长度
	* @retval None
*/
static void SPI_Flash_ReadUnfixed(uint8_t* pReadBuffer, uint32_t ReadAddr, uint32_t ReadLength)
{
	//检测flash是否处于忙碌状态
	SPI_Flash_WaitForWriteEnd();
	
	//选择Flash芯片: CS输出低电平
	CLR_SPI_Flash_CS;
	
	//发送命令：读取数据
	SPI_Flash_WriteByte(W25X_ReadData);	
	//发送地址高字节
	SPI_Flash_WriteByte((ReadAddr & 0xFF0000) >> 16);
	//发送地址中字节
	SPI_Flash_WriteByte((ReadAddr & 0xFF00) >> 8);
	//发送地址低字节
	SPI_Flash_WriteByte(ReadAddr & 0xFF);
	//开始读取数据
	while (ReadLength--)
  {
     /* 读取一个字节*/
    *pReadBuffer = SPI_Flash_ReadByte();
    /* 指向下一个字节缓冲区 */
    pReadBuffer++;
  }
	
	//禁用Flash芯片: CS输出高电平
	SET_SPI_Flash_CS;
}
/********************************************************
  End Of File
********************************************************/
