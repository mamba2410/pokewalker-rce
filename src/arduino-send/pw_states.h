#ifndef PW_STATES_H
#define PW_STATES_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "pokewalker.h"

enum comm_state_e {
  COMM_IDLE,
  COMM_KEYEX,
  COMM_READY,
  COMM_GDATA,
  COMM_ECHO,
  COMM_TRESP,         // test response to pw packets
  COMM_STATE_COUNT,
};

typedef void (*comm_func_t)(void);

void func_comm_idle(void);
void func_comm_keyex(void);
void func_comm_echo(void);

void func_post_keyex(void);

void set_comm_state(enum comm_state_e);
void send_packet(uint8_t, const size_t);

extern comm_func_t comm_func_table[];
extern enum comm_state_e current_state;
//extern uint32_t pw_key;
extern uint8_t pw_key[];

#endif /* PW_STATES_H */
