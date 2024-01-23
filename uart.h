/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Tutorial (C).            *
 ******************************************************************************/

/**
 * @file uart.h
 * @author Koryciak
 * @date Sep 2020
 * @brief File containing enums, structures and declarations.
 * @ver 0.1
 */
 
#include "frdm_bsp.h"

#ifndef UART_H
#define UART_H

#define CLK_DIS 					0x00
#define MCGFLLCLK 				0x01
#define OSCERCLK					0x02
#define MCGIRCLK					0x03

/**
 * @brief UART initialization
 *
 * @param Baud rate (e.g. 9600)
 */
void UART_Init(uint32_t baud_rate);
/**
 * @brief Print to serial port
 *
 * @param String to display.
 */
void UART_Print(const char *str);
/**
 * @brief Print to serial port and go to the next line
 *
 * @param String to display.
 */
void UART_Println(const char *str);

#endif /* UART_H */
