#!/bin/bash
CHANNEL=0
CHIP=pwmchip6
PWM=pwm$CHANNEL

sudo echo $CHANNEL > /sys/class/pwm/$CHIP/export
sleep 1
sudo echo 240000 > /sys/class/pwm/$CHIP/$PWM/period
sudo echo 120000 > /sys/class/pwm/$CHIP/$PWM/duty_cycle
sudo echo 1 > /sys/class/pwm/$CHIP/$PWM/enable