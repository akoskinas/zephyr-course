# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository uses Zephyr 4.2.0 and
Zephyr SDK 0.17.4. It includes a west manifest for fetching the required
Zephyr dependencies.

## Manifest Setup

Create a virtual environment.
Install west and initialize or update the workspace from the workspace root:

```sh
pip install west
west update
```

The manifest pins Zephyr to `v4.2.0` and places it, together with the selected
modules, under `deps/`.

## Zephyr SDK

Download and extract [Zephyr SDK 0.17.4](https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v0.17.4), then run:

```sh
cd zephyr-sdk-0.17.4
./setup.sh
```

Build the application with the native simulator:

```sh
west build -p always -b native_sim zephyr-course/app
```

then flash the application

```sh
west flash
```

After that, the following boot head should be shown:

![Successful boot header with OS version](assets/screenshots/boot_header_indicating_success.png)

---

## Custom boards

### Copy/Rename method

To complete the assignment 1 of the module 5, a custom board is created using the "Copy/Rename" method.
In my case, the custom board will be created based on the FRDM-RW612.
The custom board will be named as "MOD5-ASSGNMNT1".
To build the `app` for this method use

```sh
west build -b mod5_assgnmnt1 -p app
```

and to flash simply use

```sh
west flash
```

Remark: for other boards such as the the FRDM-MCXA159, the flash operation requires `LinkServer`. Therefore, first install it and then ensure that is in the PATH.
As an example, if the `LinkServer` executable is in `/usr/local/LinkServer`, then:

```sh
export PATH=$PATH:/usr/local/LinkServer
```

## Further resources

- [Getting Started Guide - Zephyr 4.2.0 documentation](https://docs.zephyrproject.org/4.2.0/develop/getting_started/index.html)
