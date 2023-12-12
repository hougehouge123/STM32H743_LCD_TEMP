#include "qspi.h"
#include "delay.h"
#include "norflash.h"


uint16_t g_norflash_type = BY25Q256;      /* Ĭ����BY25Q256 */
uint8_t g_norflash_mode = 0;              /* QSPIģʽ��־��0��SPIģʽ��1��QPIģʽ */

/**
 * @brief       ��ʼ��SPI NOR FLASH
 * @param       ��
 * @retval      ��
 */
void norflash_init(void)
{
    uint8_t temp;
    qspi_init();                         /* ��ʼ��QSPI */
    norflash_qspi_disable();             /* �˳�QPIģʽ(����оƬ֮ǰ�������ģʽ,��������ʧ��) */
    norflash_qe_enable();                /* ʹ��QSPIģʽ */
    g_norflash_type = norflash_read_id();/* ��ȡFLASH ID */

    if (g_norflash_type == W25Q256 || g_norflash_type == BY25Q256) /* SPI FLASHΪ25Q256, ����ʹ��4�ֽڵ�ַģʽ */
    {
        temp = norflash_read_sr(3);      /* ��ȡ״̬�Ĵ���3���жϵ�ַģʽ */

        if ((temp & 0X01) == 0)          /* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
        {
            norflash_write_enable();     /* дʹ�� */
            /* QPI,ʹ��4�ֽڵ�ַָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
            qspi_send_cmd(FLASH_Enable4ByteAddr, 0, (0 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
        }

        norflash_write_enable();         /* дʹ�� */
        /* QPI,���ö�����ָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ����� */
        qspi_send_cmd(FLASH_SetReadParam, 0, (3 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
        temp = 3 << 4;                   /* ����P4&P5=11,8��dummy clocks,104M */
        qspi_transmit(&temp, 1);         /* ����1���ֽ� */
    }
}

/**
 * @brief       ʹ��FLASH QEλ��ʹ��IO2/IO3
 * @param       ��
 * @retval      ��
 */
void norflash_qe_enable(void)
{
    uint8_t stareg2;
    stareg2 = norflash_read_sr(2);      /* �ȶ���״̬�Ĵ���2��ԭʼֵ */

    if ((stareg2 & 0X02) == 0)          /* QEλδʹ�� */
    {
        norflash_write_enable();        /* дʹ�� */
        stareg2 |= 1 << 1;              /* ʹ��QEλ */
        norflash_write_sr(2, stareg2);  /* д״̬�Ĵ���2 */
    }

    /* дcommandָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ����� */
    qspi_send_cmd(FLASH_EnterQPIMode, 0, (0 << 6) | (0 << 4) | (0 << 2) | (1 << 0), 0);
    g_norflash_mode = 1;                /* ���QSPIģʽ */
}

/**
 * @brief       �˳�QSPIģʽ
 * @param       ��
 * @retval      ��
 */
void norflash_qspi_disable(void)
{
    /* дcommandָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
    qspi_send_cmd(FLASH_ExitQPIMode, 0, (0 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    g_norflash_mode = 0;                /* ���SPIģʽ */
}

/**
 * @brief       ��ȡ25QXX��״̬�Ĵ�����25QXXһ����3��״̬�Ĵ���
 * @note        ״̬�Ĵ���1��
 *              BIT7  6   5   4   3   2   1   0
 *              SPR   RV  TB BP2 BP1 BP0 WEL BUSY
 *              SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
 *              TB,BP2,BP1,BP0:FLASH����д��������
 *              WEL:дʹ������
 *              BUSY:æ���λ(1,æ;0,����)
 *              Ĭ��:0x00
 *
 *              ״̬�Ĵ���2��
 *              BIT7  6   5   4   3   2   1   0
 *              SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
 *
 *              ״̬�Ĵ���3��
 *              BIT7      6    5    4   3   2   1   0
 *              HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
 *
 * @param       regno: ״̬�Ĵ����ţ���Χ:1~3
 * @retval      ״̬�Ĵ���ֵ
 */
uint8_t norflash_read_sr(uint8_t regno)
{
    uint8_t byte = 0, command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* ��״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* ��״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* ��״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    if (g_norflash_mode)
    {
        /* QPI,дcommandָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ����� */
        qspi_send_cmd(command, 0, (3 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    }
    else 
    {
        /* SPI,дcommandָ��,��ַΪ0,���ߴ�����_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,1���ֽ����� */
        qspi_send_cmd(command, 0, (1 << 6) | (0 << 4) | (0 << 2) | (1 << 0), 0);
    }

    qspi_receive(&byte, 1);
    return byte;
}

/**
 * @brief       д25QXX״̬�Ĵ���
 * @note        �Ĵ���˵����norflash_read_sr����˵��
 * @param       regno: ״̬�Ĵ����ţ���Χ:1~3
 * @param       sr   : Ҫд��״̬�Ĵ�����ֵ
 * @retval      ��
 */
void norflash_write_sr(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* д״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* д״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* д״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    if (g_norflash_mode)
    {
        /* QPI,дcommandָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ����� */
        qspi_send_cmd(command, 0, (3 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    }
    else
    {
        /* SPI,дcommandָ��,��ַΪ0,���ߴ�����_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,1���ֽ����� */
        qspi_send_cmd(command, 0, (1 << 6) | (0 << 4) | (0 << 2) | (1 << 0), 0);
    }

    qspi_transmit(&sr, 1);
}

/**
 * @brief       25QXXдʹ��
 * @note        ��S1�Ĵ�����WEL��λ
 * @param       ��
 * @retval      ��
 */
void norflash_write_enable(void)
{
    if (g_norflash_mode)
    {
        /* QPI,дʹ��ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
        qspi_send_cmd(FLASH_WriteEnable, 0, (0 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    }
    else 
    {
        /* SPI,дʹ��ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ����� */
        qspi_send_cmd(FLASH_WriteEnable, 0, (0 << 6) | (0 << 4) | (0 << 2) | (1 << 0), 0);
    }
}

/**
 * @brief       25QXXд��ֹ
 * @note        ��S1�Ĵ�����WEL����
 * @param       ��
 * @retval      ��
 */
void norflash_write_disable(void)
{
    if (g_norflash_mode)
    {
        /* QPI,д��ָֹ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
        qspi_send_cmd(FLASH_WriteDisable, 0, (0 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    }
    else
    {
        /* SPI,д��ָֹ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ����� */
        qspi_send_cmd(FLASH_WriteDisable, 0, (0 << 6) | (0 << 4) | (0 << 2) | (1 << 0), 0);
    }
}

/**
 * @brief       ��ȡоƬID
 * @param       ��
 * @retval      FLASHоƬID
 * @note        оƬID�б��: norflash.h, оƬ�б���
 */
uint16_t norflash_read_id(void)
{
    uint8_t temp[2];
    uint16_t deviceid;

    if (g_norflash_mode)
    {
        /* QPI,��id,��ַΪ0,4�ߴ�������_24λ��ַ_4�ߴ����ַ_4�ߴ���ָ��,�޿�����,2���ֽ����� */
        qspi_send_cmd(FLASH_ManufactDeviceID, 0, (3 << 6) | (2 << 4) | (3 << 2) | (3 << 0), 0);
    }
    else 
    {
        /* SPI,��id,��ַΪ0,���ߴ�������_24λ��ַ_���ߴ����ַ_���ߴ���ָ��,�޿�����,2���ֽ����� */
        qspi_send_cmd(FLASH_ManufactDeviceID, 0, (1 << 6) | (2 << 4) | (1 << 2) | (1 << 0), 0);
    }

    qspi_receive(temp, 2);
    deviceid = (temp[0] << 8) | temp[1];
    return deviceid;
}

/**
 * @brief       ��ȡSPI FLASH,��֧��QSPIģʽ
 * @note        ��ָ����ַ��ʼ��ȡָ�����ȵ�����
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ(���32bit)
 * @param       datalen : Ҫ��ȡ���ֽ���(���65535)
 * @retval      ��
 */
void norflash_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    /* QPI,���ٶ�����,��ַΪReadAddr,4�ߴ�������_32λ��ַ_4�ߴ����ַ_4�ߴ���ָ��,8������,NumByteToRead������ */
    qspi_send_cmd(FLASH_FastReadData, addr, (3 << 6) | (3 << 4) | (3 << 2) | (3 << 0), 8);
    qspi_receive(pbuf, datalen);
}

/**
 * @brief       SPI��һҳ(0~65535)��д������256���ֽڵ�����
 * @note        ��ָ����ַ��ʼд�����256�ֽڵ�����
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       datalen : Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
 * @retval      ��
 */
static void norflash_write_page(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    norflash_write_enable();                    /* дʹ�� */
    /* QPI,ҳдָ��,��ַΪWriteAddr,4�ߴ�������_32λ��ַ_4�ߴ����ַ_4�ߴ���ָ��,�޿�����,NumByteToWrite������ */
    qspi_send_cmd(FLASH_PageProgram, addr, (3 << 6) | (3 << 4) | (3 << 2) | (3 << 0), 0);
    qspi_transmit(pbuf, datalen);
    norflash_wait_busy();                       /* �ȴ�д����� */
}

/**
 * @brief       �޼���дSPI FLASH
 * @note        ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
 *              �����Զ���ҳ����
 *              ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
 *
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       datalen : Ҫд����ֽ���(���65535)
 * @retval      ��
 */
static void norflash_write_nocheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t pageremain;
    pageremain = 256 - addr % 256;                /* ��ҳʣ����ֽ��� */

    if (datalen <= pageremain)
    {
        pageremain = datalen;                     /* ������256���ֽ� */
    }

    while (1)
    {
        norflash_write_page(pbuf, addr, pageremain);

        if (datalen == pageremain)
        {
            break;         /* д������� */
        }
        else 
        {
            pbuf += pageremain;
            addr += pageremain;

            datalen -= pageremain;               /* ��ȥ�Ѿ�д���˵��ֽ��� */

            if (datalen > 256)
            {
                pageremain = 256;  /* һ�ο���д��256���ֽ� */
            }
            else 
            {
                pageremain = datalen;           /* ����256���ֽ��� */
            }
        }
    }
}

/**
 * @brief       дSPI FLASH
 * @note        ��ָ����ַ��ʼд��ָ�����ȵ����� , �ú�������������!
 *              SPI FLASH һ����: 256���ֽ�Ϊһ��Page, 4KbytesΪһ��Sector, 16������Ϊ1��Block
 *              ��������С��λΪSector.
 *
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       datalen : Ҫд����ֽ���(���65535)
 * @retval      ��
 */
uint8_t g_norflash_buf[4096];

void norflash_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    norflash_buf = g_norflash_buf;
    secpos = addr / 4096;                                       /* ������ַ */
    secoff = addr % 4096;                                       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;                                  /* ����ʣ��ռ��С */

    //printf("ad:%X,nb:%X\r\n",addr, datalen);                  /* ������ */
    if (datalen <= secremain)secremain = datalen;               /* ������4096���ֽ� */

    while (1)
    {
        norflash_read(norflash_buf, secpos * 4096, 4096);       /* ������������������ */

        for (i = 0; i < secremain; i++)                         /* У������ */
        {
           if (norflash_buf[secoff + i] != 0XFF)
           {
               break;          /* ��Ҫ���� */
           }
           
        }

        if (i < secremain)                                      /* ��Ҫ���� */
        {
            norflash_erase_sector(secpos);                      /* ����������� */

            for (i = 0; i < secremain; i++)                     /* ���� */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }

            norflash_write_nocheck(norflash_buf, secpos * 4096, 4096);  /* д���������� */

        }
        else
        {
            norflash_write_nocheck(pbuf, addr, secremain);     /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        }

        if (datalen == secremain)
        {
            break;                          /* д������� */
        }
        else          /* д��δ���� */
        {
            secpos++;                            /* ������ַ��1 */
            secoff = 0;                          /* ƫ��λ��Ϊ0 */

            pbuf += secremain;                   /* ָ��ƫ�� */
            addr += secremain;                   /* д��ַƫ�� */
            datalen -= secremain;                /* �ֽ����ݼ� */

            if (datalen > 4096)
            {
                secremain = 4096; /* ��һ����������д���� */
            }
            else 
            {
                secremain = datalen;            /* ��һ����������д���� */
            }
        }
    }
}

/**
 * @brief       ��������оƬ
 * @note        �ȴ�ʱ�䳬��...
 * @param       ��
 * @retval      ��
 */
void norflash_erase_chip(void)
{
    norflash_write_enable();                    /* SET WEL */
    norflash_wait_busy();
    /* QPI,дȫƬ����ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
    qspi_send_cmd(FLASH_ChipErase, 0, (0 << 6) | (0 << 4) | (0 << 2) | (3 << 0), 0);
    norflash_wait_busy();                       /* �ȴ�оƬ�������� */
}

/**
 * @brief       ����һ������
 * @note        ע��,������������ַ,�����ֽڵ�ַ!!
 *              ����һ������������ʱ��:150ms
 *
 * @param       saddr : ������ַ ����ʵ����������
 * @retval      ��
 */
void norflash_erase_sector(uint32_t saddr)
{

    //printf("fe:%x\r\n",Dst_Addr);           /* ����falsh�������,������ */
    saddr *= 4096;
    norflash_write_enable();                  /* SET WEL */
    norflash_wait_busy();
    /* QPI,д��������ָ��,��ַΪ0,������_32λ��ַ_4�ߴ����ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� */
    qspi_send_cmd(FLASH_SectorErase, saddr, (0 << 6) | (3 << 4) | (3 << 2) | (3 << 0), 0);
    norflash_wait_busy();                     /* �ȴ�������� */
}

/**
 * @brief       �ȴ�����
 * @param       ��
 * @retval      ��
 */
void norflash_wait_busy(void)
{
    while ((norflash_read_sr(1) & 0x01) == 0x01);   /* �ȴ�BUSYλ��� */
}

