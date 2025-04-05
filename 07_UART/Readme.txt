Serial Port Linux Device Driver for Raspberry Pi 5: Comprehensive Guide

This example demonstrates how to create and use a kernel module to read UART0 on the Raspberry Pi 5. The module is designed to enable UART0 ports TX (GPIO14) and RX (GPIO15) using a device tree overlay. It was tested with the HC05 Bluetooth device connected to the Raspberry Pi for data transmission.

---

Overview:
- **Kernel module**: Custom module interacts with UART0.
- **Device tree overlay**: "uart0-rb5-overlay.dts" enables UART0 ports TX (GPIO14) and RX (GPIO15).
- **Testing Hardware**: HC05 Bluetooth device used for verification.

---

Steps to Test:

1.- **Compile Required Files**
   - Use the local Makefile to compile the kernel module and application code:
     ```
     make
     ```
   - Compile the device tree overlay:
     ```
     make dto
     ```

2.- **Copy the Device Tree Overlay**
   - Move the compiled device tree overlay (`uart0-rb5.dtbo`) to Linux overlays:
     ```
     sudo cp uart0-rb5.dtbo /boot/firmware/overlays/
     ```

3.- **Edit Configuration**
   - Open and edit `/boot/firmware/config.txt` to add the following line:
     ```
     dtoverlay=uart0-rb5
     ```

4.- **Enable Serial Port in Raspberry Pi Configuration**
   - Go to: StartLogo → Preferences → Raspberry Pi Configuration → Interfaces → Serial Port → Enable.

5.- **Reboot the Raspberry Pi**
   - Restart your Raspberry Pi to apply changes.

6.- **Open a Serial Terminal**
   - Open a serial terminal and connect it to the Raspberry Pi's serial port.

7.- **Load the Kernel Module**
   - Insert the kernel module (`ldd_uart.ko`) into the Linux kernel:
     ```
     sudo insmod ldd_uart.ko
     ```
   - The external terminal should display: `"Send data: "`.

8.- **Test Data Transmission**
   - Type anything into the serial terminal connected to the Raspberry Pi.

9.- **Verify Received Data**
   - Use the following command on the Raspberry Pi to view received data:
     ```
     dmesg
     ```
   - The data from the serial terminal should be displayed.

10.- **Enjoy Your Achievement**
   - Be happy! Your serial port driver is now operational.

---

Notes:
- **Testing Device**: This project was tested using the HC05 Bluetooth device.
- **Troubleshooting**: If you encounter issues, double-check connections, configurations, and logs using `dmesg`.
- **Hardware Testing**: Use appropriate serial communication tools to verify functionality.

