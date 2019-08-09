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

bool Int_Set_flag = false;  /* 割り込みありフラグ */

/* Set 割り込みの処理ルーチン */
void Int_Set_isr(void) {
    /* フラグを立てる */
    Int_Set_flag = true;
}

int main(void) {
    __enable_irq(); /* 全体の割り込みを許可する */

    /* Set 割り込みの初期化 */
    Cy_SysInt_Init(&Int_Set_cfg, Int_Set_isr);
    NVIC_ClearPendingIRQ(Int_Set_cfg.intrSrc);
    NVIC_EnableIRQ(Int_Set_cfg.intrSrc);
        
    for (;;) {
        if (Int_Set_flag) {
            /* セマフォを獲得する */
            if (
                Cy_IPC_Sema_Set(SEMAPHORE_LED, false) == CY_IPC_SEMA_SUCCESS
            ) {
                /* GPIO 出力をセット (High) する。 */
                Cy_GPIO_Set(Pin_LEDR_PORT, Pin_LEDR_NUM);
                /* セマフォを解放する */
                while (
                    Cy_IPC_Sema_Clear(SEMAPHORE_LED, false) != CY_IPC_SEMA_SUCCESS
                ) ;
                /* 使用済みフラグを折る */
                Int_Set_flag = false;
            }
        }
    }
}

/* [] END OF FILE */
