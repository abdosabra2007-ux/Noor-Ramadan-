#include "logic.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

User          users[Max_users];
Athkar        athkarList[Max_athkar];
Dailyprogress logs[Max_logs];
Ramadanday    calendar[30];

int usercount        = 0;
int athkarCount      = 0;
int logcount         = 0;
int currentUserIndex = -1;
int currentRamadanDay = 1;

void fix_time(int &h, int &m) {
    while (m < 0)  { m += 60; h--; }
    while (m > 59) { m -= 60; h++; }
}

void initializedata() {
    usercount = 2;
    users[0] = { 1000, "عمر",  5,  100, "omar123", 285, true  };
    users[1] = { 1001, "عبدو", 12, 200, "abdo123", 395, false };

    athkarCount = 25;


    athkarList[0]  = { 1,  "سورة الاخلاص",                              "morning",     3   };
    athkarList[1]  = { 2,  "سبحان الله",                                "morning",     33  };
    athkarList[2]  = { 3,  "اللهم صل وسلم على نبينا محمد",              "morning",     10  };
    athkarList[3]  = { 4,  "سبحان الله وبحمده",                         "morning",     3   };
    athkarList[4]  = { 5,  "استغفر الله",                               "morning",     10  };
    athkarList[5]  = { 6,  "لا اله الا الله",                           "morning",     100 };
    athkarList[6]  = { 7,  "اللهم بك اصبحنا",                           "morning",     1   };
    athkarList[7]  = { 8,  "اعوذ بكلمات الله التامات من شر ما خلق",    "morning",     3   };
    athkarList[8]  = { 9,  "بسم الله الذي لا يضر مع اسمه شيء",         "morning",     3   };
    athkarList[9]  = { 10, "رضيت بالله ربا وبالاسلام دينا",             "morning",     1   };

    // أذكار المساء
    athkarList[10] = { 11, "سورة الناس",                                "evening",     3   };
    athkarList[11] = { 12, "الحمد لله",                                 "evening",     33  };
    athkarList[12] = { 13, "اللهم صل وسلم على نبينا محمد",              "evening",     10  };
    athkarList[13] = { 14, "سبحان الله وبحمده",                         "evening",     3   };
    athkarList[14] = { 15, "استغفر الله",                               "evening",     10  };
    athkarList[15] = { 16, "اللهم بك امسينا",                           "evening",     1   };
    athkarList[16] = { 17, "اعوذ بكلمات الله التامات من شر ما خلق",    "evening",     3   };
    athkarList[17] = { 18, "بسم الله الذي لا يضر مع اسمه شيء",         "evening",     3   };
    athkarList[18] = { 19, "رضيت بالله ربا وبالاسلام دينا",             "evening",     3   };
    athkarList[19] = { 20, "لا اله الا انت سبحانك اني كنت من الظالمين","evening",     3   };


    athkarList[20] = { 21, "سبحان الله",                                "afterprayer", 33  };
    athkarList[21] = { 22, "الحمد لله",                                 "afterprayer", 33  };
    athkarList[22] = { 23, "الله اكبر",                                 "afterprayer", 33  };
    athkarList[23] = { 24, "اللهم باسمك اموت واحيا",                    "afterprayer", 1   };
    athkarList[24] = { 25, "اللهم قني عذابك يوم تبعث عبادك",            "afterprayer", 3   };

    logcount = 2;
    logs[0] = { 1, 1000, 1, { true,  true,  true, true, false }, 80,  false, 285 };
    logs[1] = { 2, 1001, 1, { true,  true,  true, true, true  }, 100, true,  395 };
}

void initializecal() {
    for (int i = 0; i < 30; i++) {
        int fajr_h    = 4,  f_m = 31 - i;       fix_time(fajr_h,    f_m);
        int dhuhr_h   = 12, d_m = 9  - (i / 4); fix_time(dhuhr_h,   d_m);
        int asr_h     = 15, a_m = 30 + (i / 4); fix_time(asr_h,     a_m);
        int maghrib_h = 18, m_m = 29 + (i / 2); fix_time(maghrib_h, m_m);
        int isha_h    = 20, i_m = 0  + (i / 2); fix_time(isha_h,    i_m);

        calendar[i].day     = i + 1;
        calendar[i].fajr    = { fajr_h,    f_m };
        calendar[i].dhuhr   = { dhuhr_h,   d_m };
        calendar[i].asr     = { asr_h,     a_m };
        calendar[i].maghrib = { maghrib_h, m_m };
        calendar[i].isha    = { isha_h,    i_m };
        calendar[i].note    = "";
    }

    calendar[26].note = "ليله القدر (27)";
    calendar[29].note = "غدًا عيد… اللهم اجعله فرحًا وسلامًا وبركة.";
}


void savefiles() {

    QFile ufile("users.txt");
    if (ufile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&ufile);
        out.setEncoding(QStringConverter::Utf8);
        out << usercount << "\n";
        for (int i = 0; i < usercount; i++) {
            out << users[i].userID     << " "
                << users[i].userName   << " "
                << users[i].currentJuz << " "
                << users[i].dailyGoal  << " "
                << users[i].password   << " "
                << users[i].points     << " "
                << users[i].is_admin   << "\n";
        }
        ufile.close();
    }


    QFile afile("athkar.txt");
    if (afile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&afile);
        out.setEncoding(QStringConverter::Utf8);
        out << athkarCount << "\n";
        for (int i = 0; i < athkarCount; i++) {
            out << athkarList[i].thikrID         << "\n"
                << athkarList[i].content          << "\n"
                << athkarList[i].category         << "\n"
                << athkarList[i].targetRepetition << "\n";
        }
        afile.close();
    }


    QFile lfile("logs.txt");
    if (lfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&lfile);
        out.setEncoding(QStringConverter::Utf8);
        out << logcount << "\n";
        for (int i = 0; i < logcount; i++) {
            out << logs[i].logID  << " "
                << logs[i].userID << " "
                << logs[i].date   << " ";
            for (int j = 0; j < 5; j++)
                out << logs[i].prayersDone[j] << " ";
            out << logs[i].tasbeehCount  << " "
                << logs[i].isGoalReached << " "
                << logs[i].daily_points  << "\n";
        }
        lfile.close();
    }
}

void loadfiles() {
    QFile ufile("users.txt");
    if (ufile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ufile);
        in.setEncoding(QStringConverter::Utf8);
        in >> usercount;
        for (int i = 0; i < usercount; i++) {
            int isAdmin = 0;
            in >> users[i].userID
                >> users[i].userName
                >> users[i].currentJuz
                >> users[i].dailyGoal
                >> users[i].password
                >> users[i].points
                >> isAdmin;
            users[i].is_admin = (isAdmin == 1);
        }
        ufile.close();
    } else {
        initializedata();
    }

    QFile afile("athkar.txt");
    if (afile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&afile);
        in.setEncoding(QStringConverter::Utf8);
        in >> athkarCount;
        in.readLine();
        for (int i = 0; i < athkarCount; i++) {
            athkarList[i].thikrID         = in.readLine().toInt();
            athkarList[i].content          = in.readLine();
            athkarList[i].category         = in.readLine();
            athkarList[i].targetRepetition = in.readLine().toInt();
        }
        afile.close();
    }

    QFile lfile("logs.txt");
    if (lfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&lfile);
        in.setEncoding(QStringConverter::Utf8);
        in >> logcount;
        for (int i = 0; i < logcount; i++) {
            in >> logs[i].logID >> logs[i].userID >> logs[i].date;
            for (int j = 0; j < 5; j++) {
                int p = 0;
                in >> p;
                logs[i].prayersDone[j] = (p == 1);
            }
            int goalReached = 0;
            in >> logs[i].tasbeehCount >> goalReached >> logs[i].daily_points;
            logs[i].isGoalReached = (goalReached == 1);
        }
        lfile.close();
    }
}

void save_calendar() {
    QFile file("calendar.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        for (int i = 0; i < 30; i++) {
            out << calendar[i].day       << "|"
                << calendar[i].fajr.h   << "|" << calendar[i].fajr.m    << "|"
                << calendar[i].dhuhr.h  << "|" << calendar[i].dhuhr.m   << "|"
                << calendar[i].asr.h    << "|" << calendar[i].asr.m     << "|"
                << calendar[i].maghrib.h<< "|" << calendar[i].maghrib.m << "|"
                << calendar[i].isha.h   << "|" << calendar[i].isha.m    << "|"
                << calendar[i].note     << "\n";
        }
        file.close();
    }
}

void loadcal() {
    QFile file("calendar.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);
        for (int i = 0; i < 30; i++) {
            QString line   = in.readLine();
            QStringList p  = line.split("|");
            if (p.size() < 12) continue;
            calendar[i].day         = p[0].toInt();
            calendar[i].fajr.h      = p[1].toInt();
            calendar[i].fajr.m      = p[2].toInt();
            calendar[i].dhuhr.h     = p[3].toInt();
            calendar[i].dhuhr.m     = p[4].toInt();
            calendar[i].asr.h       = p[5].toInt();
            calendar[i].asr.m       = p[6].toInt();
            calendar[i].maghrib.h   = p[7].toInt();
            calendar[i].maghrib.m   = p[8].toInt();
            calendar[i].isha.h      = p[9].toInt();
            calendar[i].isha.m      = p[10].toInt();
            calendar[i].note        = p[11];
        }
        file.close();
    } else {
        initializecal();
    }
}

int find_user_index(int id, QString password) {
    for (int i = 0; i < usercount; i++) {
        if (users[i].userID == id && users[i].password == password)
            return i;
    }
    return -1;
}

int find_log_index(int userID, int day) {
    for (int i = 0; i < logcount; i++) {
        if (logs[i].userID == userID && logs[i].date == day)
            return i;
    }
    return -1;
}

int login(int id, QString password) {
    int index = find_user_index(id, password);
    if (index != -1) {
        users[index].points += 5;
        savefiles();
    }
    return index;
}

bool signUp(int id, QString name, QString password, int dailyGoal) {
    if (usercount >= Max_users) return false;

    for (int i = 0; i < usercount; i++) {
        if (users[i].userID == id) return false;
    }

    users[usercount].userID     = id;
    users[usercount].userName   = name;
    users[usercount].password   = password;
    users[usercount].dailyGoal  = dailyGoal;
    users[usercount].currentJuz = 1;
    users[usercount].points     = 10;
    users[usercount].is_admin   = false;
    usercount++;

    savefiles();
    return true;
}

QStringList getAthkarByCategory(QString category) {
    QStringList result;
    for (int i = 0; i < athkarCount; i++) {
        if (athkarList[i].category == category) {
            result << athkarList[i].content
                          + "  (" + QString::number(athkarList[i].targetRepetition) + " مرة)";
        }
    }
    return result;
}

bool addThikr(QString content, QString category, int repetition) {
    if (athkarCount >= Max_athkar) return false;

    athkarList[athkarCount].thikrID          = athkarCount + 1;
    athkarList[athkarCount].content          = content;
    athkarList[athkarCount].category         = category;
    athkarList[athkarCount].targetRepetition = repetition;
    athkarCount++;

    savefiles();
    return true;
}

void saveTasbeeh(int userIndex, int day, int count) {
    int logIndex = find_log_index(users[userIndex].userID, day);

    if (logIndex != -1) {
        logs[logIndex].tasbeehCount += count;
        users[userIndex].points     += count;
        logs[logIndex].daily_points += count;

        if (!logs[logIndex].isGoalReached &&
            logs[logIndex].tasbeehCount >= users[userIndex].dailyGoal) {
            logs[logIndex].isGoalReached  = true;
            users[userIndex].points      += 40;
            logs[logIndex].daily_points  += 40;
        }
    } else {
        if (logcount >= Max_logs) return;

        logs[logcount].logID         = logcount + 1;
        logs[logcount].userID        = users[userIndex].userID;
        logs[logcount].date          = day;
        logs[logcount].tasbeehCount  = count;
        logs[logcount].isGoalReached = false;
        logs[logcount].daily_points  = 0;
        for (int i = 0; i < 5; i++)
            logs[logcount].prayersDone[i] = false;

        users[userIndex].points     += count;
        logs[logcount].daily_points += count;

        if (count >= users[userIndex].dailyGoal) {
            logs[logcount].isGoalReached  = true;
            users[userIndex].points      += 40;
            logs[logcount].daily_points  += 40;
        }
        logcount++;
    }
    savefiles();
}

// =====================================================
// |       تحديث الصلوات                             |
// =====================================================
void updatePrayers(int userIndex, int day, bool prayers[5]) {
    int logIndex = find_log_index(users[userIndex].userID, day);

    if (logIndex != -1) {
        for (int i = 0; i < 5; i++) {
            bool oldState = logs[logIndex].prayersDone[i];
            bool newState = prayers[i];

            if (newState && !oldState) {
                users[userIndex].points     += 50;
                logs[logIndex].daily_points += 50;
            } else if (!newState && oldState) {
                users[userIndex].points     -= 50;
                logs[logIndex].daily_points -= 50;
            }
            logs[logIndex].prayersDone[i] = newState;
        }
    } else {
        if (logcount >= Max_logs) return;

        logs[logcount].logID         = logcount + 1;
        logs[logcount].userID        = users[userIndex].userID;
        logs[logcount].date          = day;
        logs[logcount].tasbeehCount  = 0;
        logs[logcount].isGoalReached = false;
        logs[logcount].daily_points  = 0;

        for (int i = 0; i < 5; i++) {
            logs[logcount].prayersDone[i] = prayers[i];
            if (prayers[i]) {
                users[userIndex].points     += 50;
                logs[logcount].daily_points += 50;
            }
        }
        logcount++;
    }
    savefiles();
}

int getPrayerPercent(int userIndex, int day) {
    int logIndex = find_log_index(users[userIndex].userID, day);
    if (logIndex == -1) return 0;

    int done = 0;
    for (int i = 0; i < 5; i++)
        if (logs[logIndex].prayersDone[i]) done++;

    return (done * 100) / 5;
}

QString getSpiritualReport(int userIndex) {
    QString report = "";
    report += "🌟 ـــــــــــــــ التقرير الروحي ـــــــــــــــ 🌟\n\n";

    report += "👤  الاسم\t: " + users[userIndex].userName + "\n";
    report += "📖  الجزء\t: " + QString::number(users[userIndex].currentJuz) + " / 30\n";
    report += "🎯  الهدف\t: " + QString::number(users[userIndex].dailyGoal) + " تسبيحة\n";
    report += "⭐  النقاط\t: " + QString::number(users[userIndex].points) + " نقطة\n\n";

    // المصفوفات الجديدة للإيموجي وأسماء الصلوات
    QString prayerNames[5] = { "الفجر", "الظهر", "العصر", "المغرب", "العشاء" };
    QString prayerEmojis[5] = { "🌅", "☀️", "🌤️", "🌇", "🌌" };

    bool found = false;

    for (int i = 0; i < logcount; i++) {
        if (logs[i].userID != users[userIndex].userID) continue;

        found = true;
        report += "📅  اليوم " + QString::number(logs[i].date) + " من رمضان\n\n";

        int prayerCount = 0;
        for (int j = 0; j < 5; j++) {
            if (logs[i].prayersDone[j]) prayerCount++;

            QString status = logs[i].prayersDone[j] ? "✅" : "❌";
            // هنا استخدمنا الإيموجي المخصص لكل صلاة
            report += "   " + prayerEmojis[j] + " " + prayerNames[j] + "  \t" + status + "\n";
        }

        report += "\n   📿 التسبيح: " + QString::number(logs[i].tasbeehCount) + " تسبيحة\n";
        report += "   🎯   الهدف: " + (logs[i].isGoalReached ? QString("✅ تم") : QString("❌ لم يكتمل")) + "\n";
        report += "   💫   النقاط: " + QString::number(logs[i].daily_points) + " نقطة\n";
        report += "   🙏   الإنجاز: " + QString::number(prayerCount) + " / 5\n\n";
    }

    if (!found) {
        report += "📌  لا توجد سجلات بعد.\n";
        report += "ابدأ رحلتك الروحية الآن! 🌙\n\n";
    }

    int     pts  = users[userIndex].points;
    QString rank, rankIcon;
    if (pts >= 500)      { rank = "حافظ مميز";  rankIcon = "🌟🌟🌟"; }
    else if (pts >= 300) { rank = "مجتهد";       rankIcon = "⭐⭐"; }
    else if (pts >= 100) { rank = "مبتدئ نشيط"; rankIcon = "⭐"; }
    else                 { rank = "مبتدئ";       rankIcon = "🌱"; }

    report += "🏆  مرتبتك\t: " + rankIcon + "  " + rank + "\n";

    return report;
}

QString getCalendarText() {
    auto fmt = [](int h, int m) -> QString {
        return QString("%1:%2")
        .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'));
    };

    QString text = "";
    text += "+-----+--------+--------+--------+--------+--------+------------------------+\n";
    text += "| Day | Fajr   | Dhuhr  | Asr    |Maghrib | Isha   | Note                   |\n";
    text += "+-----+--------+--------+--------+--------+--------+------------------------+\n";

    for (int i = 0; i < 30; i++) {
        text += QString("| %1 | %2 | %3 | %4 | %5 | %6 | %7|\n")
        .arg(calendar[i].day, 3)
            .arg(fmt(calendar[i].fajr.h,    calendar[i].fajr.m),    6)
            .arg(fmt(calendar[i].dhuhr.h,   calendar[i].dhuhr.m),   6)
            .arg(fmt(calendar[i].asr.h,     calendar[i].asr.m),     6)
            .arg(fmt(calendar[i].maghrib.h, calendar[i].maghrib.m), 6)
            .arg(fmt(calendar[i].isha.h,    calendar[i].isha.m),    6)
            .arg(calendar[i].note,          -22);
    }
    text += "+-----+--------+--------+--------+--------+--------+------------------------+\n";
    return text;
}

QString getDayInfo(int day) {
    if (day < 1 || day > 30) return "يوم غير صحيح";
    int i = day - 1;

    auto fmt = [](int h, int m) -> QString {
        return QString("%1:%2")
        .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'));
    };

    QString info = "";
    info += "══════════════════════════\n";
    info += "  Day " + QString::number(day) + " of Ramadan\n";
    info += "══════════════════════════\n";
    info += "Fajr    : " + fmt(calendar[i].fajr.h,    calendar[i].fajr.m)    + "\n";
    info += "Dhuhr   : " + fmt(calendar[i].dhuhr.h,   calendar[i].dhuhr.m)   + "\n";
    info += "Asr     : " + fmt(calendar[i].asr.h,     calendar[i].asr.m)     + "\n";
    info += "Maghrib : " + fmt(calendar[i].maghrib.h, calendar[i].maghrib.m) + "\n";
    info += "Isha    : " + fmt(calendar[i].isha.h,    calendar[i].isha.m)    + "\n";

    if (!calendar[i].note.isEmpty())
        info += "Note    : " + calendar[i].note + "\n";

    return info;
}

void editDayPrayer(int day, int prayerIndex, int h, int m) {
    if (day < 1 || day > 30) return;
    fix_time(h, m);
    int i = day - 1;

    switch (prayerIndex) {
    case 0: calendar[i].fajr    = { h, m }; break;
    case 1: calendar[i].dhuhr   = { h, m }; break;
    case 2: calendar[i].asr     = { h, m }; break;
    case 3: calendar[i].maghrib = { h, m }; break;
    case 4: calendar[i].isha    = { h, m }; break;
    }
    save_calendar();
}

QString getDailyHadith() {
    QString ahadith[7] = {
        "من غشنا فليس منا",
        "الدين النصيحة",
        "تبسمك في وجه اخيك صدقة",
        "الكلمة الطيبة صدقة",
        "يسروا ولا تعسروا",
        "الحياء من الايمان",
        "من لا يرحم لا يرحم"
    };
    int i = QRandomGenerator::global()->bounded(7);
    return ahadith[i];
}

void updateJuz(int userIndex, int newJuz) {
    if (newJuz < 1 || newJuz > 30) return;
    users[userIndex].currentJuz = newJuz;
    savefiles();
}

QString getTopicName(int topic) {
    QString names[7] = {
        "المحسنين", "المتقين", "التوابين",
        "المتطهرين", "المتوكلين", "المقسطين", "الصابرين"
    };
    if (topic < 1 || topic > 7) return "";
    return names[topic - 1];
}
int getDailyPoints(int userIndex, int day) {
    int logIndex = find_log_index(users[userIndex].userID, day);
    if (logIndex != -1) {
        return logs[logIndex].daily_points;
    }
    return 0;
}

QString getTopUserText() {
    if (usercount == 0) return "لا يوجد مستخدمين";

    int maxPoints = -1;
    QString topName = "";

    for (int i = 0; i < usercount; i++) {
        if (users[i].points > maxPoints) {
            maxPoints = users[i].points;
            topName = users[i].userName;
        }
    }

    return "🏆 المتصدر: " + topName + " (" + QString::number(maxPoints) + " نقطة)";
}