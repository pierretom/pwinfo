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
    printf("Usage:\n  %s [OPTION]\n\n"
        "Show information from PowerWalker UPS devices that support HID.\n"
        "Basic VI 850 SB currently supported, can work with similar models.\n"
        "No warranty, use it at your own risk.\n\n"
        "Options:\n"
        "  -l, --list             List all HID devices.\n"
        "  -i, --vpid VID:PID     Use Vendor ID and Product ID to get info from device.\n"
        "  -p, --path PATH        Use PATH (see --list option) to get info from device.\n"
        "  -d, --description      Show the description of usage names.\n"
        "  -h, --help             Show help.\n"
        , program_name
    );
}

void show_description() {
    printf(
        "\n"
        "General\n"
        "———————————————————————————————————————————————————————————————————————————————————————————\n"
        "Manufacturer:\n    Describe the manufacturer.\n"
        "Product:\n    Describe the product.\n"
        "Serial number:\n    Describe the device’s serial number.\n"
        "Firmware version:\n    Vendor-defined - Version of the firmware.\n"
        "Misc 1:\n    Vendor-defined - Unknown indexed string.\n"

        "\n"
        "Power\n"
        "———————————————————————————————————————————————————————————————————————————————————————————\n"
        "Output config active power:\n    Nominal value of the output active (RMS) power.\n"
        "Config voltage:\n    Nominal value of the voltage.\n"
        "Input voltage:\n    Actual value of the input voltage.\n"
        "Output voltage:\n    Actual value of the output voltage.\n"
        "Low voltage transfer:\n    The minimum line voltage allowed before the PS system transfers to battery backup.\n"
        "High voltage transfer:\n    The maximum line voltage allowed before the PS system transfers to battery backup.\n"
        "Output percent load:\n    The actual value of the percentage of the power capacity presently being used\n"
        "    on this output line, i.e., the greater of the percent load of true power\n"
        "    capacity and the percent load of apparent power.\n"
        "Input frequency:\n    Actual value of the input frequency.\n"
        "Output frequency:\n    Actual value of the output frequency.\n"
        "Overload:\n    Overloaded/Not Overloaded.\n"
        "    Indicates if the power demand from the connected devices exceeds the UPS's maximum \n"
        "    output capacity.\n"
        "Boost active:\n    Boosted/Not Boosted.\n"
        "    The status indicates this input is used in the module but voltage is increased\n"
        "    to fit within nominal range values.\n"
        "Buck active:\n    Bucked/Not Bucked.\n"
        "    The status indicates this input is used in the module but voltage is reduced\n"
        "    to fit with nominal range values.\n"

        "\n"
        "Battery\n"
        "———————————————————————————————————————————————————————————————————————————————————————————\n"
        "Config Voltage:\n    Nominal value of the voltage.\n"
        "Voltage:\n    Actual value of the voltage.\n"
        "Remaining capacity:\n    The predicted remaining capacity.\n"
        "Run time to empty:\n    The predicted remaining battery life, at the present rate of discharge.\n"
        "Remaining time limit:\n    Sets the value of the battery’s remaining time, which causes the RemainingTimeLimit\n"
        "    control to be activated. Whenever the battery’s remaining time falls below the\n"
        "    value in the RemainingTimeLimit register, the battery periodically issues a\n"
        "    RemainingTimeLimitExpired alarm.\n"
        "AC present:\n    Alternating current present/not present.\n"
        "Charging:\n    Battery is charging.\n"
        "Discharging:\n    Battery is discharing.\n"
        "Fully charged:\n    Battery is fully-charged.\n"
        "Below remaining capacity limit:\n    Is below?.\n"
        "Remaining time limit expired:\n    Has expired?.\n"
        "Design capacity:\n    The theoretical capacity of a new pack.\n"
        "Capacity granularity 1:\n    Battery capacity granularity between low and warning.\n"
        "Capacity granularity 2:\n    Battery capacity granularity between warning and full.\n"
        "Warning capacity limit:\n    OEM-designed battery warning capacity.\n"
        "Full charge capacity:\n    The predicted pack capacity when it is fully charged.\n"
        "Remaining capacity limit:\n    Sets the value of the battery’s remaining capacity, which causes a remaining\n"
        "    capacity alarm to be sent. Whenever the battery’s remaining capacity falls below.\n"
        "    the value in the RemainingCapacity alarm register, the battery periodically.\n"
        "    issues a RemainingCapacity alarm.\n"
        "Battery’s chemistry:\n    The battery’s chemistry.\n"
        "Rechargable:\n    Whether the battery is rechargable.\n"
        "\n"
    );
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
