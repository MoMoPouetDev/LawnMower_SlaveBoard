/**
 * @file LLD_I2C.h
 * @author MVE
 * @brief Header file for I2C peripheral
 * @details
**/

#ifndef LLD_I2C_H_
#define LLD_I2C_H_
/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
#define SCL_CLOCK  400000UL
/*** Adresses I2C ***/
#define ADDR_SLAVE_SENSOR 0x20

typedef void (*lld_i2c_callback_t)(uint8_t u8_receivedData, uint8_t* pu8_sendData);
/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DECLARATIONS ...                                   */
/*--------------------------------------------------------------------------*/
void LLD_I2C_Init(lld_i2c_callback_t pf_callback);
uint8_t LLD_I2C_IsInterruptRunning(void);
uint8_t LLD_I2C_Read(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t *pu8_receivedData);
uint8_t LLD_I2C_Write(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t u8_data);
/*--------------------------------------------------------------------------*/
/* ... END OF FILE...                                                      */
/*--------------------------------------------------------------------------*/
#endif /* LLD_I2C_H_ */
