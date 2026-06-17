#ifndef LOGIC_H
#define LOGIC_H

#include "data.h"
#include <QString>
#include <QStringList>

void initializedata();
void initializecal();
void fix_time(int &h, int &m);

void savefiles();
void loadfiles();
void save_calendar();
void loadcal();

int find_user_index(int id, QString password);
int find_log_index(int userID, int day);

int  login(int id, QString password);
bool signUp(int id, QString name, QString password, int dailyGoal);


QStringList getAthkarByCategory(QString category);
bool        addThikr(QString content, QString category, int repetition);


void saveTasbeeh(int userIndex, int day, int count);
void updatePrayers(int userIndex, int day, bool prayers[5]);
int  getPrayerPercent(int userIndex, int day);


QString getSpiritualReport(int userIndex);

QString getCalendarText();
QString getDayInfo(int day);
void    editDayPrayer(int day, int prayerIndex, int h, int m);


QString getDailyHadith();
void    updateJuz(int userIndex, int newJuz);

QString getTopicName(int topic);

int getDailyPoints(int userIndex, int day);
QString getTopUserText();

#endif
