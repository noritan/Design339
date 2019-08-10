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
#include <string.h>
#include "project.h"

#define IPC_CHANNEL_UARTTX  (8)  /* メッセージ送受信用の IPC チャネル番号 */

/* 送信バッファの定義 */
char_t txBuffer[128] = ""; /* バッファ本体 */
uint32_t txColumn = 0; /* 次に送るべき文字へのインデックス */
uint32_t txCharsLeft = 0;  /* 送信すべき残り文字数 */

int main(void) {
    /* IPC チャネルのハンドル */
    IPC_STRUCT_Type *ipcHandle;

    __enable_irq(); /* 全体の割り込みを許可する */

    /* メッセージ交換のためのハンドルを獲得する */
    ipcHandle = Cy_IPC_Drv_GetIpcBaseAddress(IPC_CHANNEL_UARTTX);

    /* Cortex-M4 を叩き起こして CY_CORTEX_M4_APPL_ADDR から実行させる。 */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* UART を初期化する */
    UART_Start();

    for (;;) {
        /* UART制御ブロック */
        if (txCharsLeft <= 0) {
            char_t *message; /* 受信したメッセージへのポインタ */

            /* バッファが空の時はメッセージが到着するのを待つ */
            if (
                Cy_IPC_Drv_ReadMsgPtr(ipcHandle, (void *)&message) == CY_IPC_DRV_SUCCESS
            ) {
                /* 受信成功:急いでデータをバッファにコピー */
                strcpy(txBuffer, message);
                txCharsLeft = strlen(txBuffer);
                txColumn = 0;
                /* IPC チャネルの解放 */
                CY_ASSERT(
                    Cy_IPC_Drv_LockRelease(ipcHandle, CY_IPC_NO_NOTIFICATION) == CY_IPC_DRV_SUCCESS
                );                
            } else {
                /* 受信できず：次回に期待 */
            }
        } else {
            /* バッファのデータを一文字ずつちんたら送り出す。 */
            UART_Put(txBuffer[txColumn]);
            txColumn++;
            txCharsLeft--;
        }
        /* 一文字当たり10msの低速処理 */
        CyDelay(10);
    }
}

/* [] END OF FILE */
