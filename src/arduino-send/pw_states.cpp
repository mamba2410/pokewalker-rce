#include "pw_states.h"
#include "pw_utils.h"
#include "Arduino.h"
#include "pokewalker.h"

comm_func_t comm_func_table[] = { func_comm_idle, func_comm_keyex, func_comm_echo };
enum comm_state_e current_state = COMM_IDLE;
uint32_t pw_key = 0;
uint16_t dump_cursor = 0;

/*
 *  Function for when we are in the idle state
 */
void func_comm_idle() {
  if (rx_buffer[0] == 0xFC) {
    delay(3);
    uint8_t packet[] = { 0xFA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    send_packet(packet, sizeof(packet));

    Serial.println("[I] PokeWalker is here");
    Serial.println("[I] Moving on to keyex");

    set_comm_state(COMM_KEYEX);
  }
  else {
    Serial.println("[E] During idle: Wrong packet received, returning to idle");
    set_comm_state(COMM_IDLE);
  }
}


/*
 *  Function for when we are exchanging keys
 */
void func_comm_keyex() {
  if (rx_cursor == 8) {
    if (rx_buffer[0] == 0xF8 && rx_buffer[1] == 0x02) {
      pw_key = ((uint32_t)rx_buffer[4] << 24) | ((uint32_t)rx_buffer[5] << 16) | ((uint32_t)rx_buffer[6] << 8) | ((uint32_t)rx_buffer[7] << 0);

      Serial.print("[I] Received PW key!\n[I] > ");
      Serial.println(pw_key, HEX);

      delay(3);
      func_post_keyex();

    }
    else {
      Serial.println("[E] During keyex: Wrong packet received, returning to idle");
      set_comm_state(COMM_IDLE);
      return;
    }
  }
}

/*
 *  Code to be called directly after we receive a key
 */
void func_post_keyex() {

}


/*
 *  Echo Serial1 to Serial0
 */
void func_comm_echo() {
  enum packet_error_e err = parse_packet();
  switch (err) {
  case PERR_OK:
    Serial.print("OK: ");
    print_bytes(packet_buffer, packet_length);
    break;

  case PERR_SHORT:
    break;

  case PERR_SESSION:
    Serial.print("Could not find valid session ID in buffer: ");
    print_bytes(rx_buffer, rx_cursor);
    Serial.println();
    rx_cursor = 0;
    break;

  case PERR_CHECKSUM:
    //Serial.print("Invalid checksum: ");
    //printBytes(rx_buffer, rx_cursor);
    //Serial.println();
    break;

  default:
    Serial.println("Unknown error");
    break;
  }

  if (err != PERR_SHORT && err != PERR_CHECKSUM) {
    Serial.println();
    rx_cursor = 0;
  }

}

/*
 *  Set the global comm state and reset the cursor
 */
void set_comm_state(const comm_state_e new_state) {
  current_state = new_state;
  rx_cursor = 0;
}
