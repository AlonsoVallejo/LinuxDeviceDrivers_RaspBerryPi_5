SPI Linux Device Driver for Raspberry Pi 5: Comprehensive Guide

This example demonstrates how to create and use a kernel module to read SPI-0 on the Raspberry Pi 5. The module is designed to read the ID of the SPI RF module NRF24L01, chosen because it was the only SPI module available for this project.

---

Overview:
- **Kernel module**: A custom module interacts with SPI0 and reads the NRF24L01 RF module.
- **Device tree overlay**: "spi0-rb5-overlay.dts" enables SPI0 on the Raspberry Pi 5.
- **CS0 of SPI0**: Likely occupied by "spidev," which must be disabled in a separate fragment.

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

2.- **Load the Device Tree Overlay**
   - Add the compiled overlay (`spi0-rb5.dtbo`) to the Linux kernel:
     ```
     sudo dtoverlay spi0-rb5.dtbo
     ```
   - Verify the overlay was correctly loaded:
     ```
     dtoverlay -a | grep spi
     ```
     - Expected result: It should appear as `* spi0-rb5`.

3.- **Disable "spidev" on CS0**
   - Confirm "spidev0.0" is disabled:
     ```
     ls -l /dev/spi*
     ```
     - Note: "spidev0.0" should **not** appear.

4.- **Load the Kernel Module**
   - Insert the kernel module (`ldd_spi.ko`) into the Linux kernel:
     ```
     sudo insmod ldd_spi.ko
     ```
   - This should create a file at:
     ```
     /sys/bus/spi/drivers/spi0_nrf24_drvd
     ```

5.- **Verify Functionality**
   - Be happy! The SPI module is now operational and ready for use.

---

Notes:
- **SPI Module**: This project uses the NRF24L01 RF module for demonstrating SPI driver functionality.
- **Hardware Testing**: Verify SPI signals using appropriate tools like logic analyzers or oscilloscopes to ensure correct module interaction.
- **Attention to Details**: Ensure each step is followed carefully, particularly disabling "spidev" on CS0.

---

Save and share as needed. Let me know if there's anything else you'd like adjusted! 😊
