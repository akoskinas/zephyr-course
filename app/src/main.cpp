#include <l6task1-driver/l6task2_driver.h>

#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// get a handle to the fake sensor device
static const struct device *p_my_fake_sensor = DEVICE_DT_GET(DT_NODELABEL(my_fake_sensor));


int main(void)
{
    while (1)
    {
        // turn on LED
        sensor_sample_fetch(p_my_fake_sensor);
        l6task2_driver_custom_api_function_incr_sample_fetch_cnt(p_my_fake_sensor);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD);
        // Turn off LED
        sensor_channel_get(p_my_fake_sensor, SENSOR_CHAN_ALL, NULL);
        l6task2_driver_custom_api_function_incr_channel_get_cnt(p_my_fake_sensor);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD);
    }
    return 0;
}
