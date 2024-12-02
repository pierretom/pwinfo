#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h> // strcmp()
#include "list_hid.h"
#include "device_info.h"

const char* const short_options = "li:p:dh";

const struct option long_options[] = {
    {"list",        no_argument,       0, 'l'},
    {"vpid",        required_argument, 0, 'i'},
    {"path",        required_argument, 0, 'p'},
    {"description", no_argument,       0, 'd'},
    {"help",        no_argument,       0, 'h'},
    {0,             0,                 0,  0 }
};

void show_help(const char* program_name) {
    printf("Usage:\n  %s [OPTION]\n\n", program_name);
    printf("Show information from PowerWalker UPS devices that support HID.\n");
    printf("Basic VI 850 SB currently supported, can work with similar models.\n");
    printf("No warranty, use it at your own risk.\n\n");
    printf("Options:\n");
    printf("  -l, --list             List all HID devices.\n");
    printf("  -i, --vpid VID:PID     Use Vendor ID and Product ID to get info from device.\n");
    printf("  -p, --path PATH        Use PATH (see --list option) to get info from device.\n");
    printf("  -d, --description      Show the description of usage names.\n");
    printf("  -h, --help             Show help.\n");
}

void show_description() {
    printf("\n");
    printf("General\n");
    printf("———————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("Manufacturer:\n    Describe the manufacturer.\n");
    printf("Product:\n    Describe the product.\n");
    printf("Serial number:\n    Describe the device’s serial number.\n");
    printf("Firmware version:\n    Vendor-defined - Version of the firmware.\n");
    printf("Misc 1:\n    Vendor-defined - Unknown indexed string.\n");

    printf("\n");
    printf("Power\n");
    printf("———————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("Output config active power:\n    Nominal value of the output active (RMS) power.\n");
    printf("Config voltage:\n    Nominal value of the voltage.\n");
    printf("Input voltage:\n    Actual value of the input voltage.\n");
    printf("Output voltage:\n    Actual value of the output voltage.\n");
    printf("Low voltage transfer:\n    The minimum line voltage allowed before the PS system transfers to battery backup.\n");
    printf("High voltage transfer:\n    The maximum line voltage allowed before the PS system transfers to battery backup.\n");
    printf("Output percent load:\n    The actual value of the percentage of the power capacity presently being used\n");
    printf("    on this output line, i.e., the greater of the percent load of true power\n");
    printf("    capacity and the percent load of apparent power.\n");
    printf("Input frequency:\n    Actual value of the input frequency.\n");
    printf("Output frequency:\n    Actual value of the output frequency.\n");
    printf("Boost:\n    Boosted/Not Boosted.\n");
    printf("    The status indicates this input is used in the module but voltage is increased\n");
    printf("    to fit within nominal range values.\n");
    printf("Overload:\n    Overloaded/Not Overloaded.\n");
    printf("Buck:\n    Bucked/Not Bucked.\n");
    printf("    The status indicates this input is used in the module but voltage is reduced\n");
    printf("    to fit with nominal range values.\n");

    printf("\n");
    printf("Battery\n");
    printf("———————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("Config Voltage:\n    Nominal value of the voltage.\n");
    printf("Voltage:\n    Actual value of the voltage.\n");
    printf("Remaining capacity:\n    The predicted remaining capacity.\n");
    printf("Run time to empty:\n    The predicted remaining battery life, at the present rate of discharge.\n");
    printf("Remaining time limit:\n    Sets the value of the battery’s remaining time, which causes the RemainingTimeLimit\n");
    printf("    control to be activated. Whenever the battery’s remaining time falls below the\n");
    printf("    value in the RemainingTimeLimit register, the battery periodically issues a\n");
    printf("    RemainingTimeLimitExpired alarm.\n");
    printf("AC present:\n    Alternating current present/not present.\n");
    printf("Charging:\n    Battery is charging.\n");
    printf("Discharging:\n    Battery is discharing.\n");
    printf("Fully charged:\n    Battery is fully-charged.\n");
    printf("Below remaining capacity limit:\n    Is below?.\n");
    printf("Remaining time limit expired:\n    Has expired?.\n");
    printf("Design capacity:\n    The theoretical capacity of a new pack.\n");
    printf("Capacity granularity 1:\n    Battery capacity granularity between low and warning.\n");
    printf("Capacity granularity 2:\n    Battery capacity granularity between warning and full.\n");
    printf("Warning capacity limit:\n    OEM-designed battery warning capacity.\n");
    printf("Full charge capacity:\n    The predicted pack capacity when it is fully charged.\n");
    printf("Remaining capacity limit:\n    Sets the value of the battery’s remaining capacity, which causes a remaining\n");
    printf("    capacity alarm to be sent. Whenever the battery’s remaining capacity falls below.\n");
    printf("    the value in the RemainingCapacity alarm register, the battery periodically.\n");
    printf("    issues a RemainingCapacity alarm.\n");
    printf("Battery’s chemistry:\n    The battery’s chemistry.\n");
    printf("Rechargable:\n    Whether the battery is rechargable.\n");
    printf("\n");
}

unsigned char is_valid_vpid(unsigned short *vid, unsigned short *pid, char *vpid) {

    if (sscanf(vpid, "%hx:%hx", vid, pid) != 2) {

        printf("Format VID:PID invalid. Use the hexadecimal format (example: 0764:0601).\n");
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    signed char opt; // The cool option goes in
    int option_index = 0;
    unsigned char option_count = 0;
    opterr = 0; // Disable error messages from getopt_long()

    // Count all options
    while ((getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        option_count++;
    }

    if (option_count > 1) {
        printf("Please, provide only one option at a time.\n");
        return EXIT_FAILURE;
    }

    optind = 1; // Reinitialize getopt_long()

    // No abbreviations for long options
    if (optind < argc && argv[optind][0] == '-' && argv[optind][1] == '-') {
        unsigned char match = 0;

        for (int i = 0; long_options[i].name != 0; i++) {
            if (strcmp(&argv[optind][2], long_options[i].name) == 0)
                match = 1;
        }

        if (!match) {
            printf("Unknown option: %s.\n", argv[optind]);
            return EXIT_FAILURE;
        }
    }

    opt = getopt_long(argc, argv, short_options, long_options, &option_index);

    if (optind < argc) {
        printf("Too many arguments: ");
        for (int i = optind; i < argc; i++) {
            printf("%s ", argv[i]);
        }
            printf("\n");
            return EXIT_FAILURE;
    }

    switch (opt) {
        case 'l':
            if (list_all_hid_devices())
                return EXIT_FAILURE;
            break;
        case 'i':
            if (optarg == argv[optind -1] + 2) { // Space mandatory between option and value for short options
                printf("Unknown option: -i%s.\n", optarg);
                return EXIT_FAILURE;
            }

            unsigned short vid, pid;
            if(is_valid_vpid(&vid, &pid, optarg))
                return EXIT_FAILURE;

            if(get_info_from_vpid(vid, pid))
                return EXIT_FAILURE;

            break;
        case 'p':
            if (optarg == argv[optind -1] + 2) {
                printf("Unknown option: -p%s.\n", optarg);
                return EXIT_FAILURE;
            }

            if (get_info_from_path(optarg))
                return EXIT_FAILURE;

            break;
        case 'd':
            show_description();
            break;
        case 'h':
            show_help(argv[0]);
            break;
        case '?':
            if (optopt == 'i' || optopt == 'p') {
                if (optarg == NULL) {
                    printf("Option %s requires an argument.\n", argv[optind - 1]);
                    return EXIT_FAILURE;
                }
            }

            printf("Unknown option: %s.\n", argv[optind - 1]);
            return EXIT_FAILURE;
        default:
            printf("Use -h to show help.\n");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
