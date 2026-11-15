#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// SWITCH DI TEST RAPIDO PER I TIMER LUNGHI (Specifica Sez. 3.1 e 3.3)
// ============================================================================
// Impostare a 1 durante le prove a banco per ridurre:
//   - Timeout inattivita' (FELICE -> ANNOIATA): da 5 minuti a 15 secondi
//   - Durata sessione Pomodoro (CONCENTRATA -> VITTORIA): da 40 minuti a 30 secondi
// Impostare a 0 per ripristinare i tempi nominali della specifica (5 min e 40 min).
#define DEBUG_SHORT_TIMERS 1

// ============================================================================
// Comunicazione Seriale e Bus I2C Condiviso (Specifica Sez. 4.2)
// ============================================================================
#define SERIAL_BAUDRATE        9600
#define I2C_FAST_MODE_HZ       400000UL  // 400 kHz Fast Mode richiesto per WCET I2C

// ============================================================================
// Configurazione Display OLED SH1106 1.3" (Specifica Sez. 4.2 e 5.1)
// ============================================================================
#define DISPLAY_WIDTH          128
#define DISPLAY_HEIGHT         64
#define DISPLAY_PAGES          8         // 8 pagine hardware da 128 byte (1024 B totali)
#define DISPLAY_PAGE_BYTES     128
#define DISPLAY_RESET          -1
#define DISPLAY_I2C_ADDRESS    0x3C

// ============================================================================
// Partizionamento Dual-Core ESP32-S3 (Specifica Sez. 4.1)
// ============================================================================
#define CORE_COMM_PRO_CPU      0         // Core 0: Stack comunicazione / GUI
#define CORE_RT_APP_CPU        1         // Core 1: Isolamento dei 6 task Real-Time

// ============================================================================
// Parametri Scheduling Rate Monotonic (RM) su Core 1 (Specifica Sez. 5.1)
// ============================================================================
// Periodi T_i = D_i (ms)
#define TASK_IMU_PERIOD_MS     20UL      // tau_1: Task_IMU_Safety   (T1 = 20 ms)
#define TASK_TOUCH_PERIOD_MS   25UL      // tau_2: Task_Touch_Input  (T2 = 25 ms)
#define TASK_MOTOR_PERIOD_MS   50UL      // tau_3: Task_Motor_Ctrl   (T3 = 50 ms)
#define TASK_FSM_PERIOD_MS     50UL      // tau_4: Task_FSM_Core     (T4 = 50 ms)
#define TASK_OLED_PERIOD_MS    100UL     // tau_5: Task_OLED_UI      (T5 = 100 ms)
#define TASK_SYNC_PERIOD_MS    500UL     // tau_6: Task_Sync_Cal     (T6 = 500 ms)

// Priorita' statiche Rate Monotonic (T_i < T_j => P_i > P_j)
#define PRIO_TASK_IMU_SAFETY   6         // Massima (6)
#define PRIO_TASK_TOUCH_INPUT  5         // Alta (5)
#define PRIO_TASK_MOTOR_CTRL   4         // Media-Alta (4)
#define PRIO_TASK_FSM_CORE     3         // Media (3)
#define PRIO_TASK_OLED_UI      2         // Bassa (2)
#define PRIO_TASK_SYNC_CAL     1         // Minima (1)

// ============================================================================
// Grammatica Temporale Sensore Touch TTP223 (Specifica Sez. 3.2.A)
// ============================================================================
#define TOUCH_MIN_TAP_MS       50UL      // Impulso breve minimo: 50 ms <= t_on < 400 ms
#define TOUCH_MAX_TAP_MS       400UL     // Impulso breve massimo: < 400 ms
#define TOUCH_INTER_TAP_GAP_MS 350UL     // Finestra di inter-tap timeout: t_gap = 350 ms
#define TOUCH_PETTING_MIN_MS   800UL     // Soglia carezza/coccola: t_on > 800 ms

// ============================================================================
// Soglie Rilevamento Sollevamento / Sballottamento MPU6050 (Specifica Sez. 5.1)
// ============================================================================
// Valori di accelerazione lineare (m/s^2) e velocita' angolare (rad/s) oltre i quali
// viene generato EV_SHOCK_LIFT (preemption immediata verso ARRABBIATA)
#define IMU_ACCEL_NORM_MIN_MS2 5.5f      // Sollevamento brusco / caduta libera (< 5.5 m/s^2)
#define IMU_ACCEL_NORM_MAX_MS2 15.5f     // Urto / sballottamento (> 15.5 m/s^2 vs 9.81 m/s^2)
#define IMU_GYRO_NORM_MAX_RADS 2.8f      // Rotazione/inclinazione rapida (> 2.8 rad/s)
#define IMU_SHOCK_COOLDOWN_MS  1000UL    // Cooldown anti-flooding della coda eventi

// ============================================================================
// Timer Deterministici della Macchina a Stati Finiti - FSM (Specifica Sez. 3.3)
// ============================================================================
#define TIME_ACCESA_MS         2000UL    // Durata stato ACCESA ("Hello") -> 2 secondi
#define TIME_COCCOLATA_MS      4000UL    // Durata stato COCCOLATA -> 4 secondi
#define TIME_VITTORIA_MS       4000UL    // Durata stato transitorio VITTORIA -> 4 secondi

#if DEBUG_SHORT_TIMERS
    // Timer ridotti per il test rapido a banco della FSM e della preemption
    #define TIME_INACTIVITY_ANNOIATA_MS  15000UL     // 15 secondi invece di 5 minuti
    #define TIME_CONCENTRATA_DEFAULT_MS  30000UL     // 30 secondi invece di 40 minuti
#else
    // Timer nominali da specifica tecnica
    #define TIME_INACTIVITY_ANNOIATA_MS  300000UL    // 5 minuti (300.000 ms)
    #define TIME_CONCENTRATA_DEFAULT_MS  2400000UL   // 40 minuti (2.400.000 ms)
#endif

// Retrocompatibilita' con vecchie costanti presenti nel progetto
#define DISPLAY_REFRESH_PERIOD_MS TASK_OLED_PERIOD_MS
#define BATTERY_CHECK_PERIOD_MS   5000
#define LOW_BATTERY_VOLTAGE       3.4
#define WEATHER_UPDATE_PERIOD_MS  300000

#endif