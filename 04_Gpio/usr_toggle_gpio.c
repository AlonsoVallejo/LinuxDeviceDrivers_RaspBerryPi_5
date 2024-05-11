#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define DRV_DEVICE_PATH "/dev/ldd_gpio_device"

volatile sig_atomic_t stop_app = 0;

void sigint_handler(int sig) {
    /* stop the app by SIGINT after type CTRL+C */
    stop_app = 1;
}

int main() {
    int fd;
    char gpio_state[2];

    fd = open(DRV_DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open the device file");
        return 1;
    }

    while (!stop_app) {
        printf("Toggling GPIO...\n");

        // Set GPIO high
        gpio_state[0] = '1';
        write(fd, gpio_state, strlen(gpio_state));
        sleep(1); 

        // Set GPIO low
        gpio_state[0] = '0';
        write(fd, gpio_state, strlen(gpio_state));
        sleep(1); // 500ms
    }

    // Close device file
    close(fd);

    return 0;
}
