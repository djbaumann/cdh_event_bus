#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <map>
#include <mutex>

/*!
 * Class Watchdog contains functions and data for the software
 * watchdog timer.
 */
class WatchDog {
private:
  /*! map which holds module's unique IDs and their alive status */
  static std::map<long, bool> module_map;
  /*! mutex to prevent write collisons on module_map */
  std::mutex module_map_mutex;
  /*! function to check alive status on all known modules */
  bool checkmap();
  /*! default ammount of time to wait between checks */
  const int TIMER_LENGTH = 10;
  /*! current number of seconds remaining */
  int timerVal;
  /*! wait 1 second and decrement the timer */
  void tick();
  /*! reboot after module stops responding */
  void reset();
public:
  /*! default constructor */
  WatchDog() {timerVal = TIMER_LENGTH;}
  /*! wrapper for timer, used to create new thread */
  static void* timerStart(void* arg);
  /*! count down from TIMER_LENGTH and reboot if any task didn't ping */
  void timer();
  /*! update map with uniqueID and set response to true */
  void kick(long uniqueID);
};

void ping(); // ping octopOS with unique id

#endif  // WATCHDOG_H_
