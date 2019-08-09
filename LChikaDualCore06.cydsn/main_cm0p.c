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

// uint32 semaphoreStore[4];  /* セマフォの格納庫 */

/* Clear 割り込みの処理ルーチン */
void Int_Clear_isr(void) {
    /* セマフォを獲得する */
    while (
        Cy_IPC_Sema_Set(SEMAPHORE_LED, false) != CY_IPC_SEMA_SUCCESS
    ) ;
    /* GPIO 出力をクリア (Low) する。 */
    Cy_GPIO_Clr(Pin_LEDR_PORT, Pin_LEDR_NUM);
    /* セマフォを解放する */
    while (
        Cy_IPC_Sema_Clear(SEMAPHORE_LED, false) != CY_IPC_SEMA_SUCCESS
    ) ;
}

int main(void) {
    __enable_irq(); /* 全体の割り込みを許可する */
    
    /* PWM の起動 */
    PWM1000_Start();
    PWM1005_Start();

//    /* セマフォを初期化する */
//    CY_ASSERT(
//        Cy_IPC_Sema_Init(
//            CY_IPC_CHAN_SEMA,
//            sizeof(semaphoreStore) * 8,
//            semaphoreStore
//        ) == CY_IPC_SEMA_SUCCESS
//    );

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
