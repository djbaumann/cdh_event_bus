#include <iostream>

#include "../include/watchdog.h"
#include "../include/octopos.h"

/*!
 * kick() locks the mutex for module_map then updates the map with
 * the uniqueID. It will add the uniqueID to the map if necessary, and
 * will set that ID's value to true.
 * @param uniqueID unique identifier for a module, provided by octopOS
 */
void WatchDog::kick(long uniqueID) {
  module_map_mutex.lock();
  module_map[uniqueID] = true;
  module_map_mutex.unlock();
}

/*!
 * timerStart() is a wrapper for timer(), and should be used to create a
 * new thread
 * @param watchdog_timer_obj pointer to a WatchDog, which will run the timer
 */
void* WatchDog::timerStart(void* watchdog_timer_obj) {
  static_cast<WatchDog*>(watchdog_timer_obj)->timer();
  return NULL;
}

/*!
 * timer() counts down from TIMER_LENGTH to zero, using tick()
 * when zero is reached, the map is checked to ensure all modules have
 * responded. If all modules have responded within the past TIMER_LENGTH
 * seconds, then each uniqueID in the map is marked false and the timer is Reset
 * back to TIMER_LENGTH. Otherwise, an error has occurred and a module may be
 * broken. In this case, a reset signal will be sent to the physical watchdog
 * timer to reset the cpu.
 */
void WatchDog::timer() {
  while (timerVal > 0) {
    tick();
  }
  if (checkmap()) {
    timerVal = TIMER_LENGTH;
    module_map_mutex.lock();
    for (auto iter = module_map.begin(); iter != module_map.end(); iter++) {
      iter->second = false;
      module_map_mutex.unlock();
    }
  } else {
    reset();
  }
}

/*!
 * tick() waits 1 second then decrements the number of seconds remaining
 */
void WatchDog::tick() {
  sleep(1);
  timerVal--;
}

/*!
 * checkmap() iterates through module_map and checks if every module has responded
 * @return true, if every module has responded; false otherwise
 */
bool WatchDog::checkmap() {
  bool alive = true;
  for (auto iter = module_map.begin(); iter != module_map.end(); iter++) {
    if (iter->second == false) {
      alive = false;
    }
  }
  return alive;
}

/*!
 * TODO: reset() should send a reset signal to the physical watchdog timer to reset
 * the cpu, or send a reset signal to the operating system to perform a
 * soft reboot if possible.
 */
void WatchDog::reset() {
  // Currently unimpplemented, stub funtion for now
  std::cout << "You fucked up" << std::endl;
  throw "Watchdog Reset";
}

/*!
 * ping sends a signal from a module to octopOS, asking for its uniqueID to be
 * writen to the watchdog timer.
 */
void ping() {

}
