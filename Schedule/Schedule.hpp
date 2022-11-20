//
// Created by Andriy Kobetiak on 19.11.2022.
//

#ifndef OOP_LAB_7_1_SCHEDULE_HPP
#define OOP_LAB_7_1_SCHEDULE_HPP

#include "string"
#include "vector"

namespace Schedule {
  class Time {
    private:
      int hour{};
      int minute{};

      static std::string normalizeTime(int time);
      static bool validate(int hour, int minute);

      Time(int hour, int minute);

    public:
      [[nodiscard]] int getHour() const;
      void setHour(int hour);

      [[nodiscard]] int getMinute() const;
      void setMinute(int minute);

      static Time createTime(int hour, int minutes);
      [[nodiscard]] std::string toString() const;

      bool operator==(const Time &rhs) const;
      bool operator!=(const Time &rhs) const;
      friend bool operator>(const Time&, const Time&);
      friend bool operator<(const Time &, const Time &);
      friend bool operator>=(const Time &, const Time &);
      friend bool operator<=(const Time &, const Time &);
  };

  class DailyItem {
    private:
      Time startAt;
      Time endAt;
      std::string description;
      bool completed = false;

    public:
      DailyItem(
        Time startAtTime,
        Time endAtTime,
        std::string description,
        bool completed = false
      );

      [[nodiscard]] std::string toString() const;

      [[nodiscard]] const Time &getStartAt() const;

      void setStartAt(const Time &startAtTime);

      [[nodiscard]] const Time &getEndAt() const;

      void setEndAt(const Time &endAtTime);

      [[nodiscard]] const std::string &getDescription() const;

      void setDescription(const std::string &description);

      [[nodiscard]] bool isCompleted() const;

      void setCompleted(bool isCompleted);
  };

  typedef std::vector<DailyItem> TodoList;

  class DailySchedule {
    private:
      TodoList events;
    public:
      static int getDifferenceInMinutes(Time from, Time to);
      [[nodiscard]] static DailySchedule createSchedule();

      [[nodiscard]] const std::vector<DailyItem> &getEvents() const;
      [[nodiscard]] DailyItem *getEvent(int index);

      [[nodiscard]] bool canAddEvent(const DailyItem& item) const;
      [[nodiscard]] bool canAddEvent(const Time& start, const Time& end) const;

      void addEvent(const DailyItem& item);
      void addEvent(
        Time startAt,
        Time endAt,
        std::string description,
        bool completed = false
      );

      DailyItem removeEvent(int index);
      DailyItem editEvent(int index);

      [[nodiscard]] bool hasFreeTime(int minutes);
      [[nodiscard]] bool hasFreeTime(int minutes, Time from);
      [[nodiscard]] bool hasFreeTime(int minutes, Time from, Time to);

      DailyItem findTime(int minutes);
      DailyItem findTime(int minutes, Time from);
      DailyItem findTime(int minutes, Time from, Time to);

      [[nodiscard]] std::string toString() const;

      DailySchedule getAllUncompleted();
  };

} // Schedule

#endif //OOP_LAB_7_1_SCHEDULE_HPP
