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

#define SEMAPHORE_SENDER        (8) /* メッセージを送信する権利 */
#define PIPE_CLIENT_UARTTX      (0) /* UART送信用の CLIENT 番号 */

/* WDT を使った周期割り込み */
bool Int_WDT_flag0 = false;
bool Int_WDT_flag1 = false;
bool Int_WDT_flag2 = false;

void Int_WDT_isr(void) {
    uint32_t status = Cy_MCWDT_GetInterruptStatus(MCWDT_HW);
    if (status & CY_MCWDT_CTR0) {
        Cy_MCWDT_ClearInterrupt(MCWDT_HW, CY_MCWDT_CTR0);
        Int_WDT_flag0 = true;
    }  
    if (status & CY_MCWDT_CTR1) {
        Cy_MCWDT_ClearInterrupt(MCWDT_HW, CY_MCWDT_CTR1);
        Int_WDT_flag1 = true;
    }  
    if (status & CY_MCWDT_CTR2) {
        Cy_MCWDT_ClearInterrupt(MCWDT_HW, CY_MCWDT_CTR2);
        Int_WDT_flag2 = true;
    }  
}

void Int_WDT_Start(void) {
    /* 割り込みコンポーネントを初期化しイネーブル */
    Cy_SysInt_Init(&Int_WDT_cfg, Int_WDT_isr);
    NVIC_ClearPendingIRQ((IRQn_Type) Int_WDT_cfg.intrSrc);
    NVIC_EnableIRQ((IRQn_Type) Int_WDT_cfg.intrSrc);
   
    /* MCWDT コンポーネントを起動する */
    Cy_MCWDT_Init(MCWDT_HW, &MCWDT_config);
    Cy_MCWDT_Enable(MCWDT_HW, MCWDT_ENABLED_CTRS_MASK, 0u);
  
    /* MCWDT コンポーネントの割り込みマスクを設定する */
    Cy_MCWDT_SetInterruptMask(MCWDT_HW,
        CY_MCWDT_CTR0 | CY_MCWDT_CTR1 | CY_MCWDT_CTR2
    );
}

/* Task1の定義 */
enum Task1State {
    ST_CREATE,
    ST_SEND
};

struct Task1Context {
    enum Task1State state;  /* ステートマシンの状態 */
    bool *flag;  /* 割り込みフラグ */
    uint32_t count;  /* メッセージ番号 */
    uint32_t side;  /* 使用中バッファ面 */
    const char_t *name;  /* タスクの名前 */
    struct {
        uint32_t header;
        char_t message[128];
    } message[2]; /* ダブルバッファ */
};

void task1_init(struct Task1Context *context, const char_t *name, bool *flag) {
    /* タスクの初期化 */
    context->name = name;
    context->count = 0;
    context->side = 0;
    context->flag = flag;
    context->state = ST_CREATE;
}

void task1_dispatch(struct Task1Context *context) {
    switch (context->state) {
        case ST_CREATE:
            /* 送信するメッセージを作成する */
            context->message[context->side].header =
                _VAL2FLD(CY_IPC_PIPE_MSG_CLIENT,  PIPE_CLIENT_UARTTX) |
                _VAL2FLD(CY_IPC_PIPE_MSG_USR,     0u) |
                _VAL2FLD(CY_IPC_PIPE_MSG_RELEASE, CY_IPC_CYPIPE_INTR_MASK );
            (void)sprintf(
                context->message[context->side].message,
                "%s: HELLO WORLD %lu\r\n",
                context->name, context->count++
            );
            context->state = ST_SEND;
            break;
        case ST_SEND:
            /* フラグが立っている時のみ実行 */
            if (! *(context->flag)) break;
            /* セマフォを獲得する */
            if (Cy_IPC_Sema_Set(SEMAPHORE_SENDER, false) == CY_IPC_SEMA_SUCCESS) {
                /* メッセージを送る */
                if (
                    Cy_IPC_Pipe_SendMessage(
                        CY_IPC_EP_CYPIPE_CM0_ADDR,
                        CY_IPC_EP_CYPIPE_CM4_ADDR,
                        (uint32_t *)(&context->message[context->side]),
                        0  
                    ) == CY_IPC_PIPE_SUCCESS
                ) {
                    /* バッファを切り替える */
                    context->side = (context->side)?(0):(1);
                    context->state = ST_CREATE;
                    /* 割り込みフラグを折る */
                    *(context->flag) = false;
                } else {
                   /* メッセージの送信に失敗したので、セマフォを開放する */
                    while (
                        Cy_IPC_Sema_Clear(SEMAPHORE_SENDER, false) != CY_IPC_SEMA_SUCCESS
                    ) ;
                }
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
    
    __enable_irq(); /* Enable global interrupts. */

    /* タスクの初期化 */
    task1_init(&context1_0, "M4-1-0", &Int_WDT_flag0);
    task1_init(&context1_1, "M4-1-1", &Int_WDT_flag1);
    task1_init(&context1_2, "M4-1-2", &Int_WDT_flag2);
    
    /* WDT タイマの起動 */
    Int_WDT_Start();

    for (;;) {
        /* Task1 のディスパッチャ */
        task1_dispatch(&context1_0);
        task1_dispatch(&context1_1);        
        task1_dispatch(&context1_2);        
    }
}

/* [] END OF FILE */
