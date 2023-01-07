#include <avr/sleep.h>
#include <avr/power.h>

void wake() {
  // Cancel sleep as a precaution
  sleep_disable();
  // Precautionary while we do other stuff
  detachInterrupt(0);
}  // End of wake
void sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Do not interrupt before we go to sleep, or the
  // ISR will detach interrupts and we won't wake.
  noInterrupts();

  // will be called when pin D2 goes HIGH
  attachInterrupt(0, wake, RISING);

  // turn off brown-out enable in software
  // BODS must be set to one and BODSE must be set to zero within four clock cycles
  MCUCR = bit(BODS) | bit(BODSE);
  // The BODS bit is automatically cleared after three clock cycles
  MCUCR = bit(BODS);

  // We are guaranteed that the sleep_cpu call will be done
  // as the processor executes the next instruction after
  // interrupts are turned on.
  interrupts();  // one cycle
  sleep_cpu();   // one cycle
}