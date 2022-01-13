#ifndef PW_UTILS_H
#define PW_UTILS_H

#include <stdint.h>
#include <stddef.h>

enum packet_error_e {
    PERR_OK,
    PERR_SHORT,
    PERR_SESSION,
    PERR_CHECKSUM,
    PERR_UNKNOWN,
};

void print_bytes(uint8_t*, size_t);
void print_packet(size_t);
void print_hex(int);
void print_bin(uint8_t);

uint16_t compute_checksum(const uint8_t packet[], const size_t packet_size);
uint16_t crc16_ccitt(uint8_t packet[], size_t packet_size);
void send_packet(uint8_t packet[], const size_t packet_size);
enum packet_error_e parse_packet();


extern uint8_t rx_buffer[];
extern size_t  rx_cursor;
extern uint8_t packet_buffer[];
extern size_t packet_length;

#endif /* PW_UTILS_H */