#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <stdint.h>
#include "usart.h"

//extern UART_HandleTypeDef g_uart_handle;

/* ���Ŷ��� */
#define ESP8266_UART_TX_GPIO_PORT           GPIOA
#define ESP8266_UART_TX_GPIO_PIN            GPIO_PIN_2
#define ESP8266_UART_TX_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PC��ʱ��ʹ�� */

#define ESP8266_UART_RX_GPIO_PORT           GPIOA
#define ESP8266_UART_RX_GPIO_PIN            GPIO_PIN_3
#define ESP8266_UART_RX_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PD��ʱ��ʹ�� */

#define ESP8266_UART_INTERFACE              USART2
#define ESP8266_UART_IRQn                   USART2_IRQn
#define ESP8266_UART_IRQHandler             USART2_IRQHandler
#define ESP8266_UART_CLK_ENABLE()           do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0) /* UART2ʱ��ʹ�� */

/* UART�շ������С */
#define ESP8266_UART_RX_BUF_SIZE            128
#define ESP8266_UART_TX_BUF_SIZE            64

/* ������� */
#define ESP8266_EOK                         0   /* û�д��� */
#define ESP8266_ERROR                       1   /* ͨ�ô��� */
#define ESP8266_ETIMEOUT                    2   /* ��ʱ���� */
#define ESP8266_EINVAL                      3   /* �������� */

/* ����ģʽ */
#define ESP8266_STA_MODE                    1
#define ESP8266_AP_MODE                     2
#define ESP8266_STA_AP_MODE                 3

#define WIFI_SSID                           "TP-LINK_3E30"
#define WIFI_PWD                            "18650711783"

#define TCP_SERVER_IP                       "192.168.0.150"
#define TCP_SERVER_PORT                     "8080"

//uint8_t esp8266_init(uint32_t baudrate);
void esp8266_clear(void);
void esp8266_uart_printf(char *fmt, ...);
#endif