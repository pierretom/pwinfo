#include <stdio.h>
#include <stdlib.h>
#include <hidapi.h>

#define MAX_STR 255

// Structure to store hours, minutes and seconds
typedef struct {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
} time;

time convert_seconds(unsigned short sec) { // Max: 18:12:15
    time time_var;

    if (sec < 60) {
        time_var.hours = 0;
        time_var.minutes = 0;
        time_var.seconds = sec;
    } else if (sec < 3600) {
        time_var.hours = 0;
        time_var.minutes = sec / 60;
        time_var.seconds = sec % 60;
    } else {
        time_var.hours = sec / 3600;
        time_var.minutes = (sec % 3600) / 60;
        time_var.seconds = sec % 60;
    }

    return time_var;
}

unsigned char get_input_report(hid_device *handle, unsigned char report_id,
                               unsigned char *buf, int *report_size) {
    buf[0] = report_id; // The first byte is the report ID.

    // Get the input report with report ID
    *report_size = hid_get_feature_report(handle, buf, sizeof(buf));
    if (report_size < 0) {
        printf("Warning: unable to get input report for report ID %hhu.\n\n", report_id);
        return 1;
    }

    return 0;
}

// Power
unsigned char get_info_report_id_24(hid_device *handle) {
    unsigned char report_id = 24;
    unsigned char buf[64]; // Buffer enough large for the report
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    // Extract data from the report
    unsigned short config_active_power = (buf[1] | (buf[2] << 8));
    printf("Output config active power:     %hu W\n", config_active_power);

    return 0;
}

// Power
unsigned char get_info_report_id_14(hid_device *handle) {
    unsigned char report_id = 14;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short config_voltage = (buf[1] | (buf[2] << 8));
    printf("Config voltage:                 %hu V\n", config_voltage);

    return 0;
}

// Power
unsigned char get_info_report_id_15(hid_device *handle) {
    unsigned char report_id = 15;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short input_voltage = (buf[1] | (buf[2] << 8)) / 10;
    printf("Input voltage:                  %hu V\n", input_voltage);

    return 0;
}

// Power
unsigned char get_info_report_id_18(hid_device *handle) {
    unsigned char report_id = 18;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short output_voltage = (buf[1] | (buf[2] << 8)) / 10;
    printf("Output voltage:                 %hu V\n", output_voltage);

    return 0;
}

// Power
unsigned char get_info_report_id_16(hid_device *handle) {
    unsigned char report_id = 16;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short low_voltage_transfert = (buf[1] | (buf[2] << 8));
    printf("Low voltage transfer:           %hu V\n", low_voltage_transfert);

    return 0;
}

// Power
unsigned char get_info_report_id_17(hid_device *handle) {
    unsigned char report_id = 17;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short high_voltage_transfert = (buf[1] | (buf[2] << 8));
    printf("High voltage transfer:          %hu V\n", high_voltage_transfert);

    return 0;
}

// Power
unsigned char get_info_report_id_19(hid_device *handle) {
    unsigned char report_id = 19;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    printf("Output percent load:            %hhu%\n", buf[1]);

    return 0;
}

// Power
unsigned char get_info_report_id_28(hid_device *handle) {
    unsigned char report_id = 28;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short input_frequency = (buf[1] | (buf[2] << 8)) / 10;
    printf("Input frequency:                %hu Hz\n", input_frequency);

    return 0;
}

// Power
unsigned char get_info_report_id_29(hid_device *handle) {
    unsigned char report_id = 29;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short output_frequency = (buf[1] | (buf[2] << 8)) / 10;
    printf("Output frequency:               %hu Hz\n", output_frequency);

    return 0;
}

// Power
unsigned char get_info_report_id_23(hid_device *handle) {
    unsigned char report_id = 23;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned char bit0 = buf[1] & 1;
    unsigned char bit1 = (buf[1] >> 1) & 1;
    unsigned char bit2 = (buf[1] >> 2) & 1;

    printf("Boost:                          %s\n", bit0 ? "yes" : "no");
    printf("Overload:                       %s\n", bit1 ? "yes" : "no");
    printf("Buck:                           %s\n", bit2 ? "yes" : "no");

    return 0;
}

// Battery
unsigned char get_info_report_id_9(hid_device *handle) {
    unsigned char report_id = 9;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short config_voltage = (buf[1] | (buf[2] << 8)) / 10;
    printf("Config voltage:                 %hu V\n", config_voltage);

    return 0;
}

// Battery
unsigned char get_info_report_id_10(hid_device *handle) {
    unsigned char report_id = 10;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned short voltage = (buf[1] | (buf[2] << 8)) / 10;
    printf("Voltage:                        %hu V\n", voltage);

    return 0;
}

// Battery
unsigned char get_info_report_id_8(hid_device *handle) {
    unsigned char report_id = 8;
    unsigned char buf[64];
    int report_size;
    unsigned short sec;
    time time_var;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned char remaining_capacity = buf[1];
    unsigned short run_time_to_empty = (buf[2] | (buf[3] << 8));
    unsigned short remaining_time_limit = (buf[4] | (buf[5] << 8));

    printf("Remaining capacity:             %hhu%%\n", remaining_capacity);

    sec = run_time_to_empty;
    time_var = convert_seconds(sec);
    printf("Run time to empty:              %02hhu:%02hu:%02hhu\n", time_var.hours, time_var.minutes, time_var.seconds);

    sec = remaining_time_limit;
    time_var = convert_seconds(sec);
    printf("Remaining time limit:           %02hhu:%02hhu:%02hhu\n", time_var.hours, time_var.minutes, time_var.seconds);

    return 0;
}

// Battery
unsigned char get_info_report_id_11(hid_device *handle) {
    unsigned char report_id = 11;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    unsigned char bit0 = buf[1] & 1;
    unsigned char bit1 = (buf[1] >> 1) & 1;
    unsigned char bit2 = (buf[1] >> 2) & 1;
    unsigned char bit3 = (buf[1] >> 3) & 1;
    unsigned char bit4 = (buf[1] >> 4) & 1;
    unsigned char bit5 = (buf[1] >> 5) & 1;

    printf("AC present:                     %s\n", bit0 ? "yes" : "no");
    printf("Charging:                       %s\n", bit1 ? "yes" : "no");
    printf("Discharging:                    %s\n", bit2 ? "yes" : "no");
    printf("Fully charged:                  %s\n", bit4 ? "yes" : "no");
    printf("Below remaining capacity limit: %s\n", bit3 ? "yes" : "no");
    printf("Remaining time limit expired:   %s\n", bit5 ? "yes" : "no");

    return 0;
}

// Battery
unsigned char get_info_report_id_7(hid_device *handle) {
    unsigned char report_id = 7;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    printf("Design capacity:                %hhu%%\n", buf[1]);
    printf("Capacity granularity 1:         %hhu%%\n", buf[2]);
    printf("Capacity granularity 2:         %hhu%%\n", buf[3]);
    printf("Warning capacity limit:         %hhu%%\n", buf[4]);
    printf("Full charge capacity:           %hhu%%\n", buf[5]);
    printf("Remaining capacity limit:       %hhu%%\n", buf[6]);

    return 0;
}

// Battery
unsigned char get_info_report_id_5(hid_device *handle) {
    unsigned char report_id = 5;
    unsigned char buf[64];
    int report_size;

    if (get_input_report(handle, report_id, buf, &report_size))
        return 1;

    printf("Rechargable:                    %s\n", buf[1] ? "yes" : "no");

    return 0;
}

unsigned char get_info_from_device(hid_device *handle) {
    int res;
    wchar_t wstr[MAX_STR];

    // General
    printf("\n");
    printf("General\n");
    printf("———————————————————————————————————————————————————————————————\n");

    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    if (res == 0)
        printf("Manufacturer:                   %ls\n", wstr);
    else
        printf("Manufacturer:                   n/a\n");

    res = hid_get_product_string(handle, wstr, MAX_STR);
    if (res == 0)
        printf("Product:                        %ls\n", wstr);
    else
        printf("Product:                        n/a\n");

    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    if (res == 0)
        printf("Serial number:                  %ls\n", wstr);
    else
        printf("Serial number:                  n/a\n");

    res = hid_get_indexed_string(handle, 5, wstr, sizeof(wstr) / sizeof(wchar_t));
    if (res == 0)
        printf("Firmware version:               %ls\n", wstr);
    else
        printf("Firmware version:               n/a\n");

    res = hid_get_indexed_string(handle, 6, wstr, sizeof(wstr) / sizeof(wchar_t));
    if (res == 0)
        printf("Misc 1:                         %ls\n", wstr);
    else
        printf("Misc 1:                         n/a\n");

    // Power
    printf("\n");
    printf("Power\n");
    printf("———————————————————————————————————————————————————————————————\n");
    get_info_report_id_24(handle);
    get_info_report_id_14(handle);
    get_info_report_id_15(handle);
    get_info_report_id_18(handle);
    get_info_report_id_16(handle);
    get_info_report_id_17(handle);
    get_info_report_id_19(handle);
    get_info_report_id_28(handle);
    get_info_report_id_29(handle);
    get_info_report_id_23(handle);

    // Battery
    printf("\n");
    printf("Battery\n");
    printf("———————————————————————————————————————————————————————————————\n");
    get_info_report_id_9(handle);
    get_info_report_id_10(handle);
    get_info_report_id_8(handle);
    get_info_report_id_11(handle);
    get_info_report_id_7(handle);

    res = hid_get_indexed_string(handle, 4, wstr, sizeof(wstr) / sizeof(wchar_t));
    if (res == 0)
        printf("Battery’s chemistry:            %ls\n", wstr);
    else
        printf("Battery’s chemistry:            n/a\n");

    get_info_report_id_5(handle);
    printf("\n");

    // Close the device
    hid_close(handle);

    // Quit HIDAPI
    if (hid_exit() < 0) {
        fprintf(stderr, "HIDAPI exit failed.\n");
        return 1;
    }

    return 0;
}

unsigned char init_hidapi() {
    int res;

    res = hid_init();
    if (res < 0) {
        fprintf(stderr, "HIDAPI init failed.\n");
        return 1;
    }

    return 0;
}

unsigned char get_info_from_vpid(unsigned short pid, unsigned short vid) {
    int res;
    hid_device *handle;

    // Initialize HIDAPI
    if (init_hidapi())
        return 1;

    // Open the device with Product ID and Vendor ID
    handle = hid_open(pid, vid, NULL);
    if (!handle) {
        printf("Unable to open device with PID:VID: %04x:%04x.\n", pid, vid);
        hid_exit();
        return 1;
    }

    get_info_from_device(handle);

    return 0;
}

unsigned char get_info_from_path(const char* path) {
    hid_device *handle;

    // Initialize HIDAPI
    if (init_hidapi())
        return 1;

    // Open the device with the path
    handle = hid_open_path(path);
    if (!handle) {
        printf("Unable to open device with path %s.\n", path);
        hid_exit();
        return 1;
    }

    get_info_from_device(handle);

    return 0;
}
