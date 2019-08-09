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

#define SEMAPHORE_LED  (8)  /* セマフォの識別番号 */

int main(void)
{
    __enable_irq(); /* 全体の割り込みを許可する */

    for(;;)
    {
        /* セマフォを獲得する */
        while (
            Cy_IPC_Sema_Set(SEMAPHORE_LED, false) != CY_IPC_SEMA_SUCCESS
        ) ;
        /* GPIO 出力をクリア (Low) する。 */
        Cy_GPIO_Clr(Pin_LEDG_PORT, Pin_LEDG_NUM);
        /* 500m秒待つ */
        CyDelay(500);
        /* GPIO 出力をセット (High) する。 */
        Cy_GPIO_Set(Pin_LEDG_PORT, Pin_LEDG_NUM);
        /* セマフォを解放する */
        while (
            Cy_IPC_Sema_Clear(SEMAPHORE_LED, false) != CY_IPC_SEMA_SUCCESS
        ) ;
        /* セマフォが引き渡されるまで待つ */
        while (
            Cy_IPC_Sema_Status(SEMAPHORE_LED) != CY_IPC_SEMA_STATUS_LOCKED
        ) ;
    }
}

/* [] END OF FILE */
