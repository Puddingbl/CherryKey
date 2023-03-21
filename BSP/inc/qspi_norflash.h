/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __QSPI_NORFLASH_H
#define __QSPI_NORFLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
//#include "public.h"
#include "quadspi.h"


/* define---------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#define  sFLASH_ID                         0XEF4019     //W25Q256JVEM

/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)


/* W25Q256JV Micron memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE            22     /* ��ַ���߿�ȷ��������ڴ�ռ� */
#define QSPI_PAGE_SIZE             256

/* QSPI Info */
typedef struct {
  uint32_t FlashSize;          /*!< �����С */
  uint32_t EraseSectorSize;    /*!< ����������������С */
  uint32_t EraseSectorsNumber; /*!< ���������������� */
  uint32_t ProgPageSize;       /*!< ��̲�����ҳ���С */
  uint32_t ProgPagesNumber;    /*!< ��̲�����ҳ���� */
} QSPI_Info;

/* Private define ------------------------------------------------------------*/
/*�����-��ͷ*******************************/
/** 
  * @brief  W25Q256JV���� 
  */  
#define W25Q256JV_FLASH_SIZE                  0x800000 /* 128 MBits => 16MBytes */
#define W25Q256JV_SECTOR_SIZE                 0x10000   /* 256 sectors of 64KBytes */
#define W25Q256JV_SUBSECTOR_SIZE              0x1000    /* 4096 subsectors of 4kBytes */
#define W25Q256JV_PAGE_SIZE                   0x100     /* 65536 pages of 256 bytes */

#define W25Q256JV_DUMMY_CYCLES_READ           4
#define W25Q256JV_DUMMY_CYCLES_READ_QUAD      10

#define W25Q256JV_BULK_ERASE_MAX_TIME         250000
#define W25Q256JV_SECTOR_ERASE_MAX_TIME       3000
#define W25Q256JV_SUBSECTOR_ERASE_MAX_TIME    800

/** 
  * @brief  W25Q256JV ָ��  
  */  
/* ��λ���� */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

#define ENTER_QPI_MODE_CMD                   0x38
#define EXIT_QPI_MODE_CMD                    0xFF

/* ʶ����� */
#define READ_ID_CMD                          0x90
#define DUAL_READ_ID_CMD                     0x92
#define QUAD_READ_ID_CMD                     0x94
#define READ_JEDEC_ID_CMD                    0x9F

/* ������ */
#define READ_CMD_4BYTE                       0x03
#define QUAD_INOUT_FAST_READ_CMD_4BYTE       0x6B
#define QUAD_INOUT_FAST_READ_CMD_4BYTE1       0xEB

/* д���� */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* �Ĵ������� */
#define READ_STATUS_REG1_CMD                  0x05
#define READ_STATUS_REG2_CMD                  0x35
#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 0x01
#define WRITE_STATUS_REG2_CMD                 0x31
#define WRITE_STATUS_REG3_CMD                 0x11
/* ��̲��� */
#define QUAD_INPUT_PAGE_PROG_CMD_1BYTE        0x02
#define QUAD_INPUT_PAGE_PROG_CMD_4BYTE        0x32
#define EXT_QUAD_IN_FAST_PROG_CMD_4BYTE       0x12

/* �������� */
#define SECTOR_ERASE_CMD_4BYTE                0x20
#define BLOCK64K_ERASE_CMD_4BYTE              0xD8 
#define CHIP_ERASE_CMD                        0xC7

//#define PROG_ERASE_RESUME_CMD                 0x7A
//#define PROG_ERASE_SUSPEND_CMD                0x75

#define ENTER_4_BYTE_ADDR_MODE_CMD            0xB7

/* ״̬�Ĵ�����־ */
#define W25Q256JV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q256JV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q256JV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */
/*�����-��β*******************************/



/* extern variables-----------------------------------------------------------*/


/* exported functions ------------------------------------------------------- */
void QSPI_FLASH_Init(void);
uint8_t BSP_QSPI_Init(void);
uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress);
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_QSPI_FastRead(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);

uint8_t BSP_QSPI_Erase_Chip(void);
static uint8_t QSPI_WriteEnable          (void);
static uint8_t QSPI_AutoPollingMemReady  (uint32_t Timeout);

uint32_t QSPI_FLASH_ReadDeviceID(void);
uint32_t QSPI_FLASH_ReadID(void);
uint32_t QSPI_FLASH_ReadStatusReg(uint8_t reg);
uint32_t QSPI_FLASH_WriteStatusReg(uint8_t reg,uint8_t regvalue);
void QSPI_Set_WP_High(void);
void QSPI_Set_WP_TO_QSPI_IO(void);
void QSPI_FLASH_Wait_Busy(void);
uint32_t QSPI_EnableMemoryMappedMode(void); //ʹ��ֱ��ӳ��ģʽ
uint8_t QSPI_ResetMemory(void);
void BSP_QSPI_WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength);
void BSP_QSPI_WriteUnfixedPP(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength);
uint8_t BSP_QSPI_WritePP(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
#ifdef __cplusplus
}
#endif

#endif
