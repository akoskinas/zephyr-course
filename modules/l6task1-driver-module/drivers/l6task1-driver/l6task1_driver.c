#include <l6task1-driver/l6task2_driver.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(l6task1_driver, CONFIG_L6TASK1_DRIVER_LOG_LEVEL);

#define DT_DRV_COMPAT l6task1_driver

#define LED_NODE DT_ALIAS(app_led)

// With the current design, the LED is a "shared" resource for all instances of this driver
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static int l6task1_driver_init(const struct device* dev)
{
    LOG_INF("Initializing %s", dev->name);
    if (!gpio_is_ready_dt(&led))
    {
        LOG_ERR("LED GPIO device not ready!");
        return -ENODEV;
    }
    LOG_INF("LED at %s:%d is ready", led.port->name, led.pin);

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
    {
        LOG_ERR("Failed to configure LED pin!");
        return -EIO;
    }
    LOG_INF("LED at %s:%d is configured", led.port->name, led.pin);
    return 0;
}

/**
 * @brief Fetch a sample from the L6 Task1 driver
 *
 * This function simply turns on the app LED.
 *
 * @param dev Pointer to the device structure
 * @param chan Sensor channel to fetch
 *
 * @return 0 on success, negative error code on failure
 */
int l6task1_driver_sample_fetch(const struct device* dev, enum sensor_channel chan)
{
    LOG_INF("l6task1_driver_sample_fetch called for %s and channel %d", dev->name, chan);
    LOG_INF("Turning ON LED");
    int ret = gpio_pin_set_dt(&led, 1);
    if (ret < 0)
    {
        LOG_ERR("Failed to set LED ON: %d", ret);
    }
    else
    {
        LOG_INF("LED set to ON successfully");
    }
    return ret;
}

/**
 * @brief Get the channel value from the L6 Task1 driver
 *
 * This function simply turns off the app LED and populates the sensor value with 0.
 *
 * @param dev Pointer to the device structure
 * @param chan Sensor channel to get
 * @param val Pointer to the sensor value structure
 *
 * @return 0 on success, negative error code on failure
 */
int l6task1_driver_channel_get(const struct device* dev, enum sensor_channel chan, struct sensor_value* val)
{
    LOG_INF("l6task1_driver_channel_get called for %s and channel %d", dev->name, chan);
    LOG_INF("Turning OFF LED");
    int ret = gpio_pin_set_dt(&led, 0);
    if (ret < 0)
    {
        LOG_ERR("Failed to set LED OFF: %d", ret);
    }
    else
    {
        LOG_INF("LED set to OFF successfully");
    }

    // populate the sensor value struct with 0
    if (val)
    {
        val->val1 = 0;
        val->val2 = 0;
    }
    return ret;
}


static DEVICE_API(sensor, l6task1_driver_api) = {
    .sample_fetch = l6task1_driver_sample_fetch,
    .channel_get  = l6task1_driver_channel_get,
};

// implementation of the L6 Task2 driver custom API functions
void l6task2_driver_custom_api_function_incr_sample_fetch_cnt(const struct device* dev)
{
    LOG_INF("l6task2_driver_custom_api_function_incr_sample_fetch_cnt called for %s", dev->name);
    // cast the device data to the L6 Task2 driver data structure
    struct l6task2_driver_data* data = dev->data;
    if (data)
    {
        data->counter_of_sample_fetch_calls++;
        LOG_INF("Sample fetch counter incremented to %d", data->counter_of_sample_fetch_calls);
    }
    else
    {
        LOG_ERR("dev->data is NULL for %s - cannot increment counter", dev->name);
    }
}

void l6task2_driver_custom_api_function_incr_channel_get_cnt(const struct device* dev)
{
    LOG_INF("l6task2_driver_custom_api_function_incr_channel_get_cnt called for %s", dev->name);
    // cast the device data to the L6 Task2 driver data structure
    struct l6task2_driver_data* data = dev->data;
    if (data)
    {
        data->counter_of_channel_get_calls++;
        LOG_INF("Channel get counter incremented to %d", data->counter_of_channel_get_calls);
    }
    else
    {
        LOG_ERR("dev->data is NULL for %s - cannot increment counter", dev->name);
    }
}

// Ensure that the L6TASK1 driver initialization priority is greater than the GPIO initialization priority
BUILD_ASSERT(CONFIG_L6TASK1_DRIVER_INIT_PRIORITY > CONFIG_GPIO_INIT_PRIORITY,
             "L6TASK1 driver init priority must be greater than GPIO init priority");

// Helper macro to define L6 Task1 driver instances
#define L6TASK1_DRIVER_DEFINE(inst)                                                                                    \
    static struct l6task2_driver_data l6task2_data_##inst;                                                             \
    DEVICE_DT_INST_DEFINE(inst,                                                                                        \
                          l6task1_driver_init,                                                                         \
                          NULL,                                                                                        \
                          &l6task2_data_##inst,                                                                        \
                          NULL,                                                                                        \
                          POST_KERNEL,                                                                                 \
                          CONFIG_L6TASK1_DRIVER_INIT_PRIORITY,                                                         \
                          &l6task1_driver_api);

DT_INST_FOREACH_STATUS_OKAY(L6TASK1_DRIVER_DEFINE);