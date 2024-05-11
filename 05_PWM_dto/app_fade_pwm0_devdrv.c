#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DRV_PWM_DEVICE_PATH ("/dev/ldd_pwm0_device")
#define PWM_MAX_DUTTY_CYCLE (100)   /* value of dutty cycle in % */

/* Send values from 0 to 100 */
void set_pwm0_duty_cycle(__uint32_t duty_cycle) 
{
    char duty_cycle_str[20];

    int duty_cycle_fd = open(DRV_PWM_DEVICE_PATH, O_WRONLY);
    
    if (duty_cycle_fd == -1) {
        perror("Error opening duty cycle file\n");
        exit(EXIT_FAILURE);
    }

    sprintf(duty_cycle_str, "%d", duty_cycle);
    write(duty_cycle_fd, duty_cycle_str, sizeof(duty_cycle_str));
    close(duty_cycle_fd);
}

int main() 
{
    __uint8_t duty_cycle = 0;

    printf("Fading LED PWM0(GPIO12)\n");
    while (1) {
        set_pwm0_duty_cycle(duty_cycle);
        usleep(50000); 
        duty_cycle += 2; 
        if (duty_cycle >= PWM_MAX_DUTTY_CYCLE) {
            duty_cycle = 0; 
        }
    }

    return 0;
}
