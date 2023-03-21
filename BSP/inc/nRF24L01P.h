/**@file  	    nRF24L01P.h
* @brief            nRF24L01+ low level operations and configurations.
* @author           hyh
* @date             2021.6.9
* @version          1.0
* @copyright        Chengdu Ebyte Electronic Technology Co.Ltd
**********************************************************************************
*/
#ifndef nRF24L01P_H
#define nRF24L01P_H

#include "main.h"
#include "usart.h"
#include "spi.h"
#include "nRF24L01P_REG.h"
#include "stdio.h"

#define IRQ_ALL ((1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT))
/*Data Rate selection*/
typedef enum {DRATE_250K,DRATE_1M,DRATE_2M}L01_DRATE;
/*Power selection*/
typedef enum {POWER_N_0,POWER_N_6,POWER_N_12,POWER_N_18}L01_PWR;
/*Mode selection*/
typedef enum {TX_MODE,RX_MODE}L01_MODE;
/*CE pin level selection*/
typedef enum {CE_LOW,CE_HIGH}CE_STAUS;

/*Select whether to use ACK mechanism for sending 
nrf_init_flag  : 1--->ACK
                 0--->NO_ACK
*/
#define nrf_init_flag   1



/*
================================================================================
============================Configurations and Options==========================
================================================================================
*/
#define DYNAMIC_PACKET      1 //1:DYNAMIC packet length, 0:fixed
#define FIXED_PACKET_LEN    32//Packet size in fixed size mode
#define INIT_ADDR           1,2,3,4,5
/*
================================================================================
==========================List of externally provided functions ================
================================================================================
*/
#define L01_CSN_LOW()      HAL_GPIO_WritePin(SPI3_CS_GPIO_Port,SPI3_CS_Pin,GPIO_PIN_RESET)//Pull down the SPI chip select
#define L01_CSN_HIGH()     HAL_GPIO_WritePin(SPI3_CS_GPIO_Port,SPI3_CS_Pin,GPIO_PIN_SET)//Pull up the SPI chip select
#define L01_CE_LOW()       HAL_GPIO_WritePin(SI24R1_CE_GPIO_Port,SI24R1_CE_Pin,GPIO_PIN_RESET)//Set CE low level
#define L01_CE_HIGH()      HAL_GPIO_WritePin(SI24R1_CE_GPIO_Port,SI24R1_CE_Pin,GPIO_PIN_SET)//Set CE high level
#define GET_L01_IRQ()      HAL_GPIO_ReadPin(SI24R1_IRQ_GPIO_Port, SI24R1_IRQ_Pin)//Get the IRQ pin status
#define SPI_ExchangeByte(data)      BSP_SPI_ExchangeByte(data) //Exchange data by the SPI
/*
================================================================================
-------------------------------------Exported APIs------------------------------
================================================================================
*/
/*Set the level status of the CE pin low or high*/
void L01_SetCE(CE_STAUS status);
/*Read the value from the specified register */
uint8_t L01_ReadSingleReg(uint8_t addr);
/*Read the values of the specified registers and store them in buffer*/
void L01_ReadMultiReg(uint8_t start_addr,uint8_t *buffer,uint8_t size);
/*Write a value to the specified register*/
void L01_WriteSingleReg(uint8_t addr,uint8_t value);
/*Write buffer to the specified registers */
void L01_WriteMultiReg(uint8_t start_addr,uint8_t *buffer,uint8_t size);
/*Set the nRF24L01 into PowerDown mode */
void L01_SetPowerDown(void);
/*Set the nRF24L01 into PowerUp mode*/
void L01_SetPowerUp(void);
/*Flush the TX buffer*/
void L01_FlushTX(void);
/*Flush the RX buffer*/
void L01_FlushRX(void);
/*Reuse the last transmitted payload*/
void L01_ReuseTXPayload(void);
/*Read the status register of the nRF24L01*/
uint8_t L01_ReadStatusReg(void);
/*Clear the IRQ caused by the nRF24L01+*/
void L01_ClearIRQ(uint8_t irqMask);
/*Read the IRQ status of the nRF24L01+*/
uint8_t L01_ReadIRQSource(void);
/*Read the payload width of the top buffer of the FIFO */
uint8_t L01_ReadTopFIFOWidth(void);
/*Read the RX payload from the FIFO and store them in buffer*/
uint8_t L01_ReadRXPayload(uint8_t *buffer);
/*Write TX Payload to a data pipe,and PRX will return ACK back*/
void L01_WriteTXPayload_Ack(uint8_t *buffer,uint8_t size);
/*Write TX Payload to a data pipe,and PRX won't return ACK back*/
void L01_WriteTXPayload_NoAck(uint8_t *buffer,uint8_t size);
/*Write TX Payload to a data pipe when RX mode*/
void L01_WriteRXPayload_InAck(uint8_t *buffer,uint8_t size);
/*Write Transmit address into TX_ADDR register */
void L01_SetTXAddr(uint8_t *Addrbuffer,uint8_t Addr_size);
/*Write address for the RX pipe*/
void L01_SetRXAddr(uint8_t pipeNum,uint8_t *addrBuffer,uint8_t addr_size);
/*Set the data rate of the nRF24L01+ */
void L01_SetDataRate(L01_DRATE drate);
/*Set the power of the nRF24L01+ */
void L01_SetPower(L01_PWR power);
/*Set the frequency of the nRF24L01+*/
void L01_WriteHoppingPoint(uint8_t freq);
/*Set the nRF24L01+ as TX/RX mode*/
void L01_SetTRMode(L01_MODE mode);
/*Initialize the nRF24L01+ */
void L01_Init(void);
uint8_t BSP_SPI_ExchangeByte(uint8_t w_data);
void APP_SwitchToRx(void);
void APP_SwitchToTx(void);
void APP_TransmitHandler(void);
uint8_t spi_rw_mode0(uint8_t writeData);
#endif