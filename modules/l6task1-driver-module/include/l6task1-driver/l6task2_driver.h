/**
 * @file
 * @brief L6 Task2 driver header file
 */

#ifndef L6TASK2_DRIVER_H
#define L6TASK2_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct Dynamic data for the L6 Task2
 */
struct l6task2_driver_data
{
    uint32_t counter_of_sample_fetch_calls;
    uint32_t counter_of_channel_get_calls;
};

/**
 * @brief L6 Task2 driver API - custom extension 1
 *        This function extends the custom API and modifies the dynamic data of a L6 Task1 driver instance.
 */
void l6task2_driver_custom_api_function_incr_sample_fetch_cnt(const struct device* dev);

/**
 * @brief L6 Task2 driver API - custom extension 2
 *        This function extends the custom API and modifies the dynamic data of a L6 Task1 driver instance.
 */
void l6task2_driver_custom_api_function_incr_channel_get_cnt(const struct device* dev);

#ifdef __cplusplus
}
#endif

#endif /* L6TASK2_DRIVER_H */