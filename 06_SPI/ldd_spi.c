#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/spi/spi.h>
#include "NRF24L01.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vallejo");
MODULE_DESCRIPTION("SPI driver for nrf24l01");

// SPI driver data structure
struct spi_nrf24l01 {
    struct spi_device *spi;
};

/* Bus driver functions */
static int spi_nrf24l01_read(struct spi_device *spi);
static int spi_nrf24l01_probe(struct spi_device *spi);
static void spi_nrf24l01_remove(struct spi_device *spi);

/* setup device tree/open firmware table for compatible matching 
 * to load this module when device tree overlay is initailized */
static const struct of_device_id spi_nrf24l01_dt_ids[] = {
    { .compatible = "spi0_nrf24l01" }, /* this shall match with indicated in DT */
    { },
};
MODULE_DEVICE_TABLE(of, spi_nrf24l01_dt_ids);

/* setup device tree/open firmware table for compatible matching 
 * to load this module when device tree overlay is initailized */
static const struct spi_device_id spi_nrf24l01_id[] = {
    { "spi0_nrf24_drv", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, spi_nrf24l01_id);

/* setup platform functions (probe and remove) and match table */
static struct spi_driver spi_nrf24l01_driver = {
    .probe = spi_nrf24l01_probe,
    .remove = spi_nrf24l01_remove,
    .id_table = spi_nrf24l01_id,
    .driver = {
        .name = "spi0_nrf24_drv",
        .of_match_table = of_match_ptr(spi_nrf24l01_dt_ids),
        .owner = THIS_MODULE,
    },
};

// Read function
static int spi_nrf24l01_read(struct spi_device *spi)
{
    uint8_t tx_buf[2] = {0xFF, REG_RX_ADDR_P0}; // set command to read the SPI device
    uint8_t rx_buf[2];
    int ret;

    struct spi_transfer transfer = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = sizeof(tx_buf),
    };
    struct spi_message message;

    // Initialize SPI message
    spi_message_init(&message);
    spi_message_add_tail(&transfer, &message);

    // Perform SPI transfer
    ret = spi_sync(spi, &message);
    if (ret < 0) {
        pr_err("ldd_spi0: SPI transfer error: %d\n", ret);
        return ret;
    }

    // Print SPI device ID
    pr_info("ldd_spi0: SPI device ID: 0x%02x\n", rx_buf[1]); // Assuming STATUS register is second byte in rx_buf

    return 0;
}

// Probe function
static int spi_nrf24l01_probe(struct spi_device *spi)
{
    struct spi_nrf24l01 *nrf24l01;

    // Create SPI device
    nrf24l01 = devm_kzalloc(&spi->dev, sizeof(*nrf24l01), GFP_KERNEL);
    if (!nrf24l01) {
        pr_err("ldd_spi0: Failed to allocate memory\n");
        return -ENOMEM;
    }

    nrf24l01->spi = spi;

    // Read SPI device ID
    spi_nrf24l01_read(spi);

    pr_info("ldd_spi0: Installed\n");

    return 0;
}

// Remove function
static void spi_nrf24l01_remove(struct spi_device *spi)
{
    pr_info("ldd_spi0: Removed\n");
}

module_spi_driver(spi_nrf24l01_driver);

