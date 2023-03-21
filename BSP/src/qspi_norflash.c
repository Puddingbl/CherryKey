/**
  ******************************************************************************
  * @file    bsp_qspi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   qspi flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 H743 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "qspi_norflash.h"
#include "quadspi.h"
#include "stdio.h"

//QSPI_HandleTypeDef hqspi;

/**
	voidд�Ĵ���
	void���Ĵ���
	
	void�ȴ�busyλ
	
	voidдʹ��
	
	voidд����
	voidд����
	voidд����+д����
	void������
	
	void��ʼ��
*/
/**
  * @brief  ����QSPIΪ�ڴ�ӳ��ģʽ
  * @retval QSPI�ڴ�״̬
  */
uint32_t QSPI_EnableMemoryMappedMode()
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* ���ö�ָ�� */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = QUAD_INOUT_FAST_READ_CMD_4BYTE1;
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
//	s_command.Address           = 0x00000020;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 6;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* �����ڴ�ӳ��ģʽ */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 5;

  if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return QSPI_ERROR;
  }
	
//	/* �������� */
//	if (HAL_QSPI_Transmit(&hqspi, &value,5000) != HAL_OK)
//	{
//		printf("hear5\n");
//		return QSPI_ERROR;
//	}

  return QSPI_OK;
}
/**
  * @brief  ����QSPI�洢��Ϊ4-byte��ַģʽ
  * @param  ��
  * @retval ����״̬
  */
uint8_t QSPI_EnterFourBytesAddress(void)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = ENTER_4_BYTE_ADDR_MODE_CMD; //WRITE_STATUS_REG2_CMD
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* ʹ��д���� */
  QSPI_WriteEnable();
  
  /* �������� */
  if (HAL_QSPI_Command(&hqspi, &s_command,5000) != HAL_OK)
  {
    return QSPI_ERROR;
  }

	/* �Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(5000) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;

}
/**
  * @brief  ��ʼ��QSPI�洢��
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Init(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t value = 0X02;
	/* ʹ��д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	
	QSPI_FLASH_ReadStatusReg(3);
	
	/* ������·ʹ�ܵ�״̬�Ĵ�����ʹ����ͨ��IO2��IO3���� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_STATUS_REG2_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Transmit(&hqspi, &value,5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(5000) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	
  /* QSPI memory reset */
  if (QSPI_ResetMemory() != QSPI_OK)
  {
    return QSPI_ERROR;
  }
	
	
//	QSPI_EnterFourBytesAddress();

	
	return QSPI_OK;
}

/**
  * @brief  ��QSPI�洢���ж�ȡ��������.
  * @param  pData: ָ��Ҫ��ȡ�����ݵ�ָ��
  * @param  ReadAddr: ��ȡ��ʼ��ַ
  * @param  Size: Ҫ��ȡ�����ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
  
  if(Size == 0)
  {
    printf("BSP_QSPI_Read Size = 0");
    return QSPI_OK;
  }

	/* ��ʼ�������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_CMD_4BYTE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = Size;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{      
		return QSPI_ERROR;
	}

	/* �������� */
	if (HAL_QSPI_Receive(&hqspi, pData, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}
/**
  * @brief  ��QSPI�洢���ж�ȡ��������.
  * @param  pData: ָ��Ҫ��ȡ�����ݵ�ָ��
  * @param  ReadAddr: ��ȡ��ʼ��ַ
  * @param  Size: Ҫ��ȡ�����ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_FastRead(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;

  if(Size == 0)
  {
    printf("BSP_QSPI_FastRead Size = 0");
    return QSPI_OK;
  }
	/* ��ʼ�������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = 0xeb;
	s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 6;
	s_command.NbData            = Size;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* �������� */
	if (HAL_QSPI_Receive(&hqspi, pData, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ����������д��QSPI�洢��
  * @param  pData: ָ��Ҫд�����ݵ�ָ��
  * @param  WriteAddr: д��ʼ��ַ
  * @param  Size: Ҫд������ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;
  
  if(Size == 0)
  {
    printf("BSP_QSPI_Write Size = 0");
    return QSPI_OK;
  }
	
	if(Size > 256)
  {
     Size = 256;
		printf("Error: Flashÿ��д�����ݲ��ܳ���256�ֽڣ�\n");
  }

	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = QUAD_INPUT_PAGE_PROG_CMD_4BYTE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	s_command.Address = WriteAddr;
	s_command.NbData  = Size;
	
	/* дʹ�� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	
	/* ����qspiҳ�������(0x32) */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}	
	
	/* ��ҳִ��д�� */
	while (Size--)
	{
		/* �������� */
		if (HAL_QSPI_Transmit(&hqspi, pData, 5000) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		pData += 1;
	} 
	
	/* �����Զ���ѯģʽ�ȴ�������� */  
	if (QSPI_AutoPollingMemReady(5000) != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	
	return QSPI_OK;
}

/**
  * @brief  ����������д��QSPI�洢��
  * @param  pData: ָ��Ҫд�����ݵ�ָ��
  * @param  WriteAddr: д��ʼ��ַ
  * @param  Size: Ҫд������ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_WritePP(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;
  
  if(Size == 0)
  {
    printf("BSP_QSPI_Write Size = 0");
    return QSPI_OK;
  }
	
	if(Size > 256)
  {
     Size = 256;
		printf("Error: Flashÿ��д�����ݲ��ܳ���256�ֽڣ�\n");
  }

	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = QUAD_INPUT_PAGE_PROG_CMD_1BYTE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;
	
	s_command.Address = WriteAddr;
	s_command.NbData  = Size;
	
	/* дʹ�� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	
	/* ����qspiҳ�������(0x02) */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}	
	
	/* ��ҳִ��д�� */
	while (Size--)
	{
		/* �������� */
		if (HAL_QSPI_Transmit(&hqspi, pData, 5000) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		pData += 1;
	} 
	
	/* �����Զ���ѯģʽ�ȴ�������� */  
	if (QSPI_AutoPollingMemReady(5000) != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	
	return QSPI_OK;
}

void BSP_QSPI_WriteUnfixedPP(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
	uint32_t PageNumofWirteLength     = WriteLength / W25Q256JV_PAGE_SIZE;            //��д��ҳ��
	uint8_t  NotEnoughNumofPage       = WriteLength % W25Q256JV_PAGE_SIZE;            //����һҳ������
	uint8_t  WriteAddrPageAlignment   = WriteAddr % W25Q256JV_PAGE_SIZE;              //���ȡ��Ϊ0�����ַҳ���룬����д����д��256�ֽ�
	uint8_t  NotAlignmentNumofPage    = W25Q256JV_PAGE_SIZE - WriteAddrPageAlignment; //��ַ�����벿�֣�������д����ֽ���
	
	//д���ַҳ����
	if(WriteAddrPageAlignment == 0)
	{
		//��д�����ݲ���һҳ
		if(PageNumofWirteLength == 0)
		{
			BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,WriteLength);
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,W25Q256JV_PAGE_SIZE);
				pWriteBuffer += W25Q256JV_PAGE_SIZE;
				WriteAddr    += W25Q256JV_PAGE_SIZE;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
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
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,WriteLength);
			}
			//����һҳ������ > ��ַ�����벿��
			else
			{
				//��д��ַ�����벿������д�����󳤶�
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
				pWriteBuffer += NotAlignmentNumofPage;
				WriteAddr    += NotAlignmentNumofPage;
				
				//��дûд�������
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,NotEnoughNumofPage-NotAlignmentNumofPage);
			}
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д��ַ�����벿������д�����󳤶ȣ���ַ��ʱ������
		  BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
			pWriteBuffer += NotAlignmentNumofPage;
			WriteAddr    += NotAlignmentNumofPage;
			
			//��ַ��������¼���д��ҳ���벻��һҳ������
			WriteLength           -= NotAlignmentNumofPage;
			PageNumofWirteLength   = WriteLength / W25Q256JV_PAGE_SIZE;            //��д��ҳ��
	    NotEnoughNumofPage     = WriteLength % W25Q256JV_PAGE_SIZE; 
			
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,W25Q256JV_PAGE_SIZE);
				pWriteBuffer += W25Q256JV_PAGE_SIZE;
				WriteAddr    += W25Q256JV_PAGE_SIZE;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				BSP_QSPI_WritePP(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
}

void BSP_QSPI_WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
	uint32_t PageNumofWirteLength     = WriteLength / W25Q256JV_PAGE_SIZE;            //��д��ҳ��
	uint8_t  NotEnoughNumofPage       = WriteLength % W25Q256JV_PAGE_SIZE;            //����һҳ������
	uint8_t  WriteAddrPageAlignment   = WriteAddr % W25Q256JV_PAGE_SIZE;              //���ȡ��Ϊ0�����ַҳ���룬����д����д��256�ֽ�
	uint8_t  NotAlignmentNumofPage    = W25Q256JV_PAGE_SIZE - WriteAddrPageAlignment; //��ַ�����벿�֣�������д����ֽ���
	
	//д���ַҳ����
	if(WriteAddrPageAlignment == 0)
	{
		//��д�����ݲ���һҳ
		if(PageNumofWirteLength == 0)
		{
			BSP_QSPI_Write(pWriteBuffer,WriteAddr,WriteLength);
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,W25Q256JV_PAGE_SIZE);
				pWriteBuffer += W25Q256JV_PAGE_SIZE;
				WriteAddr    += W25Q256JV_PAGE_SIZE;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
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
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,WriteLength);
			}
			//����һҳ������ > ��ַ�����벿��
			else
			{
				//��д��ַ�����벿������д�����󳤶�
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
				pWriteBuffer += NotAlignmentNumofPage;
				WriteAddr    += NotAlignmentNumofPage;
				
				//��дûд�������
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,NotEnoughNumofPage-NotAlignmentNumofPage);
			}
		}
		//��д�����ݳ���һҳ
		else
		{
			//��д��ַ�����벿������д�����󳤶ȣ���ַ��ʱ������
		  BSP_QSPI_Write(pWriteBuffer,WriteAddr,NotAlignmentNumofPage);				
			pWriteBuffer += NotAlignmentNumofPage;
			WriteAddr    += NotAlignmentNumofPage;
			
			//��ַ��������¼���д��ҳ���벻��һҳ������
			WriteLength           -= NotAlignmentNumofPage;
			PageNumofWirteLength   = WriteLength / W25Q256JV_PAGE_SIZE;            //��д��ҳ��
	    NotEnoughNumofPage     = WriteLength % W25Q256JV_PAGE_SIZE; 
			
			//��д����ҳ
			while(PageNumofWirteLength--)
			{
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,W25Q256JV_PAGE_SIZE);
				pWriteBuffer += W25Q256JV_PAGE_SIZE;
				WriteAddr    += W25Q256JV_PAGE_SIZE;
			}
			//��д�벻��һҳ������
			if(NotEnoughNumofPage > 0)
			{
				BSP_QSPI_Write(pWriteBuffer,WriteAddr,NotEnoughNumofPage);
			}
		}
	}
}

/**
  * @brief  ����QSPI�洢����ָ���� 
  * @param  BlockAddress: ��Ҫ�����Ŀ��ַ  
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress)
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = SECTOR_ERASE_CMD_4BYTE;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = BlockAddress;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* ����д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		printf("����");
		return QSPI_ERROR;
	}
  //QSPI_FLASH_Wait_Busy();
	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("����1");
		return QSPI_ERROR;
	}
  //QSPI_FLASH_Wait_Busy();
	/* �����Զ���ѯģʽ�ȴ��������� */  
	if (QSPI_AutoPollingMemReady(W25Q256JV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
	{
		printf("����2");
		return QSPI_ERROR;
	}
  
	return QSPI_OK;
}

/**
  * @brief  ��������QSPI�洢��
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = CHIP_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* ����д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* �����Զ���ѯģʽ�ȴ��������� */  
	if (QSPI_AutoPollingMemReady(W25Q256JV_BULK_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ��ȡQSPI�洢���ĵ�ǰ״̬
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_GetStatus(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t reg;
	/* ��ʼ����ȡ״̬�Ĵ������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Receive(&hqspi, &reg, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* ���Ĵ�����ֵ */
	if((reg & W25Q256JV_FSR_BUSY) != 0)
	{
		return QSPI_BUSY;
	}
	else
	{
		return QSPI_OK;
	}
}

/**
  * @brief  ����QSPI�洢��������
  * @param  pInfo: �����ýṹ�ϵ�ָ��  
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_GetInfo(QSPI_Info* pInfo)
{
	/* ���ô洢�����ýṹ */
	pInfo->FlashSize          = W25Q256JV_FLASH_SIZE;
	pInfo->EraseSectorSize    = W25Q256JV_SUBSECTOR_SIZE;
	pInfo->EraseSectorsNumber = (W25Q256JV_FLASH_SIZE/W25Q256JV_SUBSECTOR_SIZE);
	pInfo->ProgPageSize       = W25Q256JV_PAGE_SIZE;
	pInfo->ProgPagesNumber    = (W25Q256JV_FLASH_SIZE/W25Q256JV_PAGE_SIZE);
	return QSPI_OK;
}

/**
  * @brief  ��λQSPI�洢����
  * @param  hqspi: QSPI���
  * @retval ��
  */
uint8_t QSPI_ResetMemory()
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ����λʹ������ */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = RESET_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("hear8\n");
		return QSPI_ERROR;
	}

	/* ���͸�λ�洢������ */
	s_command.Instruction = RESET_MEMORY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("hear7\n");
		return QSPI_ERROR;
	}

	/* �����Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(5000) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}


/**
  * @brief  ����д��ʹ�ܣ��ȴ�����Ч.
  * @param  hqspi: QSPI���
  * @retval ��
  */
static uint8_t QSPI_WriteEnable()
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* ����д���� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	
	/* �����Զ���ѯģʽ�ȴ�д���� */  
	s_config.Match           = W25Q256JV_FSR_WREN;
	s_config.Mask            = W25Q256JV_FSR_WREN;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.Instruction    = READ_STATUS_REG1_CMD;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.NbData         = 1;
	
	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, 1000) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	
	
	return QSPI_OK;
}

/**
  * @brief  ��ȡ�洢����SR���ȴ�EOP
  * @param  hqspi: QSPI���
  * @param  Timeout ��ʱ
  * @retval ��
  */
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* �����Զ���ѯģʽ�ȴ��洢��׼������ */  
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	s_config.Match           = 0x00;
	s_config.Mask            = W25Q256JV_FSR_BUSY;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, Timeout) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t QSPI_FLASH_ReadID(void)
{
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/* ��ȡJEDEC ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_JEDEC_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 3;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&hqspi, pData, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}

	Temp = ( pData[2] | pData[1]<<8 )| ( pData[0]<<16 );

	return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
uint32_t QSPI_FLASH_ReadDeviceID(void)
{  
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/*##-2-��ȡ�豸ID����    ###########################################*/
	/* ��ȡ����/�豸 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 2;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&hqspi, pData, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}

	Temp = pData[1] |( pData[0]<<8 ) ;

	return Temp;
}

 /**
  * @brief  ��ȡReadStatusReg
  * @param 	��
  * @retval ReadStatusReg
  */
uint32_t QSPI_FLASH_ReadStatusReg(uint8_t reg)
{  
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[10];

	/* ��ȡ����/�豸 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	if(reg == 1)
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	else if(reg == 2)
	s_command.Instruction       = READ_STATUS_REG2_CMD;
	else if(reg == 3)
	s_command.Instruction       = READ_STATUS_REG3_CMD;
	
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&hqspi, pData, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	
	//��flashʱ��ȡ���ֽ���ΪpData[0]��pData[1]
	//Temp = pData[0] |( pData[1]<<8 ) ;
	
	//˫flashʱ��ȡ���ֽ���ΪpData[0]��pData[2]
	//Temp = pData[1] |( pData[0]<<8 ) ;
		Temp = pData[0] ;
	return Temp;
}

 /**
  * @brief  дWriteStatusReg
  * @param 	��
  * @retval ReadStatusReg
  */
uint32_t QSPI_FLASH_WriteStatusReg(uint8_t reg,uint8_t regvalue)
{  
	QSPI_CommandTypeDef s_command;
	
	/* ʹ��д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	/* ��ȡ����/�豸 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	if(reg == 1)
	s_command.Instruction       = WRITE_STATUS_REG1_CMD;
	else if(reg == 2)
	s_command.Instruction       = WRITE_STATUS_REG2_CMD;
	else if(reg == 3)
	s_command.Instruction       = WRITE_STATUS_REG3_CMD;
	
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Transmit(&hqspi, &regvalue, 5000) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	/* �Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(W25Q256JV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

//void QSPI_Set_WP_High(void)
//{
//	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	__GPIOF_CLK_ENABLE();
//	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
//	GPIO_InitStruct.Pin = GPIO_PIN_7;	
//	/*�������ŵ��������Ϊ�������*/
//	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
//	/*��������Ϊ����ģʽ*/
//	GPIO_InitStruct.Pull  = GPIO_PULLUP;
//	/*������������Ϊ���� */   
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
//	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
//	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);	
//	
//	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,1);
//}
//void QSPI_Set_WP_TO_QSPI_IO(void)
//{
//	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	QSPI_FLASH_BK1_IO2_CLK_ENABLE();

//	//��������
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	/*!< ���� QSPI_FLASH ����: IO2 */
//	GPIO_InitStruct.Pin = QSPI_FLASH_BK1_IO2_PIN;
//	GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO2_AF;
//	HAL_GPIO_Init(QSPI_FLASH_BK1_IO2_PORT, &GPIO_InitStruct);
//}

//�ȴ�����
void QSPI_FLASH_Wait_Busy(void)   
{   
	while((QSPI_FLASH_ReadStatusReg(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}   

/*********************************************END OF FILE**********************/
