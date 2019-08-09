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
    uint32_t message;

    __enable_irq(); /* 全体の割り込みを許可する */

    /* メッセージ交換のためのハンドルを獲得する */
    ipcHandle = Cy_IPC_Drv_GetIpcBaseAddress(IPC_CHANNEL_LED);

    for(;;)
    {
        /* Cortex-M0+からメッセージが到着するのを待つ */
        while (
            Cy_IPC_Drv_ReadMsgWord(ipcHandle, &message) != CY_IPC_DRV_SUCCESS
        );
        /* GPIO 出力をクリア (Low) する。 */
        Cy_GPIO_Clr(Pin_LEDG_PORT, Pin_LEDG_NUM);
        /* 500m秒待つ */
        CyDelay(500);
        /* GPIO 出力をセット (High) する。 */
        Cy_GPIO_Set(Pin_LEDG_PORT, Pin_LEDG_NUM);
        /* Cortex-M0+にメッセージの受信確認を送る */
        CY_ASSERT(
            Cy_IPC_Drv_LockRelease(ipcHandle, CY_IPC_NO_NOTIFICATION) == CY_IPC_DRV_SUCCESS
        );
    }
}

/* [] END OF FILE */
