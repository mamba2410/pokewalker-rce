#include "pw_utils.h"
#include "pw_states.h"
#include "Arduino.h"

uint8_t rx_buffer[128];
size_t rx_cursor = 0;
uint8_t packet_buffer[128];
size_t packet_length = 0;


/*
 *  Send a packet to Serial1
 *  Will calculate checksum and xor
 */
void send_packet(uint8_t packet[], const size_t packet_size) {
  uint16_t checksum = compute_checksum(packet, packet_size);
  packet[0x02] = checksum >> 8;
  packet[0x03] = checksum & 0xFF;

  for (size_t i = 0; i < packet_size; ++i)
    packet[i] ^= 0xAA;

  Serial1.write(packet, packet_size);
}

/*
 *  Compute the checksum for the packet
 */
uint16_t compute_checksum(const uint8_t packet[], const size_t packet_size) {
  uint16_t checksum = 0x0002;

  for (size_t i = 1; i < packet_size; i += 2)
    checksum += packet[i];

  for (size_t i = 0; i < packet_size; i += 2) {
    if ((packet[i] << 8) > UINT16_MAX - checksum)
      checksum++;

    checksum += (packet[i] << 8);
  }

  // Swap the bytes
  checksum = ((checksum << 8) & 0xFF00) | ((checksum >> 8) & 0xFF);

  return checksum;
}


/*
 *  Implementation of the CR16-CCITT algorithm
 *  adapted from https://stackoverflow.com/questions/17196743/crc-ccitt-implementation
 */
uint16_t crc16_ccitt(uint8_t packet[], size_t packet_size) {
  uint16_t crc = 0;
  while (packet_size > 0) {
    packet_size--;
    crc ^= (uint16_t)*packet++ << 8;
    for (size_t i = 7; i; i--) {
      if (crc & 0x8000)  crc = crc << 1 ^ 0x1021;
      else              crc = crc << 1;
    }
  }
  return crc;
}

// Check rx cursor > 7
// Check header
// Ignore bytes 1 and 2
// Ignore checksum (2/3) for now
// Check session ID
// Go back and check 
enum packet_error_e parse_packet() {
  uint32_t key;
  size_t packet_size, i;
  uint16_t checksum;
  uint16_t old_checksum;
  if (rx_cursor < 8) return PERR_SHORT;

  for (i = 0; i < rx_cursor - 7; i++) {
    key = ((uint32_t)rx_buffer[i + 4] << 24) | ((uint32_t)rx_buffer[i + 5] << 16) | ((uint32_t)rx_buffer[i + 6] << 8) | ((uint32_t)rx_buffer[i + 7] << 0);
    if (key == pw_key) goto validKey;
  }
  return PERR_SESSION;

validKey:
  packet_size = rx_cursor - i;
  old_checksum = ((uint16_t)rx_buffer[i + 2] << 8) | (uint16_t)rx_buffer[i + 3] & 0xFF;
  rx_buffer[i + 2] = 0x00;
  rx_buffer[i + 3] = 0x00;
  checksum = compute_checksum((rx_buffer + i), packet_size);
  rx_buffer[i + 2] = old_checksum >> 8;
  rx_buffer[i + 3] = old_checksum & 0xFF;

  if (checksum == old_checksum) {

    memcpy(packet_buffer, rx_buffer + i, packet_size);
    packet_length = packet_size;
    return PERR_OK;
  }
  else {
    return PERR_CHECKSUM;
  }

  return PERR_UNKNOWN;
}


/*
 *  Print bytes as a 2-digit hex number
 *  Print for `len` bytes
 */
void print_bytes(uint8_t* ptr, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (ptr[i] < 0x10) Serial.write('0');
    Serial.print(ptr[i], HEX);
  }
}

/*
 *  Print the packet in the rx_buffer for nBytes
 */
void print_packet(size_t n_bytes) {
  //if (nBytes == 0) nBytes = rx_cursor;
  print_bytes(rx_buffer, n_bytes);
  Serial.write('\n');
}

/*
 *  Print the number in 2-digit hexadecimal with prefix '0x'
 */
void print_hex(int n) {
  Serial.print("0x");
  if (n < 0x10) Serial.print("0");

  Serial.print(n, HEX);
}

/*
 *  Print the number as an 8-bit binary number
 */
void print_bin(uint8_t n) {
  char buf[9];
  for (size_t i = 0; i < 8; i++) {
    buf[7 - i] = (n & (1 << i)) ? '1' : '0';
  }
  buf[8] = 0;
  Serial.print(buf);
}
