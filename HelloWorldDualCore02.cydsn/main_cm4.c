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

/* Task1の定義 */
enum Task1State {
    ST_CREATE,
    ST_SEND
};

struct Task1Context {
    enum Task1State state;  /* ステートマシンの状態 */
    uint32_t count;  /* メッセージ番号 */
    uint32_t side;  /* 使用中バッファ面 */
    const char_t *name;  /* タスクの名前 */
    char_t buffer[2][128]; /* ダブルバッファ */
};

void task1_init(struct Task1Context *context, const char_t *name) {
    /* タスクの初期化 */
    context->name = name;
    context->count = 0;
    context->side = 0;
    context->state = ST_CREATE;
}

void task1_dispatch(struct Task1Context *context) {
    switch (context->state) {
        case ST_CREATE:
            /* 送信するメッセージを作成する */
            (void)sprintf(
                context->buffer[context->side],
                "%s: HELLO WORLD %lu\r\n",
                context->name, context->count++
            );
            context->state = ST_SEND;
            break;
        case ST_SEND:        
            /* メッセージを送る */
            if (
                Cy_IPC_Drv_SendMsgPtr(
                    ipcHandle,
                    CY_IPC_NO_NOTIFICATION,
                    context->buffer[context->side]
                ) == CY_IPC_DRV_SUCCESS
            ) {
                /* バッファを切り替える */
                context->side = (context->side)?(0):(1);
                context->state = ST_CREATE;
            }
            break;
        default:
            CY_ASSERT(false);
    }
}


int main(void) {
    struct Task1Context context1_0;
    struct Task1Context context1_1;
    struct Task1Context context1_2;
    
    __enable_irq(); /* 全体の割り込みを許可する */

    /* メッセージ交換のためのハンドルを獲得する */
    ipcHandle = Cy_IPC_Drv_GetIpcBaseAddress(IPC_CHANNEL_UARTTX);
    
    /* タスクの初期化 */
    task1_init(&context1_0, "M4-1-0");
    task1_init(&context1_1, "M4-1-1");
    task1_init(&context1_2, "M4-1-2");

    for (;;) {
        /* Task1 のディスパッチャ */
        task1_dispatch(&context1_0);
        task1_dispatch(&context1_1);        
        task1_dispatch(&context1_2);        
    }
}

/* [] END OF FILE */
