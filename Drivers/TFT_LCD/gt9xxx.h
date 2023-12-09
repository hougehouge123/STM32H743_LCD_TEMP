#ifndef __GT9XXX_H
#define __GT9XXX_H

#include "ILI93xx.h"

#define TP_PRES_DOWN    0x8000  /* ���������� */
#define TP_CATH_PRES    0x4000  /* �а��������� */
/* ������������ */
typedef struct
{
    uint16_t x[5];   /* ��ǰ���� */
    uint16_t y[5];   /* �����������10������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
                                 * x[9],y[9]�洢��һ�ΰ���ʱ������.
                                 */

    uint16_t sta;               /* �ʵ�״̬
                                 * b15:����1/�ɿ�0;
                                 * b14:0,û�а�������;1,�а�������.
                                 * b13~b10:����
                                 * b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
                                 */
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;                                                    /* ������������touch.c���涨�� */


/******************************************************************************************/

/* GT9XXX INT �� RST ���� ���� */
#define GT9XXX_RST_GPIO_PORT            GPIOI
#define GT9XXX_RST_GPIO_PIN             GPIO_PIN_8
#define GT9XXX_RST_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)     /* PI��ʱ��ʹ�� */

#define GT9XXX_INT_GPIO_PORT            GPIOH
#define GT9XXX_INT_GPIO_PIN             GPIO_PIN_7
#define GT9XXX_INT_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)     /* PH��ʱ��ʹ�� */

/******************************************************************************************/

/* ����ݴ��������ӵ�оƬ����(δ����IIC����) 
 * IO�������� 
 */
#define GT9XXX_RST(x)     do{ x ? \
                              HAL_GPIO_WritePin(GT9XXX_RST_GPIO_PORT, GT9XXX_RST_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(GT9XXX_RST_GPIO_PORT, GT9XXX_RST_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)                                                       /* ��λ���� */

#define GT9XXX_INT        HAL_GPIO_ReadPin(GT9XXX_INT_GPIO_PORT, GT9XXX_INT_GPIO_PIN)     /* ��ȡ�������� */

/* IIC��д���� */
#define GT9XXX_CMD_WR       0X28        /* д���� */
#define GT9XXX_CMD_RD       0X29        /* ������ */

/* GT9XXX ���ּĴ�������  */
#define GT9XXX_CTRL_REG     0X8040      /* GT9XXX���ƼĴ��� */
#define GT9XXX_CFGS_REG     0X8047      /* GT9XXX������ʼ��ַ�Ĵ��� */
#define GT9XXX_CHECK_REG    0X80FF      /* GT9XXXУ��ͼĴ��� */
#define GT9XXX_PID_REG      0X8140      /* GT9XXX��ƷID�Ĵ��� */

#define GT9XXX_GSTID_REG    0X814E      /* GT9XXX��ǰ��⵽�Ĵ������ */
#define GT9XXX_TP1_REG      0X8150      /* ��һ�����������ݵ�ַ */
#define GT9XXX_TP2_REG      0X8158      /* �ڶ������������ݵ�ַ */
#define GT9XXX_TP3_REG      0X8160      /* ���������������ݵ�ַ */
#define GT9XXX_TP4_REG      0X8168      /* ���ĸ����������ݵ�ַ */
#define GT9XXX_TP5_REG      0X8170      /* ��������������ݵ�ַ */
#define GT9XXX_TP6_REG      0X8178      /* ���������������ݵ�ַ */
#define GT9XXX_TP7_REG      0X8180      /* ���߸����������ݵ�ַ */
#define GT9XXX_TP8_REG      0X8188      /* �ڰ˸����������ݵ�ַ */
#define GT9XXX_TP9_REG      0X8190      /* �ھŸ����������ݵ�ַ */
#define GT9XXX_TP10_REG     0X8198      /* ��ʮ�����������ݵ�ַ */

/******************************************************************************************/

uint8_t gt9xxx_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void gt9xxx_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len); 
uint8_t gt9xxx_init(void);
uint8_t gt9xxx_scan(uint8_t mode); 

#endif

