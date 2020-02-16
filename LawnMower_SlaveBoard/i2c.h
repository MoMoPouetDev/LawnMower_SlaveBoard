//
//  i2c.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef i2c_h
#define i2c_h

void I2C_start(void);
void I2C_write(uint8_t, uint8_t);
uint8_t I2C_readACK(void);
uint8_t I2C_readNACK(void);
void I2C_stop(void);

#endif /* i2c_h */
