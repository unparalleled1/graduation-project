#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include <stdarg.h>
#define NUMBER "34848854007540710077"
#define NUMM "12345678"
//UART_HandleTypeDef g_uart_handle;
uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
uint8_t g_uart_tx_buf[ESP8266_UART_TX_BUF_SIZE];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;

int my_esp8266_init(void)
{
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CWJAP=\"%s\",\"%s\"\r\n", "unp", "lllllllk");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	printf("AT+ATKCLDSTA=\"%s\",\"%s\"\r\n", NUMBER, NUMM);
	return 1;
}
/**
 * @brief       ESP8266 UART��ʼ��
 * @param       baudrate: UARTͨѶ������
 * @retval      ��
 */
 
 /*
void esp8266_uart_init(uint32_t baudrate)
{
    g_uart_handle.Instance          = ESP8266_UART_INTERFACE;       /* ESP8266 UART 
    g_uart_handle.Init.BaudRate     = baudrate;                     /* ������ 
    g_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;           /* ����λ 
    g_uart_handle.Init.StopBits     = UART_STOPBITS_1;              /* ֹͣλ 
    g_uart_handle.Init.Parity       = UART_PARITY_NONE;             /* У��λ 
    g_uart_handle.Init.Mode         = UART_MODE_TX_RX;              /* �շ�ģʽ 
    g_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;          /* ��Ӳ������ 
    g_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;         /* ������ 
    HAL_UART_Init(&g_uart_handle);                                  /* ʹ��ESP8266 UART 
}
*/

/**
 * @brief       ESP8266 UART�жϻص�����
 * @param       ��
 * @retval      ��
 
void ESP8266_UART_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_RXNE) != RESET){
		if(esp8266_cnt >= sizeof(g_uart_rx_buf))
            esp8266_cnt = 0; //��ֹ���ڱ�ˢ��
        HAL_UART_Receive(&g_uart_handle, &receive_data, 1, 1000);//����2����1λ����
		g_uart_rx_buf[esp8266_cnt++] = receive_data;  
    }
    HAL_UART_IRQHandler(&g_uart_handle);
}
*/
/**
 * @brief       ESP8266 ѭ�����ü���Ƿ�������
 * @param       ��
 * @retval      ESP8266_EOK-�������		ESP8266_ERROR-���ճ�ʱδ���
 */
uint8_t esp8266_wait_receive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return ESP8266_ERROR;
		
	if(esp8266_cnt == esp8266_cntPre) {				//�����һ�ε�ֵ�������ͬ����˵���������
		esp8266_cnt = 0;							//��0���ռ���
		return ESP8266_EOK;							//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	return ESP8266_ERROR;							//���ؽ���δ��ɱ�־
}

/**
 * @brief       ESP8266 ��մ��ڽ��ջ���
 * @param       ��
 * @retval      ��
 */
void esp8266_clear(void)
{
	memset(g_uart_rx_buf, 0, sizeof(g_uart_rx_buf));
	esp8266_cnt = 0;
}

/**
 * @brief       ESP8266 ��������
 * @param       cmd-�����͵�AT���res-�ڴ����յ����ַ���
 * @retval      ESP8266_EOK-�ɹ�  ESP8266_ERROR-ʧ��
 */
uint8_t esp8266_send_command(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);

	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK) {						//����յ�����
			if(strstr((const char *)g_uart_rx_buf, res) != NULL)		//����������ؼ���
				return ESP8266_EOK;
		}
		delay_ms(10);
	}

	return ESP8266_ERROR;

}

/**
 * @brief       ESP8266�ָ���������
 * @param       ��
 * @retval      ESP8266_EOK  : �ָ��������óɹ�
 *              ESP8266_ERROR: �ָ���������ʧ��
 */
uint8_t esp8266_restore(void)
{
    return esp8266_send_command("AT+RESTORE", "ready");
}

/**
 * @brief       ESP8266 ATָ�����
 * @param       ��
 * @retval      ESP8266_EOK  : ATָ����Գɹ�
 *              ESP8266_ERROR: ATָ�����ʧ��
 */
uint8_t esp8266_at_test(void)
{
    return esp8266_send_command("AT\r\n", "OK");
}

/**
 * @brief       ����ESP8266����ģʽ
 * @param       mode: 1��Stationģʽ
 *                    2��APģʽ
 *                    3��AP+Stationģʽ
 * @retval      ESP8266_EOK   : ����ģʽ���óɹ�
 *              ESP8266_ERROR : ����ģʽ����ʧ��
 *              ESP8266_EINVAL: mode�������󣬹���ģʽ����ʧ��
 */
uint8_t esp8266_set_mode(uint8_t mode)
{
    switch (mode) {
        case ESP8266_STA_MODE:
            return esp8266_send_command("AT+CWMODE=1\r\n", "OK");    /* Stationģʽ */
        
        case ESP8266_AP_MODE:
            return esp8266_send_command("AT+CWMODE=2\r\n", "OK");    /* APģʽ */
        
        case ESP8266_STA_AP_MODE:
            return esp8266_send_command("AT+CWMODE=3\r\n", "OK");    /* AP+Stationģʽ */
        
        default:
            return ESP8266_EINVAL;
    }
}

/**
 * @brief       ESP8266������λ
 * @param       ��
 * @retval      ESP8266_EOK  : ������λ�ɹ�
 *              ESP8266_ERROR: ������λʧ��
 */
uint8_t esp8266_sw_reset(void)
{
    return esp8266_send_command("AT+RST\r\n", "OK");
}

/**
 * @brief       ESP8266���û���ģʽ
 * @param       cfg: 0���رջ���
 *                   1���򿪻���
 * @retval      ESP8266_EOK  : ���û���ģʽ�ɹ�
 *              ESP8266_ERROR: ���û���ģʽʧ��
 */
uint8_t esp8266_ate_config(uint8_t cfg)
{
    switch (cfg) {
        case 0:
            return esp8266_send_command("ATE0\r\n", "OK");   /* �رջ��� */
        
        case 1:
            return esp8266_send_command("ATE1\r\n", "OK");   /* �򿪻��� */
        
        default:
            return ESP8266_EINVAL;
    }
}

/**
 * @brief       ESP8266����WIFI
 * @param       ssid: WIFI����
 *              pwd : WIFI����
 * @retval      ESP8266_EOK  : WIFI���ӳɹ�
 *              ESP8266_ERROR: WIFI����ʧ��
 */
uint8_t esp8266_join_ap(char *ssid, char *pwd)
{
    char cmd[64];
    
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    
    return esp8266_send_command(cmd, "WIFI GOT IP");
}

/**
 * @brief       ESP8266��ȡIP��ַ
 * @param       buf: IP��ַ����Ҫ16�ֽ��ڴ�ռ�
 * @retval      ESP8266_EOK  : ��ȡIP��ַ�ɹ�
 *              ESP8266_ERROR: ��ȡIP��ַʧ��
 */
uint8_t esp8266_get_ip(char *buf)
{
    char *p_start;
    char *p_end;

    if (esp8266_send_command("AT+CIFSR\r\n", "STAIP") != ESP8266_EOK)
        return ESP8266_ERROR;

    p_start = strstr((const char *)g_uart_rx_buf, "\"");
    p_end = strstr(p_start + 1, "\"");
    *p_end = '\0';
    sprintf(buf, "%s", p_start + 1);
    
    return ESP8266_EOK;
}

/**
 * @brief       ESP8266����TCP������
 * @param       server_ip  : TCP������IP��ַ
 *              server_port: TCP�������˿ں�
 * @retval      ESP8266_EOK  : ����TCP�������ɹ�
 *              ESP8266_ERROR: ����TCP������ʧ��
 */
uint8_t esp8266_connect_tcp_server(char *server_ip, char *server_port)
{
    char cmd[64];

    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", server_ip, server_port);

    return esp8266_send_command(cmd, "CONNECT");
}

/**
 * @brief       ESP8266�Ͽ�TCP������
 * @param       ��
 * @retval      ESP8266_EOK  : �Ͽ�TCP�������ɹ�
 *              ESP8266_ERROR: �Ͽ�TCP������ʧ��
 */
uint8_t esp8266_disconnect_tcp_server(void)
{
    return esp8266_send_command("AT+CIPCLOSE\r\n", "");

}

/**
 * @brief       ESP8266����͸��
 * @param       ��
 * @retval      ESP8266_EOK  : ����͸���ɹ�
 *              ESP8266_ERROR: ����͸��ʧ��
 */
uint8_t esp8266_enter_unvarnished(void)
{
    uint8_t ret;
    
    ret  = esp8266_send_command("AT+CIPMODE=1\r\n", "OK");
    ret += esp8266_send_command("AT+CIPSEND\r\n", ">");
    if (ret == ESP8266_EOK)
        return ESP8266_EOK;
    else
        return ESP8266_ERROR;
}

/**
 * @brief       ESP8266�˳�͸��
 * @param       ��
 * @retval      ESP8266_EOK  : �˳�͸���ɹ�
 *              ESP8266_ERROR: �˳�͸��ʧ��
 */
uint8_t esp8266_exit_unvarnished(void)
{
    return esp8266_send_command("+++", "");

}

/**
 * @brief       ESP8266���뵥����
 * @param       ��
 * @retval      ESP8266_EOK  : ���뵥���ӳɹ�
 *              ESP8266_ERROR: ���뵥����ʧ��
 */
uint8_t esp8266_single_connection(void)
{
    return esp8266_send_command("AT+CIPMUX=0\r\n", "OK");
}

/**
 * @brief       ESP8266��ʼ��
 * @param       baudrate: ESP8266 UARTͨѶ������
 * @retval      ESP8266_EOK  : ESP8266��ʼ���ɹ�������ִ�гɹ�
 *              ESP8266_ERROR: ESP8266��ʼ��ʧ�ܣ�����ִ��ʧ��
 */
uint8_t esp8266_init(uint32_t baudrate)
{
    char ip_buf[16];

    //esp8266_uart_init(baudrate);                /* ESP8266 UART��ʼ�� */

    /* ��WIFI�˳�͸��ģʽ */
	while(esp8266_exit_unvarnished())
        delay_ms(500);

	printf("1.AT\r\n");
	while(esp8266_at_test())
        delay_ms(500);
	
	printf("2.RST\r\n");
    while(esp8266_sw_reset())
        delay_ms(500);
	while(esp8266_disconnect_tcp_server())
        delay_ms(500);
	
	printf("3.CWMODE\r\n");
	while(esp8266_set_mode(ESP8266_STA_MODE))
        delay_ms(500);
	
	printf("4.AT+CIPMUX\r\n");  //���õ�·����ģʽ��͸��ֻ��ʹ�ô�ģʽ
	while(esp8266_single_connection())
        delay_ms(500);
	
	printf("5.CWJAP\r\n");      //����WIFI
	while(esp8266_join_ap(WIFI_SSID, WIFI_PWD))
        delay_ms(1000);
    
    printf("6.CIFSR\r\n");
    while(esp8266_get_ip(ip_buf))
        delay_ms(500);

    printf("ESP8266 IP: %s\r\n", ip_buf);
    
//    printf("7.CIPSTART\r\n");
//    while(esp8266_connect_tcp_server(TCP_SERVER_IP, TCP_SERVER_PORT))
//        delay_ms(500);
//    
//    printf("8.CIPMODE\r\n");
//    while(esp8266_enter_unvarnished())
//        delay_ms(500);
    
    printf("ESP8266_Init OK\r\n");
    return ESP8266_EOK;
}

/**
 * @brief       ESP8266 UART printf
 * @param       fmt: ����ӡ������
 * @retval      ��
 */
void esp8266_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);
    
    len = strlen((const char *)g_uart_tx_buf);
  //  HAL_UART_Transmit(&g_uart_handle, g_uart_tx_buf, len, HAL_MAX_DELAY);
}