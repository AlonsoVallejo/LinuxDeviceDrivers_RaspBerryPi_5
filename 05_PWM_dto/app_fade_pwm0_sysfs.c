#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PWM_PATH "/sys/class/pwm/pwmchip1" /* Set the pwmchip#, after loading the device tree overlay of PWM0 */
#define PWM_EXPORT_PATH PWM_PATH "/export"
#define PWM_UNEXPORT_PATH PWM_PATH "/unexport"
#define PWM0_PATH PWM_PATH "/pwm0"
#define PWM0_PERIOD_PATH PWM0_PATH "/period"
#define PWM0_DUTY_CYCLE_PATH PWM0_PATH "/duty_cycle"
#define PWM0_ENABLE_PATH PWM0_PATH "/enable"

#define PWM0_PERIOD_NS (50000) 
#define PWM_RANGE (50000)   

void export_pwm(void);
void unexport_pwm(void);
void set_pwm0_period(__uint32_t period_ns);
void set_pwm0_duty_cycle(__uint32_t duty_cycle_ns);
void enable_pwm0(void);

void export_pwm(void) 
{
    int export_fd = open(PWM_EXPORT_PATH, O_WRONLY);
    if (export_fd == -1) {
        perror("Error opening export file\n");
        exit(EXIT_FAILURE);
    }
    write(export_fd, "0", 1);
    close(export_fd);
}

void unexport_pwm(void) 
{
    int unexport_fd = open(PWM_UNEXPORT_PATH, O_WRONLY);
    if (unexport_fd == -1) {
        perror("Error opening unexport file\n");
        exit(EXIT_FAILURE);
    }
    write(unexport_fd, "0", 1);
    close(unexport_fd);
}

void set_pwm0_period(__uint32_t period_ns) 
{
    int period_fd = open(PWM0_PERIOD_PATH, O_WRONLY);
    if (period_fd == -1) {
        perror("Error opening period file\n");
        exit(EXIT_FAILURE);
    }
    char period_str[20];
    sprintf(period_str, "%d", period_ns);
    write(period_fd, period_str, strlen(period_str));
    close(period_fd);
}

void set_pwm0_duty_cycle(__uint32_t duty_cycle_ns) 
{
    char duty_cycle_str[20];

    int duty_cycle_fd = open(PWM0_DUTY_CYCLE_PATH, O_WRONLY);
    
    if (duty_cycle_fd == -1) {
        perror("Error opening duty cycle file\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(duty_cycle_str, "%d", duty_cycle_ns);
    write(duty_cycle_fd, duty_cycle_str, strlen(duty_cycle_str));
    close(duty_cycle_fd);
}

void enable_pwm0(void) 
{
    int enable_fd = open(PWM0_ENABLE_PATH, O_WRONLY);
    if (enable_fd == -1) {
        perror("Error opening enable file\n");
        exit(EXIT_FAILURE);
    }
    write(enable_fd, "1", 1);
    close(enable_fd);
}

int main() 
{
    __uint32_t duty_cycle = 0;

    export_pwm();
    set_pwm0_period(PWM0_PERIOD_NS); 
    enable_pwm0();
    
    printf("Fading LED PWM0(GPIO12)\n");

    while (1) {
        set_pwm0_duty_cycle(duty_cycle);
        usleep(20000); 
        duty_cycle += 1000; 
        if (duty_cycle >= PWM_RANGE) {
            duty_cycle = 0; 
        }
    }

    unexport_pwm(); 
    return 0;
}
