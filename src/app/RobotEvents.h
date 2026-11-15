#ifndef ROBOT_EVENTS_H
#define ROBOT_EVENTS_H

#include <stdint.h>

/**
 * @brief Categorie di eventi scambiati attraverso la coda FreeRTOS robotEventQueue.
 */
typedef enum : uint8_t {
    EVENT_TOUCH,
    EVENT_MOTION,
    EVENT_FOCUS,
    EVENT_NETWORK,
    EVENT_CALENDAR
} RobotEventType;

/**
 * @brief Struttura evento inviata dai task produttori (tau_1, tau_2, tau_6)
 *        alla coda della FSM centrale (tau_4).
 */
typedef struct {
    RobotEventType type;
    uint8_t id;
} RobotEvent;

/**
 * @brief Sotto-eventi generati dal sensore touch TTP223 (Specifica Sez. 3.2.A).
 */
typedef enum : uint8_t {
    TOUCH_EVENT_SINGLE,   // EV_TAP_1: 1 singolo tap (50ms <= t_on < 400ms)
    TOUCH_EVENT_DOUBLE,   // EV_TAP_2: 2 tap consecutivi
    TOUCH_EVENT_TRIPLE,   // EV_TAP_3: 3 tap consecutivi
    TOUCH_EVENT_LONG      // EV_PETTING: Carezza prolungata (t_on > 800ms)
} TouchEvent;

/**
 * @brief Sotto-eventi generati dall'IMU MPU6050 (Specifica Sez. 3.3 e 5.1).
 */
typedef enum : uint8_t {
    MOTION_EVENT_EDGE_DETECTED,
    MOTION_EVENT_SHAKE,   // EV_SHOCK_LIFT: Sollevamento o sballottamento -> ARRABBIATA
    MOTION_EVENT_FALL
} MotionEvent;

/**
 * @brief Sotto-eventi sessione di concentrazione.
 */
typedef enum : uint8_t {
    FOCUS_EVENT_STARTED,
    FOCUS_EVENT_FINISHED
} FocusEvent;

/**
 * @brief Sotto-eventi calendario (Specifica Sez. 3.3).
 */
typedef enum : uint8_t {
    CALENDAR_EVENT_REMINDER // EV_CALENDAR_ALERT: Impegno imminente (-10 min)
} CalendarEvent;

/**
 * @brief Sotto-eventi connettivita' PC/GUI.
 */
typedef enum : uint8_t {
    NETWORK_EVENT_PC_CONNECTED,
    NETWORK_EVENT_PC_DISCONNECTED
} NetworkEvent;

#endif