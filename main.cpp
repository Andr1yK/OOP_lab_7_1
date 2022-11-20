#include <iostream>
#include "Schedule/Schedule.hpp"

using namespace Schedule;

int main() {
  Time time1 = Time::createTime(10, 20);
  Time time2 = Time::createTime(11, 40);

  DailySchedule schedule = DailySchedule::createSchedule();

  schedule.addEvent(
    Time::createTime(0, 10),
    Time::createTime(2, 20),
    "test 1",
    true
  );

  schedule.addEvent(
      Time::createTime(0, 1),
      Time::createTime(0, 9),
      "test 3"
  );

  schedule.addEvent(
      time1,
      time2,
      "test 2"
  );

  schedule.addEvent(
      Time::createTime(8, 20),
      Time::createTime(9, 30),
      "test 4"
  );

  std::cout << schedule.toString() << std::endl;

  schedule.addEvent(schedule.findTime(
    5,
    Time::createTime(10, 20)
  ));

  schedule.editEvent(3);

  std::cout << "Removed event: " << schedule.removeEvent(2).toString() << "\n\n";

  std::cout << schedule.toString() << std::endl;

  DailySchedule redo = schedule.getAllUncompleted();

  std::cout << "Redo:\n" << redo.toString() << "\n\n";

  std::cout << "Removed event: " << schedule.removeEvent(0).toString() << "\n\n";
  std::cout << "Removed event: " << schedule.removeEvent(1).toString() << "\n\n";

  std::cout << schedule.toString() << std::endl;
  std::cout << "Redo:\n" << redo.toString() << "\n\n";

  return 0;
}
