/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    __enable_irq(); /* 全体の割り込みを許可する */

    /* 500m秒待つ */
    CyDelay(500);

    for(;;)
    {
        /* GPIO 出力をセット (High) する。 */
        Cy_GPIO_Set(Pin_LEDR_PORT, Pin_LEDR_NUM);
        /* 1000m秒待つ */
        CyDelay(1000);
    }
}

/* [] END OF FILE */
