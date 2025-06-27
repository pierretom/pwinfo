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
        fprintf(stderr, "HIDAPI init failed.\n");
        return 1;
    }

    // Open the device with the path
    handle = hid_open_path(path);
    if (!handle) {
        fprintf(stderr, "Unable to open device with path %s.\n", path);
        hid_exit();
        return 1;
    }

    new_dev = hid_get_device_info(handle);
    if (!new_dev) {
        fprintf(stderr,"Can't get info from device.\n");
        hid_close(handle);
        hid_exit();
        return 1;
    }

    usage_page = new_dev->usage_page;
    printf("  Usage page:            ");

    if (usage_page == 0x00) printf("Undefined\n");
    else if (usage_page == 0x01) printf("Generic Desktop Page (%04hX)\n", usage_page);
    else if (usage_page == 0x02) printf("Simulation Controls Page (%04hX)\n", usage_page);
    else if (usage_page == 0x03) printf("VR Controls Page (%04hX)\n", usage_page);
    else if (usage_page == 0x04) printf("Sport Controls Page (%04hX)\n", usage_page);
    else if (usage_page == 0x05) printf("Game Controls Page (%04hX)\n", usage_page);
    else if (usage_page == 0x06) printf("Generic Device Controls Page (%04hX)\n", usage_page);
    else if (usage_page == 0x07) printf("Keyboard/Keypad Page (%04hX)\n", usage_page);
    else if (usage_page == 0x08) printf("LED Page (%04hX)\n", usage_page);
    else if (usage_page == 0x09) printf("Button Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0A) printf("Ordinal Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0B) printf("Telephony Device Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0C) printf("Consumer Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0D) printf("Digitizers Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0E) printf("Haptics Page (%04hX)\n", usage_page);
    else if (usage_page == 0x0F) printf("Physical Input Device Page (%04hX)\n", usage_page);
    else if (usage_page == 0x10) printf("Unicode Page (%04hX)\n", usage_page);
    else if (usage_page == 0x11) printf("SoC Page (%04hX)\n", usage_page);
    else if (usage_page == 0x12) printf("Eye and Head Trackers Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x13 && usage_page <= 0x13) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x14) printf("Auxiliary Display Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x15 && usage_page <= 0x1F) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x20) printf("Sensors Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x21 && usage_page <= 0x3F) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x40) printf("Medical Instrument Page (%04hX)\n", usage_page);
    else if (usage_page == 0x41) printf("Braille Display Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x42 && usage_page <= 0x58) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x59) printf("Lighting And Illumination Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x5A && usage_page <= 0x7F) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x80) printf("Monitor Page (%04hX)\n", usage_page);
    else if (usage_page == 0x81) printf("Monitor Enumerated Page (%04hX)\n", usage_page);
    else if (usage_page == 0x82) printf("VESA Virtual Controls Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x83 && usage_page <= 0x83) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x84) printf("Power Page (%04hX)\n", usage_page);
    else if (usage_page == 0x85) printf("Battery System Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x86 && usage_page <= 0x8B) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x8C) printf("Barcode Scanner Page (%04hX)\n", usage_page);
    else if (usage_page == 0x8D) printf("Scales Page (%04hX)\n", usage_page);
    else if (usage_page == 0x8E) printf("Magnetic Stripe Reader Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x8F && usage_page <= 0x8F) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0x90) printf("Camera Control Page (%04hX)\n", usage_page);
    else if (usage_page == 0x91) printf("Arcade Page (%04hX)\n", usage_page);
    else if (usage_page == 0x92) printf("Gaming Device Page (%04hX)\n", usage_page);
    else if (usage_page >= 0x93 && usage_page <= 0xF1CF) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page == 0xF1D0) printf("FIDO Alliance Page (%04hX)\n", usage_page);
    else if (usage_page >= 0xF1D1 && usage_page <= 0xFEFF) printf("Reserved (%04hX)\n", usage_page);
    else if (usage_page >= 0xFF00 && usage_page <= 0xFFFF) printf("Vendor-defined (%04hX)\n", usage_page);
    else printf("Unknown\n"); // Catch any other values

    if (usage_page == 0x84 && new_dev->usage == 0x04)
        printf("  Usage:                 UPS (%04hX)\n", new_dev->usage);

    // Close the device
    hid_close(handle);

    return 0;
}

unsigned char list_all_hid_devices() {
    unsigned short device_count = 0;
    unsigned short device_index = 1;

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

    printf(
        "HIDAPI version: %s\n"
        "Number of HID devices found: %hu\n\n"
        , hid_version_str(), device_count
    );

    cur_dev = devs; // Reset pointer to zero to browse list again

    while (cur_dev) {
        printf(
            "Device found #%hu:\n"
            "  Manufacturer:          %ls\n"
            "  Product:               %ls\n"
            , device_index, cur_dev->manufacturer_string, cur_dev->product_string
        );

        if (getuid() == 0)
            get_usage_page(cur_dev->path);

        printf(
            "  Vendor and Product ID: %04hX:%04hX\n"
            "  Path:                  %s\n" // Path location: /sys/bus/<bus_type>/devices
            , cur_dev->vendor_id, cur_dev->product_id, cur_dev->path
        );

        switch (cur_dev->bus_type) {
            case HID_API_BUS_USB:
                printf("  Bus type:              USB\n");
                break;
            case HID_API_BUS_BLUETOOTH:
                printf("  Bus type:              Bluetooth\n");
                break;
            case HID_API_BUS_I2C:
                printf("  Bus type:              I2C\n");
                break;
            case HID_API_BUS_SPI:
                printf("  Bus type:              SPI\n");
                break;
            case HID_API_BUS_UNKNOWN:
            default:
                printf("  Bus type:              Unknown\n");
                break;
        }

        printf(
            "  Interface number:      %d\n"
            "  Serial number:         %ls\n"
            "  Device version number: %hu.%02hu\n\n"
            , cur_dev->interface_number, cur_dev->serial_number
            , cur_dev->release_number >> 8, cur_dev->release_number & 0xFF
        );

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
