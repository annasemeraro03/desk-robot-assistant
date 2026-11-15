#ifndef MOOD_TASK_H
#define MOOD_TASK_H

/**
 * @brief Task Periodico Real-Time tau_4: Task_FSM_Core (T_4 = 50 ms, Priorita' RM = 3).
 *        Gestisce la Macchina a Stati Finiti (8 stati base + VITTORIA), la preemption
 *        immediata e il congelamento/ripristino del Timer Pomodoro (History State).
 *
 * @param pvParameters Parametri task FreeRTOS (non utilizzato).
 */
void moodTask(void *pvParameters);

#endif