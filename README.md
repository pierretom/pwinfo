# pwinfo

Show information from PowerWalker UPS devices that support HID.

Basic VI 850 SB currently supported, can work with similar models.

No warranty, use it at your own risk.

## Building

#### Dependencies

* make
* gcc (for clang use `CC = clang` in Makefile)
* libhidapi-dev

#### Compile

```
$ cd pwinfo
$ make
```

## Usage

```
Usage:
  pwinfo [OPTION]

Options:
  -l, --list             List all HID devices.
  -i, --vpid VID:PID     Use Vendor ID and Product ID to get info from device.
  -p, --path PATH        Use PATH (see --list option) to get info from device.
  -d, --description      Show the description of usage names.
  -h, --help             Show help.
```

#### Example

Get the Vendor ID and Product ID of the UPS device:

```
$ lsusb
or
$ ./pwinfo --list
```

Get information from the device:

```
# ./pwinfo --vpid 0764:0601
```

#### Running without sudo

This program requires root privileges to open the device.

You can make an udev rule to avoid to use sudo, example:

* Create `/etc/udev/rules.d/99-ups.rules` and add this line:

```
SUBSYSTEM=="usb", ATTRS{idVendor}=="0764", ATTRS{idProduct}=="0601", MODE="0666"
```

* Then reload rules:

```
# udevadm control --reload-rules
# udevadm trigger
```

If it does not work, disconnect and reconnect the device.
