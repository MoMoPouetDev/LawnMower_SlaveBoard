/*
 * HAL_FIFO.h
 *
 *  Created on: 20 FEB 2023
 *      Author: morgan.venandy
 */

 #ifndef HAL_HAL_FIFO_HAL_FIFO_H_
 #define HAL_HAL_FIFO_HAL_FIFO_H_
 
 /*--------------------------------------------------------------------------*/
 /*! ... INCLUDES ...                                                        */
 /*--------------------------------------------------------------------------*/
 #include "stdint.h"
 
 /*--------------------------------------------------------------------------*/
 /* ... DATATYPES ...                                                        */
 /*--------------------------------------------------------------------------*/
 typedef struct Element Element;
 struct Element
 {
     int value;
     Element *nextElement;
 };
 
 typedef struct
 {
     Element *firstElement;
 }Fifo;
 
 /*--------------------------------------------------------------------------*/
 /*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
 /*--------------------------------------------------------------------------*/
void RUN_FIFO_Init(void);
uint8_t RUN_FIFO_GetSonarAverageFC(uint8_t u8_distFC);
uint8_t RUN_FIFO_GetSonarAverageFL(uint8_t u8_distFL);
uint8_t RUN_FIFO_GetSonarAverageFR(uint8_t u8_distFR);

#endif /* HAL_HAL_FIFO_HAL_FIFO_H_ */
 