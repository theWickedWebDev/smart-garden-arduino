#include "globals.h"

void setup_timer1() {
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = COMPARE_MATCH_REGISTER;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

int timer1_rounds = 1;
int colon_state = 1;

// timer1 ISR
ISR(TIMER1_COMPA_vect){
  if (TIMER_TRIGGER_COUNT == timer1_rounds) {
    run_loop_checks = true;
    timer1_rounds = 1;
  } else {
    run_loop_checks = false;
    timer1_rounds++;
  }
}
