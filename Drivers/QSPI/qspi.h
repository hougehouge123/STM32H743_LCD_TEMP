#ifndef __QSPI_H
#define __QSPI_H

#include "main.h"


/******************************************************************************************/

/* QSPI相关引脚 定义 */
#define QSPI_BK1_CLK_GPIO_PORT          GPIOB
#define QSPI_BK1_CLK_GPIO_PIN           GPIO_PIN_2
#define QSPI_BK1_CLK_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define QSPI_BK1_NCS_GPIO_PORT          GPIOB
#define QSPI_BK1_NCS_GPIO_PIN           GPIO_PIN_6
#define QSPI_BK1_NCS_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_NCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define QSPI_BK1_IO0_GPIO_PORT          GPIOF
#define QSPI_BK1_IO0_GPIO_PIN           GPIO_PIN_6
#define QSPI_BK1_IO0_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF口时钟使能 */

#define QSPI_BK1_IO1_GPIO_PORT          GPIOF
#define QSPI_BK1_IO1_GPIO_PIN           GPIO_PIN_7
#define QSPI_BK1_IO1_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF口时钟使能 */

#define QSPI_BK1_IO2_GPIO_PORT          GPIOF
#define QSPI_BK1_IO2_GPIO_PIN           GPIO_PIN_8
#define QSPI_BK1_IO2_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_IO2_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF口时钟使能 */

#define QSPI_BK1_IO3_GPIO_PORT          GPIOF
#define QSPI_BK1_IO3_GPIO_PIN           GPIO_PIN_9
#define QSPI_BK1_IO3_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_IO3_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PF口时钟使能 */

/******************************************************************************************/

uint8_t qspi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime);                   /* QSPI等待某个状态 */
uint8_t qspi_init(void);                                                              /* 初始化QSPI */
void qspi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle);        /* QSPI发送命令 */
uint8_t qspi_receive(uint8_t *buf, uint32_t datalen);                                 /* QSPI接收数据 */
uint8_t qspi_transmit(uint8_t *buf, uint32_t datalen);                                /* QSPI发送数据 */

#endif



