/**@file  	    nRF24L01P_REG.h
* @brief            nRF24L01+ register descriptions and configurations.
* @author           hyh
* @date             2021.9.17
* @version          1.0
* @copyright        Chengdu Ebyte Electronic Technology Co.Ltd
**********************************************************************************
*/
#ifndef nRF24L01P_REG_H
#define nRF24L01P_REG_H

/*SPI commands for the nRF24L01+*/
#define R_REGISTER                  0x00
#define W_REGISTER                  0x20
#define R_RX_PAYLOAD                0x61
#define W_TX_PAYLOAD                0xA0
#define FLUSH_TX                    0xE1
#define FLUSH_RX                    0xE2
#define REUSE_TX_PL                 0xE3
#define R_RX_PL_WID                 0x60
#define W_ACK_PAYLOAD               0xA8
#define W_TX_PAYLOAD_NOACK          0xB0
#define NOP                         0xFF
/*
================================================================================
-------------------------------Register defination------------------------------
================================================================================
*/
/*Internal register address map and its bits definations*/
#define L01REG_CONFIG               0x00 //configuration Register
    //bit[7]:Reserved               Only '0' allowed
    #define MASK_RX_DR              6 // RW, Mask interrupt caused by RX_DR
    #define MASK_TX_DS              5 // RW, Mask interrupt caused by TX_DS
    #define MASK_MAX_PT             4 // RW, Mask interrupt caused by MAX_RT
    #define EN_CRC                  3 // RW, Enable CRC. Forced high if one of the bits in the EN_AA is high
    #define CRCO                    2 // RW, CRC encoding scheme: 0:1 byte; 1:2 bytes
    #define PWR_UP                  1 // RW, 1:POWER UP;0:POWER DOWN
    #define PRIM_RX                 0 // RW, RX/TX control: 1:PRX; 0:PTX
#define L01REG_ENAA                 0x01 //Enable Auto Acknowledgment function
    //bit[7:6]:Reserved             Only '00' allowed
    #define ENAA_P5                 5 // RW, Enable auto acknowledgement data pipe 5
    #define ENAA_P4                 4 // RW, Enable auto acknowledgement data pipe 4
    #define ENAA_P3                 3 // RW, Enable auto acknowledgement data pipe 3
    #define ENAA_P2                 2 // RW, Enable auto acknowledgement data pipe 2
    #define ENAA_P1                 1 // RW, Enable auto acknowledgement data pipe 1
    #define ENAA_P0                 0 // RW, Enable auto acknowledgement data pipe 0
#define L01REG_EN_RXADDR            0x02 //Enabled RX Addresses
    //bit[7:6]:Reserved             Only '00' allowed
    #define ERX_P5                  5 // RW, Enable data pipe 5
    #define ERX_P4                  4 // RW, Enable data pipe 4
    #define ERX_P3                  3 // RW, Enable data pipe 3
    #define ERX_P2                  2 // RW, Enable data pipe 2
    #define ERX_P1                  1 // RW, Enable data pipe 1
    #define ERX_P0                  0 // RW, Enable data pipe 0
#define L01REG_SETUP_AW             0X03 //Setup of Address Widths (common for all data pipes)
    //bit[7:2]:Reserved             Only '000000' allowed
    //bit[1:0]:AW                   RX/TX Address field width 
    #define AW_ILLEGAL              0x00 // illegal
    #define AW_3BYTES               0x01 // 3 bytes
    #define AW_4BYTES               0x02 // 4 bytes
    #define AW_5BYTES               0x03 // 5 bytes
#define L01REG_SETUP_RETR           0x04 //Setup of Automatic Retransmission
    //bit[7:4]:ARD                  Auto Retransmit Delay
    #define ARD_250US               (0x00<<4)
    #define ARD_500US               (0x01<<4)
    #define ARD_750US               (0x02<<4)
    #define ARD_1000US              (0x03<<4)
    #define ARD_1250US              (0x04<<4)
    #define ARD_1500US              (0x05<<4)
    #define ARD_1750US              (0x06<<4)
    #define ARD_2000US              (0x07<<4)
    //......
    #define ARD_4000US              (0x0F<<4)
    //bit[3:0]:ARC                  Auto retransmission Count
    #define ARC_DISABLED            0x00
    #define ARC_1                   0x01
    #define ARC_5                   0x05

    //......
    #define ARC_15                  0x0F
#define L01REG_RF_CH                0x05 //RF Channel
    //bit[7]:Reserved               Only '0' allowed
    //bit[6:0]:RF_CH:               Sets the frequency channel nRF24L01+ operates on.Range:[0-126] 2400Mhz-2525Mhz
#define L01REG_RF_SETUP             0x06 //RF Setup Register
    //nRF24L01+:bit[6]:Reserved
    #define CONT_WAVE               7 // RW,Enables continuous carrier transmit when high.
    #define RF_DR_LOW               5 // RW,Set data rate to 250Kbps,
    #define PLL_LOCK                4 // RW,Force PLL lock signal. Only used in test
    #define RF_DR_HIGH              3 // RW,Air Data Rate: [RF_DR_LOW,RF_DR_HIGH]->00:1Mbps; 01:2Mbps; 10:250Kbps; 11:Reserved
    //bit[2:1]:RF_PWR               Set RF output power in TX mode
    #define PWR_N_18DB                (0x00<<1)
    #define PWR_N_12DB                (0x01<<1)
    #define PWR_N_6DB                 (0x02<<1)
    #define PWR_N_0DB                 (0x03<<1)
    #define OBSOLETE                0 // Don't care
#define L01REG_STATUS               0x07 //Status Register
    //bit[7]:Reserved               Only '0' allowed
    #define RX_DR                   6 // RW,Data Ready RX FIFO interrupt,Write 1 to clear bit
    #define TX_DS                   5 // RW,Data Sent TX FIFO interrupt,Write 1 to clear bit
    #define MAX_RT                  4 // RW,Maximum number of TX retransmits interrupt,Write 1 to clear bit
    //bit[3:1]:RX_P_NO:             R,Data pipe number for the payload available for reading from RX_FIFO
    //000-101:Data Pipe Number      110:Not Used;111: RX FIFO Empty
    #define TX_FULL_0               0 // R,TX FIFO full flag. 1:FULL; 0:Available locations in TX FIFO
#define L01REG_OBSERVE_TX           0x08 // Transmit observe register
    //bit[7:4]:PLOS_CNT:            R,Count lost packets.
    //bit[3:0]:ARC_CNT:             R,Count retransmitted packets.
#define L01REG_RPD                  0x09 // Received Power Detector
    //bit[7:1]:Reserved             Only '0000000' allowed
    //bit[0]:RPD                    R,Received Power Detector    
#define L01REG_RX_ADDR_P0           0x0A    
    //bit[39:0]                     Receive address data pipe 0.5 Bytes maximum length.LSByte is written first
#define L01REG_RX_ADDR_P1           0x0B   
    //bit[39:0]                     Receive address data pipe 1.5 Bytes maximum length.LSByte is written first
#define L01REG_RX_ADDR_P2           0x0C   
    //bit[7:0]                      Receive address data pipe 2. Only LSByte.MSBytes is equal to RX_ADDR_P1[39:8]
#define L01REG_RX_ADDR_P3           0x0D    
    //bit[7:0]                      Receive address data pipe 3. Only LSByte.MSBytes is equal to RX_ADDR_P1[39:8]
#define L01REG_RX_ADDR_P4           0x0E    
    //bit[7:0]                      Receive address data pipe 4. Only LSByte.MSBytes is equal to RX_ADDR_P1[39:8]
#define L01REG_RX_ADDR_P5           0x0F   
    //bit[7:0]                      Receive address data pipe 5. Only LSByte.MSBytes is equal to RX_ADDR_P1[39:8] 
#define L01REG_TX_ADDR              0x10
    //bit[39:0]Transmit address. Used for a PTX device only.LSByte is written first.
    //Set RX_ADDR_P0 equal to this address to han-dle automatic acknowledge 
    //if this is a PTX device with Enhanced ShockBurst™ enabled.
#define L01REG_RX_PW_P0             0x11
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P0             Number of bytes in RX payload in data pipe 0 (0 to 32 bytes).
#define L01REG_RX_PW_P1             0x12
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P1             Number of bytes in RX payload in data pipe 1 (0 to 32 bytes).
#define L01REG_RX_PW_P2             0x13
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P2             Number of bytes in RX payload in data pipe 2 (0 to 32 bytes).
#define L01REG_RX_PW_P3             0x14
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P3             Number of bytes in RX payload in data pipe 3 (0 to 32 bytes).
#define L01REG_RX_PW_P4             0x15
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P4             Number of bytes in RX payload in data pipe 4 (0 to 32 bytes).
#define L01REG_RX_PW_P5             0x16
    //bit[7:6]:Reserved             Only '00' allowed
    //bit[5:0]:RX_PW_P5             Number of bytes in RX payload in data pipe 5 (0 to 32 bytes).
#define L01REG_FIFO_STATUS          0x17 // FIFO Status Register
    //bit[7]:Reserved               Only '0' allowed
    #define TX_REUSE                6 // R,Reuse last transmitted data packet if set high.
    #define TX_FULL_1               5 // R,TX FIFO full flag. 1: TX FIFO full. 0: Available locations in TX FIFO.
    #define TX_EMPTY                4 // R,TX FIFO empty flag.1: TX FIFO empty. 0: Data in TX FIFO.
    //bit[3:2]:Reserved             Only '00' allowed
    #define RX_FULL                 1 // R,RX FIFO full flag. 1: RX FIFO full. 0: Available locations in RX FIFO.
    #define RX_EMPTY                0 // R,RX FIFO empty flag. 1: RX FIFO empty. 0: Data in RX FIFO.
#define L01REG_DYNPD                0x1C // Enable dynamic payload length
    //bit[7:6]:Reserved             Only '00' allowed
    #define DPL_P5                  5 // RW,Enable dyn. payload length  data pipe 5. (Requires EN_DPL and ENAA_P5)
    #define DPL_P4                  4 // RW,Enable dyn. payload length  data pipe 4. (Requires EN_DPL and ENAA_P4)
    #define DPL_P3                  3 // RW,Enable dyn. payload length  data pipe 3. (Requires EN_DPL and ENAA_P3)
    #define DPL_P2                  2 // RW,Enable dyn. payload length  data pipe 2. (Requires EN_DPL and ENAA_P2)
    #define DPL_P1                  1 // RW,Enable dyn. payload length  data pipe 1. (Requires EN_DPL and ENAA_P1)
    #define DPL_P0                  0 // RW,Enable dyn. payload length  data pipe 0. (Requires EN_DPL and ENAA_P0)
#define L01REG_FEATURE              0x1D // Feature Register
    //bit[7:3]:Reserved             Only '00000' allowed
    #define EN_DPL                  2 // RW,Enables Dynamic Payload Length
    #define EN_ACK_PAY              1 // RW,Enables Payload with ACK
    #define EN_DYN_ACK              0 // RW,Enables the W_TX_PAYLOAD_NOACK command

#endif
