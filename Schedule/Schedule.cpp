//
// Created by Andriy Kobetiak on 19.11.2022.
//

#include "Schedule.hpp"
#include "iostream"
#include <utility>
#include "cmath"

namespace Schedule {
#pragma region DailyItem
  DailyItem::DailyItem(
      Time startAtTime,
      Time endAtTime,
      std::string description,
      bool completed
  ) : description(std::move(description)),
      completed(completed),
      startAt(startAtTime),
      endAt(endAtTime)
  {
    if (startAtTime >= endAtTime) {
      throw std::invalid_argument("Start time cant be after end time");
    }
  }

  std::string DailyItem::toString() const {
    std::string timeBlock = this->startAt.toString() + "-" + this->endAt.toString();
    std::string desc = "description: " + this->description;
    std::string isCompleted = this->completed
      ? "done"
      : "to do";

    return timeBlock + "\t|\t" + isCompleted + "\t|\t" + desc;
  }

  const Time &DailyItem::getStartAt() const {
    return startAt;
  }

  void DailyItem::setStartAt(const Time &startAtTime) {
    if (startAtTime >= endAt) {
      throw std::invalid_argument("Start time cant be after end time");
    }

    DailyItem::startAt = startAtTime;
  }

  const Time &DailyItem::getEndAt() const {
    return endAt;
  }

  void DailyItem::setEndAt(const Time &endAtTime) {
    if (startAt <= endAtTime) {
      throw std::invalid_argument("End time cant be before start time");
    }

    DailyItem::endAt = endAtTime;
  }

  const std::string &DailyItem::getDescription() const {
    return description;
  }

  void DailyItem::setDescription(const std::string &desc) {
    DailyItem::description = desc;
  }

  bool DailyItem::isCompleted() const {
    return completed;
  }

  void DailyItem::setCompleted(bool isCompleted) {
    DailyItem::completed = isCompleted;
  }

#pragma endregion DailyItem

#pragma region Time

  Time::Time(int hour, int minute) {
    if (Time::validate(hour, minute)) {
      throw std::invalid_argument("Invalid value for hour or/and minute");
    }

    this->setHour(hour);
    this->setMinute(minute);
  }

  bool Time::validate(int hour, int minute) {
    return (minute < 0 || minute > 59) || (hour < 0 || hour > 23);
  }

  Time Time::createTime(int hour, int minute) {
    if (minute == 60) {
      return {hour + 1, 0};
    }

    return {hour, minute};
  }

  std::string Time::toString() const {
    std::string stringifiedHour = Time::normalizeTime(this->getHour());
    std::string stringifiedMinute = Time::normalizeTime(this->getMinute());

    return std::string(stringifiedHour + ":" + stringifiedMinute);
  }

  int Time::getHour() const {
    return hour;
  }

  void Time::setHour(int value) {
    Time::hour = value;
  }

  int Time::getMinute() const {
    return minute;
  }

  void Time::setMinute(int value) {
    Time::minute = value;
  }

  std::string Time::normalizeTime(int time) {
    return time < 10
           ? "0" + std::to_string(time)
           : std::to_string(time);
  }

  bool Time::operator==(const Time &rhs) const {
    return hour == rhs.hour &&
           minute == rhs.minute;
  }

  bool Time::operator!=(const Time &rhs) const {
    return !(rhs == *this);
  }

  bool operator>(const Time &rhs, const Time &rhs2) {
    if (rhs.getHour() < rhs2.getHour()) {
      return false;
    }

    if (rhs.getHour() > rhs2.getHour()) {
      return true;
    }

    return rhs.getMinute() > rhs2.getMinute();
  }

  bool operator<(const Time &rhs, const Time &rhs2) {
    return operator>(rhs2, rhs);
  }

  bool operator>=(const Time &rhs, const Time &rhs2) {
    return !(operator<(rhs, rhs2));
  }

  bool operator<=(const Time &rhs, const Time &rhs2) {
    return !operator>(rhs, rhs2);
  }

#pragma endregion Time

#pragma region DailySchedule
  DailySchedule DailySchedule::createSchedule() {
    return {};
  }

  void DailySchedule::addEvent(const DailyItem& item) {
    if (this->getEvents().empty()) {
      this->events.push_back(item);
    }

    if (!this->canAddEvent(item)) {
      return;
    }

    int size = static_cast<int>(this->getEvents().size());
    int index = size;

    for(int i = 0; i < size; i++) {
      if (item.getStartAt() <= this->getEvents()[i].getStartAt()) {
        index = i;
        break;
      }
    }

    auto insertPosition = this->getEvents().begin() + index;

    this->events.insert(insertPosition, item);
  }

  void DailySchedule::addEvent(
    Time startAt,
    Time endAt,
    std::string description,
    bool completed
  ) {
    auto item = DailyItem(
      startAt,
      endAt,
      std::move(description),
      completed
    );

    this->addEvent(item);
  }

  DailyItem DailySchedule::removeEvent(int index) {
    if (index > this->getEvents().size()) {
      throw std::out_of_range("Event not found");
    }

    const auto position = this->getEvents().begin() + index;
    DailyItem event = this->getEvents()[index];

    this->events.erase(position);

    return event;
  }

  std::string DailySchedule::toString() const {
    std::string stringifiedSchedule;
    int index = 0;

    std::for_each(
    this->getEvents().begin(),
    this->getEvents().end(),
    [&stringifiedSchedule, &index](const DailyItem &item) {
      stringifiedSchedule += std::to_string(index++) + '\t' + item.toString() + "\n";
    });

    return stringifiedSchedule;
  }

  const std::vector<DailyItem> &DailySchedule::getEvents() const {
    return events;
  }

  DailyItem *DailySchedule::getEvent(int index) {
    if (index > this->getEvents().size()) {
      throw std::out_of_range("Event not found");
    }

    return &events[index];
  }

  bool DailySchedule::canAddEvent(const Time &startAt, const Time &endAt) const {
    int size = static_cast<int>(this->getEvents().size());

    for (int i = 0; i < size; i++) {
      DailyItem event = this->getEvents()[i];

      if ((startAt >= event.getStartAt() && event.getEndAt() > startAt)
          || (endAt > event.getStartAt() && event.getEndAt() > endAt)
          || (startAt < event.getStartAt() && event.getEndAt() < endAt)) {
        return false;
      }
    }

    return true;
  }

  bool DailySchedule::canAddEvent(const DailyItem& item) const {
    return this->canAddEvent(item.getStartAt(), item.getEndAt());
  }

  DailyItem DailySchedule::editEvent(int index) {
    int fieldId;
    int newHour;
    int newMinute;
    bool isValid;
    Time newTime = Time::createTime(0, 0);
    std::string editedDescription;
    DailyItem* event = this->getEvent(index);

    std::cout << "Event: " << event->toString() << std::endl;
    std::cout << "  What do you want to change?" << std::endl;
    std::cout << "\t1. start time"
      << "\n\t2. end time"
      << "\n\t3. description"
      << "\n\t4. status" << std::endl;

    std::cout << "Enter index: "; std::cin >> fieldId;

    switch (fieldId) {
      case 1:
      case 2:
        std::cout << "Enter new "
          << (fieldId == 1 ? "start" : "end")
          << " time:\n";
        std::cout << " Hour: "; std::cin >> newHour;
        std::cout << " Minute: "; std::cin >> newMinute;

        newTime.setHour(newHour);
        newTime.setMinute(newMinute);

        isValid = this->canAddEvent(event->getStartAt(), newTime);

        if (!isValid) {
          throw std::invalid_argument("Invalid new time");
        }

        event->setEndAt(newTime);

        return *event;
      case 3:
        std::cout << "Enter new description: ";
        std::cin >> editedDescription;
        std::cout << std::endl;

        event->setDescription(editedDescription);

        return *event;

      case 4:
        event->setCompleted(!event->isCompleted());
        std::cout << "Status successfully changed" << std::endl << std::endl;

        return *event;

      default:
        std::cout << "Nothing to change" << std::endl;
        return *event;
    }
  }

  bool DailySchedule::hasFreeTime(int minutes, Time from, Time to) {
    if (minutes > 24 * 60) {
      return false;
    }

    int size = static_cast<int>(this->getEvents().size());

    if (!size) {
      return true;
    }

    Time eventStartTime = from;
    Time eventEndTime = this->getEvent(0)->getEndAt();
    int diffInMinutes = DailySchedule::getDifferenceInMinutes(
      eventEndTime,
      eventStartTime
    );

    if (diffInMinutes > minutes) {
      return true;
    }

    for (int i = 0; i < size - 1; i++) {
      eventEndTime = this->getEvent(i + 1)->getStartAt();
      eventStartTime = this->getEvent(i)->getEndAt();

      diffInMinutes = DailySchedule::getDifferenceInMinutes(
          eventEndTime,
          eventStartTime
      );

      if (diffInMinutes > minutes) {
        return true;
      }
    }

    eventStartTime = this->getEvent(size - 1)->getEndAt();
    eventEndTime = to;

    diffInMinutes = DailySchedule::getDifferenceInMinutes(
        eventEndTime,
        eventStartTime
    );

    if (diffInMinutes > minutes) {
      return true;
    }

    return false;
  }

  bool DailySchedule::hasFreeTime(int minutes, Time from) {
    return this->hasFreeTime(
      minutes,
      from,
      Time::createTime(23, 59)
    );
  }

  bool DailySchedule::hasFreeTime(int minutes) {
    return this->hasFreeTime(
      minutes,
      Time::createTime(0, 0)
    );
  }

  int DailySchedule::getDifferenceInMinutes(Time from, Time to) {
    const int hoursDiff = abs(from.getHour() - to.getHour());
    const int minutesDiff = from.getMinute() - to.getMinute();

    return hoursDiff * 60 + minutesDiff;
  }

  DailyItem DailySchedule::findTime(int minutes, Time from, Time to) {
    if (
      !this->hasFreeTime(minutes)
      || minutes > DailySchedule::getDifferenceInMinutes(from, to)
    ) {
      std::cout << "here" << std::endl;
      throw std::out_of_range("No time period found for these parameters");
    }

    int countOfMinutes = (minutes) % 60;
    int countOfHours = (minutes - countOfMinutes) / 60;

    DailyItem dailyItem{
        from,
        Time::createTime(
          from.getHour() + countOfHours,
          from.getMinute() + countOfMinutes
        ),
        ""
    };

    if (this->canAddEvent(dailyItem)) {
      return dailyItem;
    }

    int size = static_cast<int>(this->getEvents().size());

    for (int i = 0; i < size - 1; i++) {
      const Time endTime = this->getEvent(i)->getEndAt();
      const Time startTime = this->getEvent(i + 1)->getStartAt();

      const int diffInMinutes = DailySchedule::getDifferenceInMinutes(startTime, endTime);

      if (diffInMinutes > minutes && endTime >= from) {
        Time eventStartTime = Time::createTime(endTime.getHour(), endTime.getMinute());
        Time eventEndTime = Time::createTime(
          endTime.getHour() + countOfHours,
          endTime.getMinute() + countOfMinutes
        );

        if (eventEndTime > to) {
          throw std::out_of_range("No time period found for these parameters");
        }

        std::cout << eventStartTime.toString() << '\t' << eventEndTime.toString() << '\t' << to.toString() << '\n';

        return {
            eventStartTime,
            eventEndTime,
            "",
            false
        };
      }
    }

    DailyItem* lastEvent = this->getEvent(size - 1);

    dailyItem = {
        lastEvent->getEndAt(),
        Time::createTime(
            lastEvent->getEndAt().getHour() + countOfHours,
            lastEvent->getEndAt().getMinute() + countOfMinutes
        ),
        ""
    };

    if (this->canAddEvent(dailyItem) && dailyItem.getEndAt() <= to) {
      return dailyItem;
    }

    throw std::out_of_range("No time period found for these parameters");
  }

  DailyItem DailySchedule::findTime(int minutes) {
    return this->findTime(
      minutes,
      Time::createTime(0, 0)
    );
  }

  DailyItem DailySchedule::findTime(int minutes, Time from) {
    return this->findTime(
        minutes,
        from,
        Time::createTime(23, 59)
    );
  }

  DailySchedule DailySchedule::getAllUncompleted() {
    DailySchedule redo{};

    int size = static_cast<int>(this->getEvents().size());

    for (int i = 0; i < size; i++) {
      DailyItem event = *this->getEvent(i);

      if (!event.isCompleted()) {
        redo.addEvent(event);
      }
    }

    return redo;
  }
#pragma endregion DailySchedule
} // Schedule