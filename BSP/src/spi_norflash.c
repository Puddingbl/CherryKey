
#include "spi_norflash.h"
#include "spi.h"
#include "stdio.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
static void SPI_Flash_ReadFlashID(void);                          //��ȡFlashID
static void SPI_Flash_EraseSector(uint32_t);                      //��������(4kB)
static void SPI_Flash_EraseTotal(void);                           //����ȫ��
static void SPI_Flash_WriteUnfixed(uint8_t*,uint32_t,uint32_t);   //д�벻�̶���������
static void SPI_Flash_ReadUnfixed(uint8_t*,uint32_t,uint32_t);    //��ȡ���̶���������

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
static uint8_t SPI_Flash_ReadByte(void);      //��Flash��1���ֽ�
static void    SPI_Flash_WriteByte(uint8_t);  //��Flashд1���ֽ�

static void SPI_Flash_WriteEnable(void);      //Flashдʹ��
static void SPI_Flash_WaitForWriteEnd(void);  //�ȴ�Flashд�����

static void SPI_Flash_WritePage(uint8_t*,uint32_t,uint16_t);      //д��ҳ(256Bytes),д�볤�Ȳ�����256�ֽ�

/*
	* @name   SPI_Flash_ReadByte
	* @brief  ��Flash��1���ֽ�
	* @param  None
	* @retval ��ȡ��������     
*/
static uint8_t SPI_Flash_ReadByte()
{	
	uint8_t ReceiveByte;
	//�ȴ�ģʽ����1���ֽ�
	if(HAL_SPI_Receive(&hspi2,&ReceiveByte,1,0x0A) != HAL_OK)
		ReceiveByte = Dummy_Byte;
	//�����ֽ�
	return ReceiveByte;
}

/*
	* @name   SPI_Flash_ReadByte
	* @brief  ��Flashд1���ֽ�
	* @param  Byte -> ��д����ֽ�
	* @retval ��ȡ��������     
*/
static void SPI_Flash_WriteByte(uint8_t Byte)
{	
	uint8_t SendByte = Byte;
	//�ȴ�ģʽд��1���ֽ�
	HAL_SPI_Transmit(&hspi2,&SendByte,1,0x0A);
}

/*
	* @name   SPI_Flash_WriteEnable
	* @brief  Flashдʹ��
	* @param  None
	* @retval None
*/
static void SPI_Flash_WriteEnable()
{
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	//�������дʹ��
	SPI_Flash_WriteByte(W25X_WriteEnable);
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_WaitForWriteEnd
	* @brief  �ȴ�Flashд�����
	* @param  None
	* @retval None
*/
void SPI_Flash_WaitForWriteEnd()
{
	uint8_t Flash_Status1 = 0;
	
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	//���������״̬�Ĵ���1
	SPI_Flash_WriteByte(W25X_ReadStatusReg1);
	
	//������ȡ״̬�Ĵ���1��ֱ��WELΪ�͵�?
//	Timer6.usDelay_Timer = 0;
	do
	{
		Flash_Status1 = SPI_Flash_ReadByte();
		//��ֹ���⣬���ʼ���
//		if(Timer6.usDelay_Timer >= TIMER0_10S)
//			break;
	}
	while((Flash_Status1 & Flash_Status1_BUSY) == Flash_Status1_BUSY);
/*�����ܽ᣺�������ֲ�13ҳ�п�ͼ��״̬�Ĵ�����0λ��д��busy��־λ*/	
	//����FlashоƬ: CS����ߵ�ƽ

	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_WaitForWriteEnd
	* @brief  �ȴ�Flashд�����
	* @param  None
	* @retval None
*/
void SPI_Flash_writeReg()
{
	uint8_t Flash_Status1 = 0;
	
	SPI_Flash_WriteEnable();	
	
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	//���������״̬�Ĵ���1
	SPI_Flash_WriteByte(0x15);
	
	Flash_Status1 = SPI_Flash_ReadByte();
	printf("Flash_Status1=%d\n",Flash_Status1);
/*�����ܽ᣺�������ֲ�13ҳ�п�ͼ��״̬�Ĵ�����0λ��д��busy��־λ*/	
	//����FlashоƬ: CS����ߵ�ƽ

	SET_SPI_Flash_CS;
}

/*
	* @name   SPI_Flash_ReadFlashID
	* @brief  ��ȡFlashID
	* @param  None
	* @retval None
*/
static void SPI_Flash_ReadFlashID()
{
	uint8_t buf[3];
	
	//���flash�Ƿ���æµ״̬
	SPI_Flash_WaitForWriteEnd();
	
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	//���������ȡJEDEC ID(�豸��ʶ�� -> ������+�ڴ�����+����)
	SPI_Flash_WriteByte(W25X_ReadJedecID);
	buf[0] = SPI_Flash_ReadByte();
	buf[1] = SPI_Flash_ReadByte();
	buf[2] = SPI_Flash_ReadByte();
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
	
	SPI_Flash.JedecID = (buf[0] << 16) + (buf[1] << 8) + buf[2];
	printf("The JEDEC ID of SPI flash is 0x%.6X\r\n",SPI_Flash.JedecID);
	
	/***
	JEDEC ID      IC�ͺ�             �洢�ռ��С         
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
	* @brief  ��������(4kB)
	* @param  SectorAddr��������������ַ��Ҫ��Ϊ4k����
	* @retval None
*/
static void SPI_Flash_EraseSector(uint32_t SectorAddr)
{
	//���flash�Ƿ���æµ״̬
	SPI_Flash_WaitForWriteEnd();
	
	//Flashдʹ�ܣ��������
	SPI_Flash_WriteEnable();
	
	/* �������� */
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	
	//���������������
	SPI_Flash_WriteByte(W25X_SectorErase);
	//���Ͳ���������ַ�ĸ��ֽ�
	SPI_Flash_WriteByte((SectorAddr & 0xFF0000) >> 16);
	//���Ͳ���������ַ�����ֽ�
	SPI_Flash_WriteByte((SectorAddr & 0xFF00) >> 8);
	//���Ͳ���������ַ�ĵ��ֽ�
	SPI_Flash_WriteByte(SectorAddr & 0xFF);	
	
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
	
	//�ȴ��������
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_EraseTotal
	* @brief  ����ȫ��
	* @param  None
	* @retval None
*/
static void SPI_Flash_EraseTotal()
{
	//���flash�Ƿ���æµ״̬
	SPI_Flash_WaitForWriteEnd();
	
	//Flashдʹ�ܣ��������
	SPI_Flash_WriteEnable();
	
	/* ������Ƭ */
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	
	//���������Ƭ����
	SPI_Flash_WriteByte(W25X_ChipErase);	
	
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
	
	//�ȴ��������
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_WritePage
	* @brief  д��ҳ(256Bytes),д�볤�Ȳ�����256�ֽ�
	* @param  pWriteBuffer����д�����ݵ�ָ��
  *         WriteAddr   ��д���ַ
  *         WriteLength ��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize(256Bytes)
	* @retval None
*/
static void SPI_Flash_WritePage(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint16_t WriteLength)
{
	//���flash�Ƿ���æµ״̬
	SPI_Flash_WaitForWriteEnd();
	
	//Flashдʹ�ܣ�����д��
	SPI_Flash_WriteEnable();
	
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	
	//�������ҳ����
	SPI_Flash_WriteByte(W25X_PageProgram);	
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte((WriteAddr & 0xFF0000) >> 16);
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte((WriteAddr & 0xFF00) >> 8);
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte(WriteAddr & 0xFF);
	
	if(WriteLength > SPI_FLASH_PageSize)
  {
     WriteLength = SPI_FLASH_PageSize;
		printf("Error: Flash:can't more than 256!\n");
  }
	
	//��ʼд������
	while (WriteLength--)
  {
     /* ��ȡһ���ֽ�*/
    SPI_Flash_WriteByte(*pWriteBuffer);
    /* ָ����һ���ֽڻ����� */
    pWriteBuffer++;
  }
	
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
	
	//�ȴ�д�����
	SPI_Flash_WaitForWriteEnd();
}

/*
	* @name   SPI_Flash_WriteUnfixed
	* @brief  д�벻�̶���������
	* @param  pWriteBuffer����д�����ݵĻ���ָ��
  *         WriteAddr   ��д���ַ
  *         WriteLength ��д�����ݳ���
	* @retval None
*/
static void SPI_Flash_WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
	uint32_t PageNumofWirteLength     = WriteLength / SPI_FLASH_PageSize;            //��д��ҳ��
	uint8_t  NotEnoughNumofPage       = WriteLength % SPI_FLASH_PageSize;            //����һҳ������
	uint8_t  WriteAddrPageAlignment   = WriteAddr % SPI_FLASH_PageSize;              //���ȡ��Ϊ0�����ַҳ���룬����д����д��256�ֽ�
	uint8_t  NotAlignmentNumofPage    = SPI_FLASH_PageSize - WriteAddrPageAlignment; //��ַ�����벿�֣�������д����ֽ���
	
	//д���ַҳ����
	if(WriteAddrPageAlignment == 0)
	{
		//��д�����ݲ���һҳ
		if(PageNumofWirteLength == 0)
		{
			SPI_Flash_WritePage(pWriteBuffer,WriteAddr,WriteLength);
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,SPI_FLASH_PageSize);
				pWriteBuffer += SPI_FLASH_PageSize;
				WriteAddr    += SPI_FLASH_PageSize;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
	//д���ַ��ҳ������
	else
	{
		//��д�����ݲ���һҳ
		if(PageNumofWirteLength == 0)
		{
			//����һҳ������ <= ��ַ�����벿��
			if(NotEnoughNumofPage <= NotAlignmentNumofPage)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,WriteLength);
			}
			//����һҳ������ > ��ַ�����벿��
			else
			{
				//��д��ַ�����벿������д�����󳤶�
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
				pWriteBuffer += NotAlignmentNumofPage;
				WriteAddr    += NotAlignmentNumofPage;
				
				//��дûд�������
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage-NotAlignmentNumofPage);
			}
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д��ַ�����벿������д�����󳤶ȣ���ַ��ʱ������
		  SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
			pWriteBuffer += NotAlignmentNumofPage;
			WriteAddr    += NotAlignmentNumofPage;
			
			//��ַ��������¼���д��ҳ���벻��һҳ������
			WriteLength           -= NotAlignmentNumofPage;
			PageNumofWirteLength   = WriteLength / SPI_FLASH_PageSize;            //��д��ҳ��
	    NotEnoughNumofPage     = WriteLength % SPI_FLASH_PageSize; 
			
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,SPI_FLASH_PageSize);
				pWriteBuffer += SPI_FLASH_PageSize;
				WriteAddr    += SPI_FLASH_PageSize;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				SPI_Flash_WritePage(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
}

/*
	* @name   SPI_Flash_ReadUnfixed
	* @brief  ��ȡ���̶���������
	* @param  pReadBuffer����Ŷ�ȡ���ݵĻ���ָ��
  *         ReadAddr   ����ȡ��ַ
  *         ReadLength ����ȡ���ݳ���
	* @retval None
*/
static void SPI_Flash_ReadUnfixed(uint8_t* pReadBuffer, uint32_t ReadAddr, uint32_t ReadLength)
{
	//���flash�Ƿ���æµ״̬
	SPI_Flash_WaitForWriteEnd();
	
	//ѡ��FlashоƬ: CS����͵�ƽ
	CLR_SPI_Flash_CS;
	
	//���������ȡ����
	SPI_Flash_WriteByte(W25X_ReadData);	
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte((ReadAddr & 0xFF0000) >> 16);
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte((ReadAddr & 0xFF00) >> 8);
	//���͵�ַ���ֽ�
	SPI_Flash_WriteByte(ReadAddr & 0xFF);
	//��ʼ��ȡ����
	while (ReadLength--)
  {
     /* ��ȡһ���ֽ�*/
    *pReadBuffer = SPI_Flash_ReadByte();
    /* ָ����һ���ֽڻ����� */
    pReadBuffer++;
  }
	
	//����FlashоƬ: CS����ߵ�ƽ
	SET_SPI_Flash_CS;
}
/********************************************************
  End Of File
********************************************************/
