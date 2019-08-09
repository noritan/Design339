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

#define IPC_CHANNEL_LED  (8)  /* メッセージ送受信用の IPC チャネル番号 */

int main(void)
{
    IPC_STRUCT_Type *ipcHandle;
    
    __enable_irq(); /* 全体の割り込みを許可する */

    /* メッセージ交換のためのハンドルを獲得する */
    ipcHandle = Cy_IPC_Drv_GetIpcBaseAddress(IPC_CHANNEL_LED);
    
    /* Cortex-M4 を叩き起こして CY_CORTEX_M4_APPL_ADDR から実行させる。 */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    for(;;)
    {
        /* GPIO 出力をクリア (Low) する。 */
        Cy_GPIO_Clr(Pin_LEDR_PORT, Pin_LEDR_NUM);
        /* 500m秒待つ */
        CyDelay(500);
        /* GPIO 出力をセット (High) する。 */
        Cy_GPIO_Set(Pin_LEDR_PORT, Pin_LEDR_NUM);
        /* Cortex-M4にメッセージを送る */
        while (
            Cy_IPC_Drv_SendMsgWord(ipcHandle, CY_IPC_NO_NOTIFICATION, 0) != CY_IPC_DRV_SUCCESS
        );
        /* Cortex-M4の受信確認を待つ */
        while (
            Cy_IPC_Drv_IsLockAcquired(ipcHandle) == true
        );
    }
}

/* [] END OF FILE */
