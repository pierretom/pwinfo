#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getuid()
#include <hidapi.h>

unsigned char get_usage_page(const char* path) {
    int res;
    hid_device *handle;
    struct hid_device_info *new_dev;
    unsigned short usage_page;

    // Initialize HIDAPI
    res = hid_init();
    if (res < 0) {
        printf("HIDAPI init failed.\n");
        return 1;
    }

    // Open the device with the path
    handle = hid_open_path(path);
    if (!handle) {
        printf("Unable to open device with path %s.\n", path);
        hid_exit();
        return 1;
    }

    new_dev = hid_get_device_info(handle);
    if (!new_dev) {
        printf("can't get info from device\n");
        hid_close(handle);
        hid_exit();
        return 1;
    }

    usage_page = new_dev->usage_page;
    printf("  Usage page:       ");

    if (usage_page == 0x00) printf("Undefined\n");
    else if (usage_page == 0x01) printf("Generic Desktop Page (%04X)\n", usage_page);
    else if (usage_page == 0x02) printf("Simulation Controls Page (%04X)\n", usage_page);
    else if (usage_page == 0x03) printf("VR Controls Page (%04X)\n", usage_page);
    else if (usage_page == 0x04) printf("Sport Controls Page (%04X)\n", usage_page);
    else if (usage_page == 0x05) printf("Game Controls Page (%04X)\n", usage_page);
    else if (usage_page == 0x06) printf("Generic Device Controls Page (%04X)\n", usage_page);
    else if (usage_page == 0x07) printf("Keyboard/Keypad Page (%04X)\n", usage_page);
    else if (usage_page == 0x08) printf("LED Page (%04X)\n", usage_page);
    else if (usage_page == 0x09) printf("Button Page (%04X)\n", usage_page);
    else if (usage_page == 0x0A) printf("Ordinal Page (%04X)\n", usage_page);
    else if (usage_page == 0x0B) printf("Telephony Device Page (%04X)\n", usage_page);
    else if (usage_page == 0x0C) printf("Consumer Page (%04X)\n", usage_page);
    else if (usage_page == 0x0D) printf("Digitizers Page (%04X)\n", usage_page);
    else if (usage_page == 0x0E) printf("Haptics Page (%04X)\n", usage_page);
    else if (usage_page == 0x0F) printf("Physical Input Device Page (%04X)\n", usage_page);
    else if (usage_page == 0x10) printf("Unicode Page (%04X)\n", usage_page);
    else if (usage_page == 0x11) printf("SoC Page (%04X)\n", usage_page);
    else if (usage_page == 0x12) printf("Eye and Head Trackers Page (%04X)\n", usage_page);
    else if (usage_page >= 0x13 && usage_page <= 0x13) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x14) printf("Auxiliary Display Page (%04X)\n", usage_page);
    else if (usage_page >= 0x15 && usage_page <= 0x1F) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x20) printf("Sensors Page (%04X)\n", usage_page);
    else if (usage_page >= 0x21 && usage_page <= 0x3F) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x40) printf("Medical Instrument Page (%04X)\n", usage_page);
    else if (usage_page == 0x41) printf("Braille Display Page (%04X)\n", usage_page);
    else if (usage_page >= 0x42 && usage_page <= 0x58) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x59) printf("Lighting And Illumination Page (%04X)\n", usage_page);
    else if (usage_page >= 0x5A && usage_page <= 0x7F) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x80) printf("Monitor Page (%04X)\n", usage_page);
    else if (usage_page == 0x81) printf("Monitor Enumerated Page (%04X)\n", usage_page);
    else if (usage_page == 0x82) printf("VESA Virtual Controls Page (%04X)\n", usage_page);
    else if (usage_page >= 0x83 && usage_page <= 0x83) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x84) printf("Power Page (%04X)\n", usage_page);
    else if (usage_page == 0x85) printf("Battery System Page (%04X)\n", usage_page);
    else if (usage_page >= 0x86 && usage_page <= 0x8B) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x8C) printf("Barcode Scanner Page (%04X)\n", usage_page);
    else if (usage_page == 0x8D) printf("Scales Page (%04X)\n", usage_page);
    else if (usage_page == 0x8E) printf("Magnetic Stripe Reader Page (%04X)\n", usage_page);
    else if (usage_page >= 0x8F && usage_page <= 0x8F) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0x90) printf("Camera Control Page (%04X)\n", usage_page);
    else if (usage_page == 0x91) printf("Arcade Page (%04X)\n", usage_page);
    else if (usage_page == 0x92) printf("Gaming Device Page (%04X)\n", usage_page);
    else if (usage_page >= 0x93 && usage_page <= 0xF1CF) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page == 0xF1D0) printf("FIDO Alliance Page (%04X)\n", usage_page);
    else if (usage_page >= 0xF1D1 && usage_page <= 0xFEFF) printf("Reserved (%04X)\n", usage_page);
    else if (usage_page >= 0xFF00 && usage_page <= 0xFFFF) printf("Vendor-defined (%04X)\n", usage_page);
    else printf("Unknown\n"); // Catch any other values

    if (usage_page == 0x84 && new_dev->usage == 0x04)
        printf("  Usage:            UPS (%04X)\n", new_dev->usage);

    // Close the device
    hid_close(handle);

    return 0;
}

unsigned char list_all_hid_devices() {
    int device_count = 0;
    int device_index = 1;

    // Initialize HIDAPI
    if (hid_init() < 0) {
        fprintf(stderr, "HIDAPI init failed.\n");
        return 1;
    }

    // Get the HID devices list
    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(0x0, 0x0); // 0x0, all HID devices
    cur_dev = devs;

    while (cur_dev) {
        device_count++;
        cur_dev = cur_dev->next;
    }

    printf("HIDAPI version: %s\n", hid_version_str());
    printf("Number of HID devices found: %d\n\n", device_count);

    cur_dev = devs; // Reset pointer to zero to browse list again

    while (cur_dev) {
        printf("Device found #%d:\n", device_index);
        printf("  Manufacturer:     %ls\n  Product:          %ls\n", cur_dev->manufacturer_string, cur_dev->product_string);

        if (getuid() == 0)
            get_usage_page(cur_dev->path);

        printf("  Vendor ID:        %04hx\n  Product ID:       %04hx\n", cur_dev->vendor_id, cur_dev->product_id);
        printf("  Path:             %s\n", cur_dev->path); // Path location: /sys/bus/<bus_type>/devices

        switch (cur_dev->bus_type) {
            case HID_API_BUS_USB:
                printf("  Bus type:         USB\n");
                break;
            case HID_API_BUS_BLUETOOTH:
                printf("  Bus type:         Bluetooth\n");
                break;
            case HID_API_BUS_I2C:
                printf("  Bus type:         I2C\n");
                break;
            case HID_API_BUS_SPI:
                printf("  Bus type:         SPI\n");
                break;
            case HID_API_BUS_UNKNOWN:
            default:
                printf("  Bus type:         Unknown\n");
                break;
        }

        printf("  Serial number:    %ls\n  Firmware version: %hx\n\n", cur_dev->serial_number, cur_dev->release_number);

        cur_dev = cur_dev->next;
        device_index++;
    }

    if (getuid() != 0)
        printf("You can run the program with root privileges to get usage pages.\n");

    // Free devices list
    hid_free_enumeration(devs);

    // Quit HIDAPI
    if (hid_exit() < 0) {
        fprintf(stderr, "HIDAPI exit failed.\n");
        return 1;
    }

    return 0;
}
