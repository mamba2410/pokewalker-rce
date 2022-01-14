#include <stdlib.h>
#include "send_payload.h"
#include "pw_states.h"
#include "pw_utils.h"
#include "payload.h"

static const uint8_t EXEC_VEC_HIGH = 0xf7;
static const uint8_t EXEC_VEC_LOW  = 0xe0;

void send_payload(uint8_t high, uint8_t low) {
	size_t packet_length = 8+1+__pw_bin_len;
	uint8_t *packet = malloc(packet_length);

	packet[0] = 0x06;					// CMD_06
	packet[1] = high;					// high byte of payload addr
	memcpy(pw_key, &(packet[4]), 4);	// Copy key into packet header
	packet[8] = low;					// low byte of payload addr
	memcpy(__pw_bin, &(packet[9]), __pw_bin_len);	// copy payload

	send_packet(packet, packet_length);	// Send off packet

	free(packet);
}

void write_addr(uint8_t high, uint8_t low) {
	size_t packet_length = 8+3;
	uint8_t packet[] = {
		0x06, EXEC_VEC_HIGH,		// CMD_06 plus high byte of vector
		0x00, 0x00,					// empty checksum
		0x00, 0x00, 0x00, 0x00,		// empty session id
		EXEC_VEC_LOW, high, low};	// low byte of vector plus address of payload

	memcpy(pw_key, &(packet[4]), 4);	// Copy key into packet header

	send_packet(packet, packet_length);	// Send off packet

}
