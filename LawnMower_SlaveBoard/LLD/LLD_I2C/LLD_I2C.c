/**
 * @file LLD_I2C.c
 * @author ACR
 * @brief Specific I2C driver
 * @details
**/

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "LLD_I2C.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES LLD I2C ...                                                */
/*--------------------------------------------------------------------------*/
static lld_i2c_callback_t gpf_callback;
static volatile uint8_t gu8_flagInterrupt;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static uint8_t _LLD_I2C_Start(void);
static uint8_t _LLD_I2C_RepeatStart(void);
static uint8_t _LLD_I2C_WriteCmd(uint8_t u8_slaveAddr, uint8_t u8_readWriteByte);
static uint8_t _LLD_I2C_WriteByte(uint8_t u8_byte);
static uint8_t _LLD_I2C_ReadACK(uint8_t* pu8_dataBuffer);
static uint8_t _LLD_I2C_ReadNACK(uint8_t* pu8_dataBuffer);
static uint8_t _LLD_I2C_Stop(void);
/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DEFINITIONS ...                                    */
/*--------------------------------------------------------------------------*/
ISR(TWI_vect)
{
    static uint8_t _u8_sendData;
    uint8_t u8_receivedData = 0;
    
    switch (TW_STATUS) {
        case TW_SR_SLA_ACK:
            gu8_flagInterrupt = 1;
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) | (1<<TWIE);
            break;
            
        case TW_SR_DATA_ACK:
            u8_receivedData = TWDR;
            gpf_callback(u8_receivedData, &_u8_sendData);
            
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA)| (1<<TWIE);
            break;
			
		case TW_SR_STOP:
			TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWEN) | (1<<TWINT);
			break;
            
        case TW_ST_SLA_ACK:
            TWDR = _u8_sendData;
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) | (1<<TWIE);
            break;
            
        case TW_ST_DATA_NACK:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWIE) | (1<<TWEA);
            gu8_flagInterrupt = 0;
            break;
            
        case TW_BUS_ERROR:
            TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE)| (1<<TWSTO) | (1<<TWINT);
            gu8_flagInterrupt = 0;
            break;
            
        default:
            TWCR = (1<<TWINT) | (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
            gu8_flagInterrupt = 0;
            break;
    }
}
/**
* @brief		I2C initialization
* @return		void
* @details
**/
void LLD_I2C_Init(lld_i2c_callback_t pf_callback)
{
    TWAR = ADDR_SLAVE_SENSOR;
    TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWINT) | (1<<TWIE);

    gpf_callback = pf_callback;
    gu8_flagInterrupt = 0;
}

uint8_t LLD_I2C_IsInterruptRunning(void)
{
    return gu8_flagInterrupt;
}

/**
* @brief		Write data to slave
* @param		e_I2c : I2C number
* @param		u8_SlaveAddress : slave address
* @param		u8_DataAddress : data address
* @return		void
* @details
**/
uint8_t LLD_I2C_Write(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t u8_data)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cReturnValue = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            u8_i2cReturnValue = _LLD_I2C_Start();
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 1:
            u8_i2cReturnValue = _LLD_I2C_WriteCmd(u8_slaveAddr, TW_WRITE);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            u8_i2cReturnValue = _LLD_I2C_WriteByte(u8_dataAddr);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 3:
            u8_i2cReturnValue = _LLD_I2C_WriteByte(u8_data);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 4:
            u8_i2cReturnValue = _LLD_I2C_Stop();
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;

        default:
            break;
            /* code */
            break;
    }

    return u8_i2cStatus;
}

/**
* @brief		Read data from slave
* @param		e_I2c : I2C number
* @param		u8_SlaveAddress : slave address (7 bits)
* @param		u8_DataAddress : data address
* @return		uint8_t
* @details
**/
uint8_t LLD_I2C_Read(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t *pu8_receivedData)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cReturnValue = 0;
    uint8_t u8_i2cStatus = 0;
	
    switch (_u8_i2cState)
    {
        case 0:
            u8_i2cReturnValue = _LLD_I2C_Start();
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 1:
            u8_i2cReturnValue = _LLD_I2C_WriteCmd(u8_slaveAddr, TW_READ);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            u8_i2cReturnValue = _LLD_I2C_WriteByte(u8_dataAddr);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;
        
        case 3:
            u8_i2cReturnValue = _LLD_I2C_RepeatStart();
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 4:
            u8_i2cReturnValue =_LLD_I2C_WriteCmd(u8_slaveAddr, TW_READ);
           if (u8_i2cReturnValue != 0)
           {
               _u8_i2cState++;
           }
            break;

        case 5:
            u8_i2cReturnValue = _LLD_I2C_ReadNACK(pu8_receivedData);
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 6:
            u8_i2cReturnValue =_LLD_I2C_Stop();
            if (u8_i2cReturnValue != 0)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;
        
        default:
            break;
    }
        
    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_Start(void)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == TW_START)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_RepeatStart(void)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == TW_REP_START)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_WriteCmd(uint8_t u8_slaveAddr, uint8_t u8_readWriteByte)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_twiState = 0;
    uint8_t u8_i2cStatus = 0;

    TWDR = u8_slaveAddr + u8_readWriteByte;

    if (u8_readWriteByte == 0)
    {
        u8_twiState = TW_MT_SLA_ACK;
    }
    else
    {
        u8_twiState = TW_MR_SLA_ACK;
    }
    

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == u8_twiState)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_WriteByte(uint8_t u8_byte)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    TWDR = u8_byte;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == TW_MT_DATA_ACK)
            {
                _u8_i2cState = 0;
                u8_i2cStatus = 1;
            }
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_ReadACK(uint8_t* pu8_dataBuffer)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == TW_MR_DATA_ACK)
            {
                _u8_i2cState++;
            }
            break;
        
        case 3:
            (*pu8_dataBuffer) = TWDR;
            _u8_i2cState = 0;
            u8_i2cStatus = 1;
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_ReadNACK(uint8_t* pu8_dataBuffer)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWEN) | (1<<TWINT);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWINT)) != 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            if ((TWSR & 0xF8) == TW_MR_DATA_NACK)
            {
                _u8_i2cState++;
            }
            break;
        
        case 3:
            (*pu8_dataBuffer) = TWDR;
            _u8_i2cState = 0;
            u8_i2cStatus = 1;
            break;
        
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}

static uint8_t _LLD_I2C_Stop(void)
{
    static uint8_t _u8_i2cState = 0;
    uint8_t u8_i2cStatus = 0;

    switch (_u8_i2cState)
    {
        case 0:
            TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
            _u8_i2cState++;
            break;

        case 1:
            if ((TWCR & (1<<TWSTO)) == 0)
            {
                _u8_i2cState++;
            }
            break;

        case 2:
            _u8_i2cState = 0;
            u8_i2cStatus = 1;
            break;
            
        default:
            _u8_i2cState = 0;
            break;
    }

    return u8_i2cStatus;
}
