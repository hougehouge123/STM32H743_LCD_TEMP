#ifndef __MALLOC_H
#define __MALLOC_H

#include "main.h"

#ifndef NULL
#define NULL 0
#endif

/* ���������ڴ�� */
#define SRAMEX                  0                               /* �ⲿ�ڴ��(SDRAM),SDRAM��32MB */

#define SRAMBANK                1                               /* ����֧�ֵ�SRAM����. */

/* �����ڴ���������,������SDRAM��ʱ�򣬱���ʹ��uint32_t���ͣ�������Զ����uint16_t���Խ�ʡ�ڴ�ռ�� */
#define MT_TYPE     uint32_t


/* mem2�ڴ�����趨.mem2���ⲿ��SDRAM�ڴ� */
#define MEM2_BLOCK_SIZE         64                              /* �ڴ���СΪ64�ֽ� */
#define MEM2_MAX_SIZE           28912 * 1024                    /* �������ڴ�28912K,����SDRAM�ܹ�32MB,LTDCռ��2MB,��ʣ30MB. */
#define MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE / MEM2_BLOCK_SIZE /* �ڴ���С */



/* �ڴ��������� */
struct _m_mallco_dev
{
    void (*init)(uint8_t);              /* ��ʼ�� */
    uint16_t (*perused)(uint8_t);       /* �ڴ�ʹ���� */
    uint8_t *membase[SRAMBANK];         /* �ڴ�� ����SRAMBANK��������ڴ� */
    uint32_t *memmap[SRAMBANK];         /* �ڴ����״̬�� */
    uint8_t  memrdy[SRAMBANK];          /* �ڴ�����Ƿ���� */
};

extern struct _m_mallco_dev mallco_dev;                 /* ��mallco.c���涨�� */

/******************************************************************************************/

void my_mem_set(void *s, uint8_t c, uint32_t count);    /* �����ڴ� */
void my_mem_copy(void *des, void *src, uint32_t n);     /* �����ڴ� */
void my_mem_init(uint8_t memx);                         /* �ڴ�����ʼ������(��/�ڲ�����) */
uint32_t my_mem_malloc(uint8_t memx, uint32_t size);    /* �ڴ����(�ڲ�����) */
uint8_t my_mem_free(uint8_t memx, uint32_t offset);     /* �ڴ��ͷ�(�ڲ�����) */
uint16_t my_mem_perused(uint8_t memx) ;                 /* ����ڴ�ʹ����(��/�ڲ�����)  */

/* �û����ú��� */
void myfree(uint8_t memx, void *ptr);                   /* �ڴ��ͷ�(�ⲿ����) */
void *mymalloc(uint8_t memx, uint32_t size);            /* �ڴ����(�ⲿ����) */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size);/* ���·����ڴ�(�ⲿ����) */

#endif

