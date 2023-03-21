/**@file  	    nRF24L01P.c
* @brief            nRF24L01+ low level operations and configurations.
* @author           hyh
* @date             2021.9.17
* @version          1.0
* @copyright        Chengdu Ebyte Electronic Technology Co.Ltd
**********************************************************************************
*/
#include "nRF24L01P.h"


uint8_t spi_rw_mode0(uint8_t writeData)
{
	uint8_t i = 0;
	uint8_t readData = 0;
	
	for(i=0; i<8; i++)
	{
		if(writeData>>7 & 0x01)
		{
			HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port, SPI3_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port, SPI3_MOSI_Pin,GPIO_PIN_RESET);
		}
		writeData = writeData<<1;
//		delay_us(1);
		HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_SET);
		
		readData <<= 1;
		if(HAL_GPIO_ReadPin(SPI3_MISO_GPIO_Port, GPIO_PIN_11) & 0x01)
		{
			readData++;
		}				
//		delay_us(1);
		HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_RESET);
	}

		return readData;
}



/*the CE pin level status*/
static uint8_t CE_Status = 0;
/*!
================================================================================
------------------------------------Functions-----------------------------------
================================================================================
*/
/*!
 *  @brief          Get the level status of the CE pin
 *  @param          None     
 *  @return         CE pin level status: 0:low; 1:high
 *  @note          
*/
uint8_t L01_GetCEStatus(void)
{
    return CE_Status;
}
/*!
 *  @brief          Set the level status of the CE pin low or high
 *  @param          status:CE pin level status    
 *  @return         None
 *  @note          
*/
void L01_SetCE(CE_STAUS status)
{
    CE_Status = status;
    if (status == CE_LOW)    { L01_CE_LOW(); }
    else                     { L01_CE_HIGH(); }
}
/*!
 *  @brief        Read the value from the specified register   
 *  @param        addr:the address of the register
 *  @return       value:the value read from the register  
 *  @note          
*/
uint8_t L01_ReadSingleReg(uint8_t addr)
{
    uint8_t value;
    L01_CSN_LOW();
    SPI_ExchangeByte( R_REGISTER | addr);
    value = SPI_ExchangeByte(NOP);
    L01_CSN_HIGH();
    return value;
}
/*!
 *  @brief        Read the values of the specified registers and store them in buffer
 *  @param        start_addr:the start address of the registers
 *  @param        buffer:the buffer stores the read values
*  @param         size:the size to be read
 *  @return       None  
 *  @note          
*/
void L01_ReadMultiReg(uint8_t start_addr,uint8_t *buffer,uint8_t size)
{
    uint8_t i;
    L01_CSN_LOW();
    SPI_ExchangeByte(R_REGISTER | start_addr);
    for (i = 0; i < size; i++)
    {
        *(buffer + i) = SPI_ExchangeByte(NOP);
    }
    L01_CSN_HIGH();
}
/*!
 *  @brief        Write a value to the specified register   
 *  @param        addr:the address of the register
 *  @param        value:the value to be written  
 *  @return       None
 *  @note          
*/
void L01_WriteSingleReg(uint8_t addr,uint8_t value)
{
    L01_CSN_LOW();
    SPI_ExchangeByte(W_REGISTER | addr);
    SPI_ExchangeByte(value);
    L01_CSN_HIGH();
}
/*!
 *  @brief        Write buffer to the specified registers  
 *  @param        start_addr:the start address of the registers
 *  @param        buffer:the buffer to be written
 *  @param        size:the size to be written  
 *  @return       None
 *  @note          
*/
void L01_WriteMultiReg(uint8_t start_addr,uint8_t *buffer,uint8_t size)
{
    uint8_t i;
    L01_CSN_LOW();
    SPI_ExchangeByte(W_REGISTER | start_addr);
    for ( i = 0; i < size; i++)
    {
        SPI_ExchangeByte(*(buffer + i));
    }
    L01_CSN_HIGH();
}
/*!
 *  @brief        Set the nRF24L01 into PowerDown mode          
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_SetPowerDown(void)
{
    uint8_t controlreg = L01_ReadSingleReg(L01REG_CONFIG);
    L01_WriteSingleReg(L01REG_CONFIG,controlreg & (~(1 << PWR_UP)));
}
/*!
 *  @brief        Set the nRF24L01 into PowerUp mode       
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_SetPowerUp(void)
{
    uint8_t controlreg = L01_ReadSingleReg(L01REG_CONFIG);
    L01_WriteSingleReg(L01REG_CONFIG,controlreg | (1 << PWR_UP));
}
/*!
 *  @brief        Flush the TX buffer             
 *  @param        None 
 *  @return       None  
 *  @note          
*/
void L01_FlushTX(void)
{
    L01_CSN_LOW();
    SPI_ExchangeByte(FLUSH_TX);
    L01_CSN_HIGH();
}
/*!
 *  @brief        Flush the RX buffer           
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_FlushRX(void)
{
    L01_CSN_LOW();
    SPI_ExchangeByte(FLUSH_RX);
    L01_CSN_HIGH();
}
/*!
 *  @brief        Reuse the last transmitted payload           
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_ReuseTXPayload(void)
{
    L01_CSN_LOW();
    SPI_ExchangeByte(REUSE_TX_PL);
    L01_CSN_HIGH();
}
/*!
 *  @brief        NOP operation about the nRF24L01+           
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_Nop(void)
{
    L01_CSN_LOW();
    SPI_ExchangeByte(NOP);
    L01_CSN_HIGH();
}
/*!
 *  @brief        Read the status register of the nRF24L01+           
 *  @param        None
 *  @return       the value of the status register  
 *  @note          
*/
uint8_t L01_ReadStatusReg(void)
{
    uint8_t status;
    L01_CSN_LOW();
    status = SPI_ExchangeByte(R_REGISTER + L01REG_STATUS);
    L01_CSN_HIGH();
    return status;
}
/*!
 *  @brief        Clear the IRQ caused by the nRF24L01+           
 *  @param        irqMask:RX_DR(bit[6]),TX_DS(bit[5]),MAX_RT(bit[4])
 *  @return       None  
 *  @note          
*/
void L01_ClearIRQ(uint8_t irqMask)
{
    uint8_t status = 0;
    irqMask &= IRQ_ALL;
    status = L01_ReadStatusReg();
    L01_CSN_LOW();
    L01_WriteSingleReg(L01REG_STATUS,irqMask | status);
    L01_CSN_HIGH();
    L01_ReadStatusReg();
}
/*!
 *  @brief        Read the IRQ status of the nRF24L01+           
 *  @param        None
 *  @return       irqMask:RX_DR(bit[6]),TX_DS(bit[5]),MAX_RT(bit[4]) 
 *  @note          
*/
uint8_t L01_ReadIRQSource(void)
{
    uint8_t status = 0;
    status = L01_ReadStatusReg();
    return (status & IRQ_ALL);
}
/*!
 *  @brief        Read the payload width of the top buffer of the FIFO           
 *  @param        None
 *  @return       width:the width of the pipe buffer  
 *  @note          
*/
uint8_t L01_ReadTopFIFOWidth(void)
{
    uint8_t width;
    L01_CSN_LOW();
    SPI_ExchangeByte(R_RX_PL_WID);
    width = SPI_ExchangeByte(NOP);
    L01_CSN_HIGH();
    return width;
}
/*!
 *  @brief        Read the RX payload from the FIFO and store them in buffer            
 *  @param        buffer:the buffer to store the data
 *  @return       the length to be read
 *  @note          
*/
uint8_t L01_ReadRXPayload(uint8_t *buffer)
{
    uint8_t width,i;
    width = L01_ReadTopFIFOWidth();
    if(width > 32)
    {
        L01_CSN_HIGH();
        L01_FlushRX();
        return 0;
    }
    L01_CSN_LOW();
    SPI_ExchangeByte(R_RX_PAYLOAD);
    for (i = 0; i < width; i++)
    {
        *(buffer + i) = SPI_ExchangeByte(NOP);
    }
    L01_CSN_HIGH();
    L01_FlushRX();
    return width;
}
/*!
 *  @brief        Write TX Payload to a data pipe,and PRX will return ACK back         
 *  @param        buffer:the buffer stores the data
 *  @param        size:the size to be written  
 *  @return       None  
 *  @note          
*/
void L01_WriteTXPayload_Ack(uint8_t *buffer,uint8_t size)
{
    uint8_t i;
    uint8_t w_size = (size > 32) ? 32 : size;
    L01_FlushTX();
    L01_CSN_LOW();
    SPI_ExchangeByte(W_TX_PAYLOAD);
    for (i = 0; i < w_size; i++)
    {
        SPI_ExchangeByte(*(buffer + i));
    }
    L01_CSN_HIGH();
}
/*!
 *  @brief        Write TX Payload to a data pipe,and PRX won't return ACK back         
 *  @param        buffer:the buffer stores the data
 *  @param        size:the size to be written  
 *  @return       None  
 *  @note          
*/
void L01_WriteTXPayload_NoAck(uint8_t *buffer,uint8_t size)
{
    if (size > 32 || size == 0)
    {
        return;
    }
    L01_CSN_LOW();
    SPI_ExchangeByte(W_TX_PAYLOAD_NOACK);
    while (size --)
    {
        SPI_ExchangeByte(*buffer++);
    }
    L01_CSN_HIGH();
}
/*!
 *  @brief        Write TX Payload to a data pipe when RX mode         
 *  @param        buffer:the buffer stores the data
 *  @param        size:the size to be written  
 *  @return       None  
 *  @note          
*/
void L01_WriteRXPayload_InAck(uint8_t *buffer,uint8_t size)
{
    uint8_t i;
    uint8_t w_size = (size > 32) ? 32 : size;
    L01_CSN_LOW();
    SPI_ExchangeByte(W_ACK_PAYLOAD);
    for (i = 0; i < w_size; i++)
    {
        SPI_ExchangeByte(*(buffer + i));
    }
    L01_CSN_HIGH();
}
/*!
 *  @brief        Write Transmit address into TX_ADDR register          
 *  @param        addrBuffer:the buffer stores the address
 *  @param        addr_size:the address byte num
 *  @return       None  
 *  @note         Used for a PTX device only 
*/
void L01_SetTXAddr(uint8_t *addrBuffer,uint8_t addr_size)
{
    uint8_t size = (addr_size > 5) ? 5 : addr_size;
    L01_WriteMultiReg(L01REG_TX_ADDR,addrBuffer,size);
}
/*!
 *  @brief        Write address for the RX pipe 
 *  @param        pipeNum:the number of the data pipe         
 *  @param        addrBuffer:the buffer stores the address
 *  @param        addr_size:the address byte num
 *  @return       None  
 *  @note          
*/
void L01_SetRXAddr(uint8_t pipeNum,uint8_t *addrBuffer,uint8_t addr_size)
{
    uint8_t size = (addr_size > 5) ? 5 : addr_size;
    uint8_t num = (pipeNum > 5) ? 5 : pipeNum;
    L01_WriteMultiReg(L01REG_RX_ADDR_P0 + num,addrBuffer,size);
}
/*!
 *  @brief        Set the data rate of the nRF24L01+          
 *  @param        drate:250K,1M,2M
 *  @return       None  
 *  @note          
*/
void L01_SetDataRate(L01_DRATE drate)
{
    uint8_t mask = L01_ReadSingleReg(L01REG_RF_SETUP);
    mask &= ~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH));
    if(drate == DRATE_250K)
    {
        mask |= (1 << RF_DR_LOW);
    }
    else if(drate == DRATE_1M)
    {
        mask |= ~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH));
    }
    else if(drate == DRATE_2M)
    {
        mask |= (1 << RF_DR_HIGH);
    }
    L01_WriteSingleReg(L01REG_RF_SETUP,mask);
}
/*!
 *  @brief        Set the power of the nRF24L01+          
 *  @param        power:18dB,12dB,6dB,0dB
 *  @return       None  
 *  @note          
*/
void L01_SetPower(L01_PWR power)
{
    uint8_t mask = L01_ReadSingleReg(L01REG_RF_SETUP) & ~0x07;
    switch (power)
    {
    case POWER_N_18:
        mask |= PWR_N_18DB;
        break;
    case POWER_N_12:
        mask |= PWR_N_12DB;
        break;
    case POWER_N_6:
        mask |= PWR_N_6DB;
        break;
    case POWER_N_0:
        mask |= PWR_N_0DB;
        break;
    default:
        break;
    }
    L01_WriteSingleReg(L01REG_RF_SETUP,mask);
}
/*!
 *  @brief        Set the frequency of the nRF24L01+          
 *  @param        freq:the hopping frequency point,range:0-125,2400Mhz-2525Mhz
 *  @return       None  
 *  @note          
*/
void L01_WriteHoppingPoint(uint8_t freq)
{
    L01_WriteSingleReg(L01REG_RF_CH,freq <= 125 ? freq : 125);
}
/*!
 *  @brief        Set the nRF24L01+ as TX/RX mode         
 *  @param        mode:TX/RX
 *  @return       None  
 *  @note          
*/
void L01_SetTRMode(L01_MODE mode)
{
    uint8_t mask = L01_ReadSingleReg(L01REG_CONFIG);
    if (mode == TX_MODE)
    {
        mask &= ~(1 << PRIM_RX);
    }
    else if (mode == RX_MODE)
    {
        mask |= (1 << PRIM_RX);
    }
    L01_WriteSingleReg(L01REG_CONFIG,mask);
}

/*!
 *  @brief        Exchange a byte via the SPI bus
 *  @note
*/
uint8_t BSP_SPI_ExchangeByte(uint8_t w_data)
{
   uint8_t rdata, wdata;
		
	wdata = w_data;
	rdata = spi_rw_mode0(w_data);
//	HAL_SPI_Transmit(&hspi3, &wdata, 1, 100);
//	HAL_SPI_Receive(&hspi3, &rdata, 1, 100);
	
  return rdata;
}

/*!
 *  @brief        Initialize the nRF24L01+         
 *  @param        None
 *  @return       None  
 *  @note          
*/
void L01_Init(void)
{
    uint8_t addr[5] = {INIT_ADDR};
    uint8_t status = L01_GetCEStatus();
    L01_SetCE(CE_LOW);
    L01_SetPowerDown();
    L01_ClearIRQ(IRQ_ALL);
#if DYNAMIC_PACKET == 1
    //dynamic payload length
    L01_WriteSingleReg(L01REG_DYNPD,(1 << DPL_P0));//Enable pipe 0 dynamic payload length
    L01_WriteSingleReg(L01REG_FEATURE,(1 << EN_DPL)|(1 << EN_DYN_ACK));
#elif DYNAMIC_PACKET == 0
    //fixed payload length
    L01_WriteSingleReg(L01REG_RX_PW_P0,FIXED_PACKET_LEN);
#endif
    L01_WriteSingleReg(L01REG_CONFIG,(1 << EN_CRC));//Enable CRC,2 bytes
    L01_WriteSingleReg(L01REG_ENAA,(1 << ENAA_P0));//Auto Ack in pipe0
    L01_WriteSingleReg(L01REG_EN_RXADDR,(1 << ERX_P0));//Enable RX pipe 0
    L01_WriteSingleReg(L01REG_SETUP_AW,AW_5BYTES);//Address width:5bytes
    L01_WriteSingleReg(L01REG_SETUP_RETR,ARD_4000US|ARC_15);//ARD:4000us,repeat time:15
    L01_WriteSingleReg(L01REG_RF_SETUP,(1 << RF_DR_LOW) | PWR_N_0DB);//Drate:250Kbps.Power:0dB
    L01_SetDataRate(DRATE_2M);
    L01_SetTXAddr(addr,5);//Set TX address
    L01_SetRXAddr(0,addr,5);//Set RX address
    L01_SetTRMode(RX_MODE);
    L01_WriteHoppingPoint(2);
    L01_SetPowerUp();
}

/*!
 *  @brief        Switch the current RF mode to RX
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_SwitchToRx(void)
{
    L01_SetCE(CE_LOW);
    L01_SetPowerUp();
    L01_SetTRMode(RX_MODE);
    L01_SetDataRate(DRATE_2M);
    L01_WriteHoppingPoint(2);
    L01_FlushRX();
    L01_FlushTX();
    L01_ClearIRQ(IRQ_ALL);
    L01_SetCE(CE_HIGH);
}

/*!
 *  @brief        Switch the current RF mode to TX
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_SwitchToTx(void)
{
    L01_SetCE(CE_LOW);
    L01_SetPowerUp();
    L01_SetTRMode(TX_MODE);
    L01_SetDataRate(DRATE_2M);
    L01_WriteHoppingPoint(2);
    L01_FlushRX();
    L01_FlushTX();
    L01_ClearIRQ(IRQ_ALL);
}

static uint8_t RF_SendBuffer[32] = {0};
/*!
 *  @brief        Transmit handler for transmit mode
 *  @param        None
 *  @return       None
 *  @note
*/
void APP_TransmitHandler(void)
{
	uint8_t len,IRQ_flag,ack_rx[32]={0};
	
//	if((len = Uart_RecieveLength()) != 0)
//	{
//			LED0_ON();
//			BSP_USART_ReadBuffer(UART_RX, RF_SendBuffer, len);       
//			APP_SwitchToTx();
			L01_SetCE(CE_LOW);
			L01_WriteTXPayload_Ack(RF_SendBuffer, len);
			L01_SetCE(CE_HIGH);
			while(GET_L01_IRQ());
			printf("ok\n");
			IRQ_flag=L01_ReadIRQSource();//????§Ø???¦Ë
			if(IRQ_flag&0x20)//?§Ø??§Ø?????????????§Ø?
			{
				len=L01_ReadRXPayload(ack_rx);//???????????????
//				BSP_USART_WriteBuffer(UART_TX, ack_rx, len);
//				USART_ITConfig(USART_IT_TXE, ENABLE);
			}  
			if(IRQ_flag&0x10)//?§Ø????????????§Ø?
			{
				ack_rx[0]=0xFF;
				len=1;
//				BSP_USART_WriteBuffer(UART_TX, ack_rx, len);
//				USART_ITConfig(USART_IT_TXE, ENABLE);
			}
			L01_FlushTX();
			L01_FlushRX( );
			L01_ClearIRQ(IRQ_ALL);
//			LED0_OFF();
//	}
}
