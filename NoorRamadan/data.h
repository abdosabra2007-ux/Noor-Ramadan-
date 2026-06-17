#ifndef DATA_H
#define DATA_H

#include <QString>
struct User {
    int     userID      = 0;
    QString userName    = "";
    int     currentJuz  = 1;
    int     dailyGoal   = 0;
    QString password    = "";
    int     points      = 0;
    bool    is_admin    = false;
};

struct Athkar {
    int     thikrID          = 0;
    QString content          = "";
    QString category         = "";
    int     targetRepetition = 0;
};
struct Dailyprogress {
    int  logID          = 0;
    int  userID         = 0;
    int  date           = 0;
    bool prayersDone[5] = { false, false, false, false, false };
    int  tasbeehCount   = 0;
    bool isGoalReached  = false;
    int  daily_points   = 0;
};

struct Time {
    int h = 0;
    int m = 0;
};

struct Ramadanday {
    int     day  = 0;
    Time    fajr, dhuhr, asr, maghrib, isha;
    QString note = "";
};
const int Max_users  = 10;
const int Max_athkar = 35;
const int Max_logs   = 10;

extern User          users[Max_users];
extern Athkar        athkarList[Max_athkar];
extern Dailyprogress logs[Max_logs];
extern Ramadanday    calendar[30];

extern int usercount;
extern int athkarCount;
extern int logcount;

extern int currentUserIndex;
extern int currentRamadanDay;

#endif
