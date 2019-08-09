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
#include <stdio.h>
#include "project.h"

#define IPC_CHANNEL_UARTTX  (8)  /* メッセージ送受信用の IPC チャネル番号 */

/* IPC チャネルのハンドル */
IPC_STRUCT_Type *ipcHandle;

int main(void) {
    uint32_t count = 0;  /* メッセージ番号 */
    uint32_t side = 0;  /* 使用中バッファ面 */
    char_t buffer[2][128]; /* ダブルバッファ */

    __enable_irq(); /* 全体の割り込みを許可する */

    /* メッセージ交換のためのハンドルを獲得する */
    ipcHandle = Cy_IPC_Drv_GetIpcBaseAddress(IPC_CHANNEL_UARTTX);
    
    for (;;) {
        /* 送信するメッセージを作成する */
        (void)sprintf(buffer[side], "M4: HELLO WORLD %lu\r\n", count++);
        /* メッセージを送る */
        while (
            Cy_IPC_Drv_SendMsgPtr(ipcHandle, CY_IPC_NO_NOTIFICATION, buffer[side]) != CY_IPC_DRV_SUCCESS
        ) ;
        /* バッファを切り替える */
        side = (side)?(0):(1);
    }
}

/* [] END OF FILE */
