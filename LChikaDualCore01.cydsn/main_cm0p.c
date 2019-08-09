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
    /* Cortex-M4 を叩き起こして CY_CORTEX_M4_APPL_ADDR から実行させる。 */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    for(;;)
    {
        /* GPIO 出力をクリア (Low) する。 */
        Cy_GPIO_Clr(Pin_LEDR_PORT, Pin_LEDR_NUM);
        /* 1000m秒待つ */
        CyDelay(1000);
    }
}

/* [] END OF FILE */
