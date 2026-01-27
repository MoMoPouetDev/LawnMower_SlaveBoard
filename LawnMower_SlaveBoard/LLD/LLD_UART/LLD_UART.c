/**
 * @file LLD_UART.c
 * @author MVE
 * @brief Specific UART driver
 * @details
**/

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "LLD_UART.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES LLD UART ...                                               */
/*--------------------------------------------------------------------------*/
#define F_CPU 20000000UL
#define BAUD 19200UL
#define BAUD_PRESCALE ((F_CPU/ (16UL*BAUD))-1)

volatile static uint8_t gu8_flagUart;
volatile static char gu8_uartBuffer[UART_BUFFER_SIZE] = {0};
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _LLD_UART_ReceiveChar(void);
/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DEFINITIONS ...                                    */
/*--------------------------------------------------------------------------*/
ISR(USART_RX_vect)
{
    _LLD_UART_ReceiveChar();
}
/**
* @brief		UART initialization
* @return		void
* @details
**/
void LLD_UART_Init(void)
{
    /***** UART BaudRate *****/
    UBRR0H = (unsigned char) ((BAUD_PRESCALE)>>8);
    UBRR0L = (unsigned char) BAUD_PRESCALE;
    
    /***** Autoriser Reception *****/
    UCSR0B |= (1<<RXEN0) | (1<<RXCIE0);
    /***** Format : 8 bits, 1 stop bit *****/
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);

    gu8_flagUart = 0;
}

static void _LLD_UART_ReceiveChar(void)
{
    static uint8_t _u8_flagReceive = 0;
    static uint8_t _u8_index = 0;
    
    while (!(UCSR0A & (1<<RXC0)));
    
    if (UDR0 == '$')
    {
        _u8_index = 0;
        gu8_uartBuffer[_u8_index] = UDR0;
        _u8_index++;
        _u8_flagReceive = 1;
        gu8_flagUart = 0;
    }
    else if (_u8_flagReceive == 1)
    {
        gu8_uartBuffer[_u8_index] = UDR0;
    
        if (UDR0 == 0x0D)
        {
            _u8_index = 0;
            _u8_flagReceive = 0;
            gu8_flagUart = 1;
        }
		else if (_u8_index >= UART_BUFFER_SIZE - 1)
		{
			_u8_index = 0;
			_u8_flagReceive = 0;
		}
        else
        {
            _u8_index++;
        }
    }
}

/**
 * @brief Getter for uart buffer
 * 
 * @return uint8_t 
 */
uint8_t LLD_UART_GetBuffer(char* pc_RxBuff)
{
    uint8_t u8_i = 0;

    for (u8_i = 0; u8_i < UART_BUFFER_SIZE; u8_i++)
    {
        pc_RxBuff[u8_i] = gu8_uartBuffer[u8_i];
    }

    return gu8_flagUart;
}

/**
* @brief		Receive data
* @param		u8_RxBuff : pointer on rx buffer
* @return		uint8_t
* @details
**/
uint8_t LLD_UART_Receive(uint8_t* pu8_rxBuff)
{
	static uint8_t _u8_uartState = 0;
    uint8_t u8_uartStatus = 0;

    switch (_u8_uartState)
    {
        case 0:
            if ((UCSR0A & (1<<RXC0)) != 0)
            {
                _u8_uartState++;
            }
            break;
        
        case 1:
            (*pu8_rxBuff) = UDR0;
            _u8_uartState = 0;
            u8_uartStatus = 1;
            break;
        
        default:
            _u8_uartState = 0;
            break;
    }

    return u8_uartStatus;
}

/**
* @brief		Send data
* @param		u8_TxBuff : pointer on tx buffer
* @return		uint8_t
* @details
**/
uint8_t LLD_UART_Send(uint8_t* pu8_txBuffer)
{
	static uint8_t _u8_uartState = 0;
    uint8_t u8_uartStatus = 0;

    switch (_u8_uartState)
    {
        case 0:
            if ((UCSR0A & (1<<UDRE0)) != 0)
            {
                _u8_uartState++;
            }
            break;
        
        case 1:
            UDR0 = (*pu8_txBuffer);
            _u8_uartState = 0;
            u8_uartStatus = 1;
            break;
        
        default:
            _u8_uartState = 0;
            break;
    }

    return u8_uartStatus;
}
