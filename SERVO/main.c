int main(void) {
    /* Servo structs */
    TM_SERVO_t Servo1;

    /* Initialize system */
    SystemInit();

    /* Initialize delay */
    TM_DELAY_Init();

    /* Initialize servo 1, TIM2, Channel 1, Pinspack 2 = PA5 */
    TM_SERVO_Init(&Servo1, TIM2, TM_PWM_Channel_1, TM_PWM_PinsPack_2);

    while (1) {
        /* 0 degrees rotation on servo 1 */
        TM_SERVO_SetDegrees(&Servo1, 0);
        /* 2s delay */
        Delayms(2000);
        /* 90 degrees rotation */
        TM_SERVO_SetDegrees(&Servo1, 90);
        /* 2s delay */
        Delayms(2000);
        /* 180 degrees rotation */
        TM_SERVO_SetDegrees(&Servo1, 180);
        /* 2s delay */
        Delayms(2000);
    }
}
