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

#define SEMAPHORE_SENDER        (8) /* メッセージを送信する権利 */
#define PIPE_CLIENT_UARTTX      (0) /* UART送信用の CLIENT 番号 */

/* 送信バッファの定義 */
char_t txBuffer[128] = ""; /* バッファ本体 */
uint32_t txColumn = 0; /* 次に送るべき文字へのインデックス */
uint32_t txCharsLeft = 0;  /* 送信すべき残り文字数 */

void pipeUartTxCallback(uint32_t *message) {
    CY_ASSERT(
        txCharsLeft == 0
    );
    
    /* 受信成功:急いでデータをバッファにコピー */
    strcpy(txBuffer, (char_t *)(&message[1]));
    txCharsLeft = strlen(txBuffer);
    txColumn = 0;
}

int main(void) {
    __enable_irq(); /* 全体の割り込みを許可する */
    
    /* メッセージ受信後に呼び出されるCallbackを登録する */
    CY_ASSERT(
        Cy_IPC_Pipe_RegisterCallback(
            CY_IPC_EP_CYPIPE_CM0_ADDR,
            pipeUartTxCallback,
            PIPE_CLIENT_UARTTX
        ) == CY_IPC_PIPE_SUCCESS
    );

    /* Cortex-M4 を叩き起こして CY_CORTEX_M4_APPL_ADDR から実行させる。 */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* UART を初期化する */
    UART_Start();

    for (;;) {
        /* UART制御ブロック */
        if (txCharsLeft > 0) {
            /* バッファのデータを一文字ずつちんたら送り出す。 */
            UART_Put(txBuffer[txColumn]);
            txColumn++;
            txCharsLeft--;
            
            if (txCharsLeft <= 0) {
                /* すべて送り出したらセマフォを開放する */
                while (
                    Cy_IPC_Sema_Clear(SEMAPHORE_SENDER, false) != CY_IPC_SEMA_SUCCESS
                ) ;
            }
        }
        /* 一文字当たり10msの低速処理 */
        CyDelay(10);
    }
}

/* [] END OF FILE */
