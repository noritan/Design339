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

/* Clear 割り込みの処理ルーチン */
void Int_Clear_isr(void) {
    /* GPIO 出力をクリア (Low) する。 */
    Cy_GPIO_Clr(Pin_LEDR_PORT, Pin_LEDR_NUM);
}

int main(void) {
    __enable_irq(); /* 全体の割り込みを許可する */
    
    /* PWM の起動 */
    PWM_Start();

    /* Cortex-M4 を叩き起こして CY_CORTEX_M4_APPL_ADDR から実行させる。 */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* Clear 割り込みの初期化 */
    Cy_SysInt_Init(&Int_Clear_cfg, Int_Clear_isr);
    NVIC_ClearPendingIRQ(Int_Clear_cfg.intrSrc);
    NVIC_EnableIRQ(Int_Clear_cfg.intrSrc);

    for (;;) {
    }
}

/* [] END OF FILE */
