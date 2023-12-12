#include "malloc.h"
#include "nand.h"
#include "ftl.h"
#include "diskio.h"
#include "norflash.h"


#define EX_FLASH    0       /* �ⲿspi flash,���Ϊ0 */
#define EX_NAND     1       /* �ⲿNAND FLASH���Ϊ1 */

/**
 * ����25Q256 FLASHоƬ, ���ǹ涨ǰ 25M ��FATFSʹ��, 25M�Ժ�
 * ���ڴ���ֿ⣬�ֿ�ռ��6.01M.ʣ�ಿ�ָ��ͻ��Լ��� 
 */
#define SPI_FLASH_SECTOR_SIZE   512
#define SPI_FLASH_SECTOR_COUNT  25 * 1024 * 2   /* 25Q256, ǰ25M�ֽڸ�FATFSռ�� */
#define SPI_FLASH_BLOCK_SIZE    8               /* ÿ��BLOCK��8������ */
#define SPI_FLASH_FATFS_BASE    0               /* FATFS ���ⲿFLASH����ʼ��ַ ��0��ʼ */

/**
 * NAND FLASHȫ����FATFS����
 */
uint32_t NAND_FLASH_SECTOR_COUNT;
uint8_t NAND_FLASH_BLOCK_SIZE;
 

/**
 * @brief       ��ô���״̬
 * @param       pdrv : ���̱��0~9
 * @retval      ��
 */
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    return RES_OK;
}

/**
 * @brief       ��ʼ������
 * @param       pdrv : ���̱��0~9
 * @retval      ��
 */
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    uint8_t res = 0;

    switch (pdrv)
    {
        case EX_FLASH:          /* �ⲿflash */
            norflash_init(); 
            break;

        case EX_NAND:           /* NAND FLASH */
            res = nand_init();
            break;

        default:
            res = 1;
            break;
    }

    if (res)
    {
        return  STA_NOINIT;
    }
    else
    {
        return 0; /* ��ʼ���ɹ�*/
    }
}

/**
 * @brief       ������
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : ���ݽ��ջ����׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫ��ȡ��������
 * @retval      ��
 */
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    uint8_t res = 0;

    if (!count)
    {
        return RES_PARERR;   /* count���ܵ���0�����򷵻ز������� */
    }

    switch (pdrv)
    {
        case EX_FLASH:      /* �ⲿflash */
            for (; count > 0; count--)
            {
                norflash_read(buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff += SPI_FLASH_SECTOR_SIZE;
            }
            res = 0;
            break;

        case EX_NAND:      /* �ⲿNAND */
            res = ftl_read_sectors(buff, sector, 512, count);
            break;

        default:
            res = 1;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR; 
    }
}

/**
 * @brief       д����
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : �������ݻ������׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫд���������
 * @retval      ��
 */
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t res = 0;

    if (!count)
    {
        return RES_PARERR;   /* count���ܵ���0�����򷵻ز������� */
    }

    switch (pdrv)
    {
        case EX_FLASH:      /* �ⲿflash */
            for (; count > 0; count--)
            {
                norflash_write((uint8_t *)buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff += SPI_FLASH_SECTOR_SIZE;
            }
            res = 0;
            break;
            
        case EX_NAND:      /* �ⲿNAND */
            res = ftl_write_sectors((uint8_t *)buff, sector, 512, count);
            break;

        default:
            res = 1;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR; 
    }
}

/**
 * @brief       ��ȡ�������Ʋ���
 * @param       pdrv   : ���̱��0~9
 * @param       ctrl   : ���ƴ���
 * @param       buff   : ����/���ջ�����ָ��
 * @retval      ��
 */
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    if (pdrv == EX_FLASH)  /* �ⲿFLASH */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(WORD *)buff = SPI_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = SPI_FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = SPI_FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else if (pdrv == EX_NAND)  /* �ⲿNAND FLASH */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(WORD *)buff = 512;       /* NAND FLASH����ǿ��Ϊ512�ֽڴ�С */
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = nand_dev.page_mainsize / 512;    /* block��С�������һ��page�Ĵ�С */
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = nand_dev.valid_blocknum * nand_dev.block_pagenum * nand_dev.page_mainsize / 512;    /* NAND FLASH����������С */
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else
    {
        res = RES_ERROR;    /* �����Ĳ�֧�� */
    }
    
    return res;
}



