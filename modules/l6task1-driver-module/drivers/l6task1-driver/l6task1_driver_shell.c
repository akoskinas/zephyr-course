#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

/**
 * @brief Fetch command handler for the custom shell commands
 *
 */
static int cmd_fetch(const struct shell *shell, size_t argc, char **argv)
{
    shell_info(shell, "Fetch command called with %zu arguments", argc);
    for (size_t i = 0; i < argc; i++) {
        shell_info(shell, "Argument %zu: %s", i, argv[i]);
    }
    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device not found");
        return -ENODEV;
    }
    int res = sensor_sample_fetch(dev);
    if (res) {
        shell_error(shell, "Failed to fetch sensor sample: %d", res);
        return res;
    }
    else {
        shell_info(shell, "Sensor sample fetched successfully");
    }
    return 0;
}

/**
 * @brief Read command handler for the custom shell commands
 *
 */
static int cmd_read(const struct shell *shell, size_t argc, char **argv)
{
    shell_info(shell, "Read command called with %zu arguments", argc);
    for (size_t i = 0; i < argc; i++) {
        shell_info(shell, "Argument %zu: %s", i, argv[i]);
    }
    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device not found");
        return -ENODEV;
    }
    struct sensor_value val;
    int res = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if (res) {
        shell_error(shell, "Failed to read sensor sample: %d", res);
        return res;
    } else {
        shell_info(shell, "Sensor sample read successfully and has value: %d.%06d", val.val1, val.val2);
    }
    return 0;
}

/**
 * @brief Info command handler for the custom shell commands
 *
 */
static int cmd_info(const struct shell *shell, size_t argc, char **argv)
{
    shell_info(shell, "Info command called with %zu arguments", argc);
    for (size_t i = 0; i < argc; i++) {
        shell_info(shell, "Argument %zu: %s", i, argv[i]);
    }
    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device not found");
        return -ENODEV;
    }
    shell_info(shell, "Device name: %s", dev->name);
    shell_info(shell, "Device is %s", device_is_ready(dev) ? "ready" : "not ready");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_l7task1,
    SHELL_CMD(fetch, NULL, "Fetch sensor sample", cmd_fetch),
    SHELL_CMD(read, NULL, "Read sensor sample", cmd_read),
    SHELL_CMD(info, NULL, "Print device name and ready state", cmd_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(l7_custom_shell_cmds, &sub_l7task1, "Custom driver commands for L7", NULL);