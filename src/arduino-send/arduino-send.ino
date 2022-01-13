#include "pw_utils.h"
#include "pw_states.h"
#include "pokewalker.h"

static unsigned long last_recv = 0;

/*
 *  Built around arduino due
 *  Serial  - arduino/host
 *  Serial1 - arduino/pokewalker
 */
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("[I] Arduino is up!");
}

void loop() {
  if (Serial1.available() > 0) {
    // Get the packet data
    while (Serial1.available()) {
      rx_buffer[rx_cursor++] = Serial1.read() ^ 0xAA;
    }

    // Save when we last received a packet
    last_recv = millis();

    // Are we in a valid state? If no...
    if (current_state >= COMM_STATE_COUNT) {
      Serial.println("[W] Unknown comm state");
      Serial.println("[W] Packet received: ");
      print_packet(8);

      set_comm_state(COMM_IDLE);
      Serial.println("[W] Returning to idle state");
      return;
    }

    // we must be in a valid state, so jump to the function which handles that state
    (*(comm_func_table[current_state]))();

    return;
  } // End of Serial1.available()

  // Are we still active or expected to be idle?
  if (millis() > last_recv + 1000 && current_state != COMM_IDLE && current_state != COMM_ECHO) {
    Serial.println("[E] Timeout");
    set_comm_state(COMM_IDLE);
  }
}
