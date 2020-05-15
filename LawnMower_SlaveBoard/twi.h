//
//  twi.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 21/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef twi_h
#define twi_h

void TWI_decodeReceivedData(uint8_t);
void TWI_start(void);
void TWI_write(uint8_t, uint8_t);
uint8_t TWI_readACK(void);
uint8_t TWI_readNACK(void);
void TWI_stop(void);

#endif /* twi_h */
