#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

unsigned char get_info_from_vpid(unsigned short pid, unsigned short vid);
unsigned char get_info_from_path(const char* path);

#endif
