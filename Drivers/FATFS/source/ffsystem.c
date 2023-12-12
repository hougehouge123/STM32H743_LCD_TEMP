#include "malloc.h"
#include "main.h"
#include "ff.h"


/**
 * @brief       ���ʱ��
 * @param       mf  : �ڴ��׵�ַ
 * @retval      ʱ��
 * @note        ʱ������������:
 *              User defined function to give a current time to fatfs module 
 *              31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)
 *              15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) 
 */
DWORD get_fattime (void)
{
    return 0;
}

/**
 * @brief       ��̬�����ڴ�
 * @param       size : Ҫ������ڴ��С(�ֽ�)
 * @retval      ���䵽���ڴ��׵�ַ.
 */
void *ff_memalloc (UINT size)
{
    return (void*)mymalloc(SRAMEX, size);
}

/**
 * @brief       �ͷ��ڴ�
 * @param       mf  : �ڴ��׵�ַ
 * @retval      ��
 */
void ff_memfree (void* mf)
{
    myfree(SRAMEX, mf);
}


