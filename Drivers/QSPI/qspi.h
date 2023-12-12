/**
 ****************************************************************************************************
 * @file        qspi.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-09-06
 * @brief       QSPI ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ H743������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220906
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __QSPI_H
#define __QSPI_H

#include "main.h"


/******************************************************************************************/

/* QSPI������� ���� */
#define QSPI_BK1_CLK_GPIO_PORT          GPIOB
#define QSPI_BK1_CLK_GPIO_PIN           GPIO_PIN_2
#define QSPI_BK1_CLK_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

#define QSPI_BK1_NCS_GPIO_PORT          GPIOB
#define QSPI_BK1_NCS_GPIO_PIN           GPIO_PIN_6
#define QSPI_BK1_NCS_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_NCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

#define QSPI_BK1_IO0_GPIO_PORT          GPIOF
#define QSPI_BK1_IO0_GPIO_PIN           GPIO_PIN_6
#define QSPI_BK1_IO0_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF��ʱ��ʹ�� */

#define QSPI_BK1_IO1_GPIO_PORT          GPIOF
#define QSPI_BK1_IO1_GPIO_PIN           GPIO_PIN_7
#define QSPI_BK1_IO1_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF��ʱ��ʹ�� */

#define QSPI_BK1_IO2_GPIO_PORT          GPIOF
#define QSPI_BK1_IO2_GPIO_PIN           GPIO_PIN_8
#define QSPI_BK1_IO2_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_IO2_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF��ʱ��ʹ�� */

#define QSPI_BK1_IO3_GPIO_PORT          GPIOF
#define QSPI_BK1_IO3_GPIO_PIN           GPIO_PIN_9
#define QSPI_BK1_IO3_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_IO3_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF��ʱ��ʹ�� */

/******************************************************************************************/

uint8_t qspi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime);                   /* QSPI�ȴ�ĳ��״̬ */
uint8_t qspi_init(void);                                                              /* ��ʼ��QSPI */
void qspi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle);        /* QSPI�������� */
uint8_t qspi_receive(uint8_t *buf, uint32_t datalen);                                 /* QSPI�������� */
uint8_t qspi_transmit(uint8_t *buf, uint32_t datalen);                                /* QSPI�������� */

#endif



