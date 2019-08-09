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

/* Set 割り込みの処理ルーチン */
void Int_Set_isr(void) {
    /* GPIO 出力をセット (High) する。 */
    Cy_GPIO_Set(Pin_LEDR_PORT, Pin_LEDR_NUM);
}

int main(void) {
    __enable_irq(); /* 全体の割り込みを許可する */

    /* Set 割り込みの初期化 */
    Cy_SysInt_Init(&Int_Set_cfg, Int_Set_isr);
    NVIC_ClearPendingIRQ(Int_Set_cfg.intrSrc);
    NVIC_EnableIRQ(Int_Set_cfg.intrSrc);
        
    for (;;) {
    }
}

/* [] END OF FILE */
