#include "mainwindow.h"
#include "logic.h"
#include "data.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QFrame>
#include <QApplication>
#include <QSoundEffect>
#include <QUrl>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QTimer>
#include <QTime>
#include <QTableWidget>
#include <QHeaderView>
#include <QMainWindow>
#include <QStackedWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    loadfiles();
    loadcal();
    calendarTable = new QTableWidget(30, 7);
    calendarTable->setHorizontalHeaderLabels({"اليوم", "الفجر", "الظهر", "العصر", "المغرب", "العشاء", "ملاحظات"});

    achieveSound = new QSoundEffect(this);
    achieveSound->setSource(QUrl::fromLocalFile("achieve.wav"));
    achieveSound->setVolume(0.8f);
    azanPlayer = new QMediaPlayer(this);
    azanAudioOutput = new QAudioOutput(this);
    azanPlayer->setAudioOutput(azanAudioOutput);

    // بناء المسار تلقائياً بناءً على مكان الـ exe
    QString soundPath = QCoreApplication::applicationDirPath() + "/azan.mp3";
    azanPlayer->setSource(QUrl::fromLocalFile(soundPath));

    azanAudioOutput->setVolume(1.0f);
    setWindowTitle("نور رمضان 🌙");
    setMinimumSize(900, 650);
    setStyleSheet(BG_COLOR);


    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    stack->addWidget(makePage_Welcome());
    stack->addWidget(makePage_Login());
    stack->addWidget(makePage_SignUp());
    stack->addWidget(makePage_Main());
    stack->addWidget(makePage_Athkar());
    stack->addWidget(makePage_Tasbeeh());
    stack->addWidget(makePage_Progress());
    stack->addWidget(makePage_Report());
    stack->addWidget(makePage_Calendar());
    stack->addWidget(makePage_Stories());
    stack->addWidget(makePage_AddThikr());
    stack->addWidget(makePage_EditCal());
    stack->addWidget(makePage_StoryImage());
    stack->setCurrentIndex(0);

    tasbeehCounter = 0;
    QTimer *azanTimer = new QTimer(this);
    connect(azanTimer, &QTimer::timeout, this, [this]() {

        int currentH = QTime::currentTime().hour();
        int currentM = QTime::currentTime().minute();

        static int lastNotifiedMinute = -1;

        int todayIndex = currentRamadanDay - 1;
        if (todayIndex < 0 || todayIndex > 29) {
            return;
        }

        Time fajr    = calendar[todayIndex].fajr;
        Time dhuhr   = calendar[todayIndex].dhuhr;
        Time asr     = calendar[todayIndex].asr;
        Time maghrib = calendar[todayIndex].maghrib;
        Time isha    = calendar[todayIndex].isha;

        if (currentH == fajr.h && currentM == fajr.m) {
            if (lastNotifiedMinute != currentM) {
                lastNotifiedMinute = currentM;
                showPrayMessage("الفجر");
            }
        } else if (currentH == dhuhr.h && currentM == dhuhr.m) {
            if (lastNotifiedMinute != currentM) {
                lastNotifiedMinute = currentM;
                showPrayMessage("الظهر");
            }
        } else if (currentH == asr.h && currentM == asr.m) {
            if (lastNotifiedMinute != currentM) {
                lastNotifiedMinute = currentM;
                showPrayMessage("العصر");
            }
        } else if (currentH == maghrib.h && currentM == maghrib.m) {
            if (lastNotifiedMinute != currentM) {
                lastNotifiedMinute = currentM;
                showPrayMessage("المغرب");
            }
        } else if (currentH == isha.h && currentM == isha.m) {
            if (lastNotifiedMinute != currentM) {
                lastNotifiedMinute = currentM;
                showPrayMessage("العشاء");
            }
        }
    });
    azanTimer->start(1000);
}

void MainWindow::showPrayMessage(QString prayName) {
    QString soundPath = QCoreApplication::applicationDirPath() + "/azan.mp3";

    if (!QFile::exists(soundPath)) {
        QMessageBox::critical(this, "خطأ مسار", "البرنامج بيبحث عن الملف في المسار ده ومش لاقيه:\n" + soundPath);
        return;
    }

    azanPlayer->play();

    QMessageBox msg;
    msg.setWindowTitle("تنبيه الصلاة");
    msg.setText("حان الآن موعد أذان صلاة " + prayName);
    msg.setIcon(QMessageBox::Information);
    msg.setStyleSheet("QLabel { color: white; font-size: 18px; font-weight: bold; } "
                      "QMessageBox { background-color: #121212; } "
                      "QPushButton { background-color: #0078D7; color: white; }");
    msg.exec();

    azanPlayer->stop();
}

QWidget* MainWindow::makeDarkPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet(BG_COLOR);
    return page;
}

QPushButton* MainWindow::makeBtn(QString text, bool red) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(red ? BTN_RED_STYLE : BTN_STYLE);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumHeight(50);
    return btn;
}

QLabel* MainWindow::makeTitle(QString text) {
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet(
        "color: #E6C200;"
        "font-size: 26px;"
        "font-weight: bold;"
        "padding: 15px;"
        "font-family: 'Tajawal', 'Cairo', sans-serif;"
        );
    return lbl;
}

QLineEdit* MainWindow::makeInput(QString placeholder, bool password) {
    QLineEdit *inp = new QLineEdit();
    inp->setPlaceholderText(placeholder);
    inp->setStyleSheet(INPUT_STYLE);
    inp->setMinimumHeight(45);
    inp->setLayoutDirection(Qt::RightToLeft);
    if (password)
        inp->setEchoMode(QLineEdit::Password);
    return inp;
}

QComboBox* MainWindow::makeCombo() {
    QComboBox *cb = new QComboBox();
    cb->setLayoutDirection(Qt::RightToLeft);
    cb->setStyleSheet(COMBO_STYLE);
    return cb;
}

QWidget* MainWindow::makePage_Welcome() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(20);
    lay->setContentsMargins(120, 60, 120, 60);

    QLabel *moon = new QLabel("🌙");
    moon->setAlignment(Qt::AlignCenter);
    moon->setStyleSheet("font-size: 80px;");

    QLabel *title = new QLabel("نور رمضان");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "color: #E6C200;"
        "font-size: 34px;"
        "font-weight: bold;"
        "font-family: 'Tajawal', 'Cairo', sans-serif;"
        );

    QLabel *sub = new QLabel("رفيقك الروحي في شهر القرآن");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #FFFFFF; font-size: 16px; font-style: italic; font-family: 'Tajawal', 'Cairo', sans-serif;");

    lay->addStretch();
    lay->addWidget(moon);
    lay->addWidget(title);
    lay->addWidget(sub);
    lay->addSpacing(30);

    QPushButton *btnLogin  = makeBtn("🔑  تسجيل الدخول");
    QPushButton *btnSignUp = makeBtn("📝  إنشاء حساب جديد");
    QPushButton *btnExit   = makeBtn("🚪  خروج", true);

    lay->addWidget(btnLogin);
    lay->addWidget(btnSignUp);
    lay->addSpacing(10);
    lay->addWidget(btnExit);
    lay->addStretch();

    connect(btnLogin,  &QPushButton::clicked, this, &MainWindow::goToLogin);
    connect(btnSignUp, &QPushButton::clicked, this, &MainWindow::goToSignUpFromWelcome);
    connect(btnExit,   &QPushButton::clicked, this, &MainWindow::onExitApp);

    return page;
}

QWidget* MainWindow::makePage_Login() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(15);
    lay->setContentsMargins(100, 50, 100, 50);

    QLabel *moon = new QLabel("🌙");
    moon->setAlignment(Qt::AlignCenter);
    moon->setStyleSheet("font-size: 60px;");

    lay->addWidget(moon);
    lay->addWidget(makeTitle("تسجيل الدخول"));

    QLineEdit *idField = makeInput("الرقم التعريفي");
    idField->setObjectName("loginID");
    lay->addWidget(idField);

    QLineEdit *passField = makeInput("كلمة المرور", true);
    passField->setObjectName("loginPass");
    lay->addWidget(passField);

    QLabel *dayLabel = makeTitle("أنت في أي يوم من رمضان؟");
    QLineEdit *dayInputBox = makeInput("اكتب اليوم (1 - 30)");
    dayInputBox->setValidator(new QIntValidator(1, 30, this));

    lay->addWidget(dayLabel);
    lay->addWidget(dayInputBox);

    QPushButton *btnLogin = makeBtn("تسجيل الدخول");
    QPushButton *btnBack  = makeBtn("🔙 رجوع", true);

    lay->addSpacing(10);
    lay->addWidget(btnLogin);
    lay->addWidget(btnBack);
    lay->addStretch();

    connect(btnLogin, &QPushButton::clicked, this, [this, dayInputBox]() {
        currentRamadanDay = dayInputBox->text().toInt();

        if (currentRamadanDay > 30) {
            QMessageBox::warning(this, "تنبيه", "شهر رمضان 30 يوماً فقط! يرجى إدخال رقم صحيح.");
            return;
        }

        if (currentRamadanDay <= 0) {
            currentRamadanDay = 1;
        }

        onLogin();
    });

    connect(btnBack, &QPushButton::clicked, this, [=]{ stack->setCurrentIndex(0); });

    return page;
}

QWidget* MainWindow::makePage_SignUp() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(100, 40, 100, 40);

    lay->addWidget(makeTitle("إنشاء حساب جديد 🌙"));

    QLineEdit *idF    = makeInput("الرقم التعريفي (أرقام فقط)");
    QLineEdit *nameF  = makeInput("الاسم");
    QLineEdit *passF  = makeInput("كلمة المرور", true);

    QLabel *goalLbl = new QLabel("الهدف اليومي من التسبيح:");
    goalLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    goalLbl->setAlignment(Qt::AlignRight);

    QSpinBox *goalSpin = new QSpinBox();
    goalSpin->setRange(1, 10000);
    goalSpin->setValue(100);
    goalSpin->setStyleSheet(INPUT_STYLE);
    goalSpin->setMinimumHeight(45);

    idF->setObjectName("signupID");
    nameF->setObjectName("signupName");
    passF->setObjectName("signupPass");
    goalSpin->setObjectName("signupGoal");

    QPushButton *btnCreate = makeBtn("إنشاء الحساب");
    QPushButton *btnBack   = makeBtn("🔙  رجوع", true);

    lay->addWidget(idF);
    lay->addWidget(nameF);
    lay->addWidget(passF);
    lay->addWidget(goalLbl);
    lay->addWidget(goalSpin);
    lay->addSpacing(10);
    lay->addWidget(btnCreate);
    lay->addWidget(btnBack);
    lay->addStretch();

    connect(btnCreate, &QPushButton::clicked, this, &MainWindow::onSignUp);
    connect(btnBack,   &QPushButton::clicked, this, [=]{ stack->setCurrentIndex(0); });

    return page;
}

QWidget* MainWindow::makePage_Main() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *mainLay = new QVBoxLayout(page);
    mainLay->setSpacing(20);
    mainLay->setContentsMargins(40, 30, 40, 30);

    QLabel *moon = new QLabel("🌙");
    moon->setAlignment(Qt::AlignCenter);
    moon->setStyleSheet("font-size: 50px;");

    QLabel *welcomeLbl = new QLabel("أهلاً بك في نور رمضان");
    welcomeLbl->setObjectName("welcomeLabel");
    welcomeLbl->setAlignment(Qt::AlignCenter);
    welcomeLbl->setStyleSheet("color: #E6C200; font-size: 20px; font-weight: bold; font-family: 'Tajawal', 'Cairo', sans-serif;");

    QLabel *hadithLbl = new QLabel();
    hadithLbl->setObjectName("hadithLabel");
    hadithLbl->setAlignment(Qt::AlignCenter);
    hadithLbl->setWordWrap(true);
    hadithLbl->setStyleSheet(
        "color: #E6C200;"
        "font-size: 20px;"
        "font-weight: bold;"
        "padding: 10px;"
        "font-family: 'Tajawal', 'Cairo', sans-serif;"
        );
    mainLay->addWidget(moon);
    mainLay->addWidget(welcomeLbl);
    mainLay->addWidget(hadithLbl);
    mainLay->addSpacing(10);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(15);

    QPushButton *btnAthkar   = makeBtn("📿  الأذكار");
    QPushButton *btnTasbeeh  = makeBtn("🤲  عداد التسبيح");
    QPushButton *btnCalendar = makeBtn("📅  التقويم");
    QPushButton *btnProgress = makeBtn("📊  تحديث التقدم");
    QPushButton *btnReport   = makeBtn("⭐  تقريري الروحي");
    QPushButton *btnStories  = makeBtn("📖  القصص والدروس");

    grid->addWidget(btnAthkar,   0, 0);
    grid->addWidget(btnTasbeeh,  0, 1);
    grid->addWidget(btnCalendar, 0, 2);
    grid->addWidget(btnProgress, 1, 0);
    grid->addWidget(btnReport,   1, 1);
    grid->addWidget(btnStories,  1, 2);

    mainLay->addLayout(grid);

    QPushButton *btnAddThikr = makeBtn("➕  إضافة ذكر جديد");
    btnAddThikr->setObjectName("adminBtn");
    btnAddThikr->setVisible(false);

    QPushButton *btnEditCal = makeBtn("✏️  تعديل التقويم");
    btnEditCal->setObjectName("adminCalBtn");
    btnEditCal->setVisible(false);

    mainLay->addWidget(btnAddThikr);
    mainLay->addWidget(btnEditCal);
    mainLay->addSpacing(10);

    QHBoxLayout *exitLay = new QHBoxLayout();
    QPushButton *btnLogout = makeBtn("🚪  تسجيل الخروج", true);
    QPushButton *btnExit   = makeBtn("❌  إغلاق التطبيق", true);
    exitLay->addWidget(btnLogout);
    exitLay->addWidget(btnExit);
    mainLay->addLayout(exitLay);

    connect(btnAthkar,   &QPushButton::clicked, this, &MainWindow::goToAthkar);
    connect(btnTasbeeh,  &QPushButton::clicked, this, &MainWindow::goToTasbeeh);
    connect(btnCalendar, &QPushButton::clicked, this, &MainWindow::goToCalendar);
    connect(btnProgress, &QPushButton::clicked, this, &MainWindow::goToProgress);
    connect(btnReport,   &QPushButton::clicked, this, &MainWindow::goToReport);
    connect(btnStories,  &QPushButton::clicked, this, &MainWindow::goToStories);
    connect(btnAddThikr, &QPushButton::clicked, this, &MainWindow::goToAddThikr);
    connect(btnEditCal,  &QPushButton::clicked, this, &MainWindow::goToEditCal);
    connect(btnLogout,   &QPushButton::clicked, this, &MainWindow::onLogout);
    connect(btnExit,     &QPushButton::clicked, this, &MainWindow::onExitApp);

    return page;
}

QWidget* MainWindow::makePage_Athkar() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(40, 30, 40, 30);

    lay->addWidget(makeTitle("📿  مكتبة الأذكار"));

    QHBoxLayout *catLay = new QHBoxLayout();
    QPushButton *btnMorning  = makeBtn("☀️  الصباح");
    QPushButton *btnEvening  = makeBtn("🌙  المساء");
    QPushButton *btnAfter    = makeBtn("🕌  بعد الصلاة");

    catLay->addWidget(btnMorning);
    catLay->addWidget(btnEvening);
    catLay->addWidget(btnAfter);
    lay->addLayout(catLay);

    athkarDisplay = new QTextEdit();
    athkarDisplay->setReadOnly(true);
    athkarDisplay->setLayoutDirection(Qt::RightToLeft);
    athkarDisplay->setStyleSheet(
        "QTextEdit {"
        "  color: #FFFFFF;"
        "  background: #0B2411;"
        "  border: 1px solid #28A745;"
        "  border-radius: 6px;"
        "  padding: 15px;"
        "  font-size: 16px;"
        "  line-height: 1.8;"
        "  font-family: 'Tajawal', 'Cairo', sans-serif;"
        "}"
        );
    lay->addWidget(athkarDisplay);

    QPushButton *btnBack = makeBtn("🔙  رجوع", true);
    lay->addWidget(btnBack);

    connect(btnMorning, &QPushButton::clicked, this, &MainWindow::showAthkarMorning);
    connect(btnEvening, &QPushButton::clicked, this, &MainWindow::showAthkarEvening);
    connect(btnAfter,   &QPushButton::clicked, this, &MainWindow::showAthkarAfter);
    connect(btnBack,    &QPushButton::clicked, this, &MainWindow::goBack);

    showAthkarMorning();

    return page;
}

QWidget* MainWindow::makePage_Tasbeeh() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(10);
    lay->setContentsMargins(60, 20, 60, 20);

    lay->addWidget(makeTitle("🤲  عداد التسبيح"));

    tasbeehCombo = makeCombo();
    for (int i = 0; i < athkarCount; i++) {
        tasbeehCombo->addItem(
            athkarList[i].content + "  (" +
            QString::number(athkarList[i].targetRepetition) + " مرة)"
            );
    }
    lay->addWidget(tasbeehCombo);

    tasbeehThikr = new QLabel("");
    tasbeehThikr->setAlignment(Qt::AlignCenter);
    tasbeehThikr->setWordWrap(true);
    tasbeehThikr->setStyleSheet(
        "color: #FFFFFF;"
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 8px;"
        "background: #0B2411;"
        "border: 1px solid #28A745;"
        "border-radius: 6px;"
        "font-family: 'Tajawal', 'Cairo', sans-serif;"
        );
    lay->addWidget(tasbeehThikr);

    tasbeehTarget = new QLabel("الهدف: - | سبّحت: 0 | باقي: -");
    tasbeehTarget->setAlignment(Qt::AlignCenter);
    tasbeehTarget->setStyleSheet(
        "color: #FFFFFF; font-size: 14px; padding: 4px; font-family: 'Tajawal', 'Cairo', sans-serif;"
        );
    lay->addWidget(tasbeehTarget);

    tasbeehCount = new QLabel("0");
    tasbeehCount->setAlignment(Qt::AlignCenter);
    tasbeehCount->setFixedSize(160, 160);

    tasbeehCount->setStyleSheet(
        "color: #28A745;"
        "font-size: 60px;"
        "font-weight: bold;"
        "border: 3px solid #28A745;"
        "border-radius: 12px;"
        "background: #0B2411;"
        );
    lay->addWidget(tasbeehCount, 0, Qt::AlignCenter);

    lay->addSpacing(30);

    QPushButton *btnSabeeh = new QPushButton("سبّح  🤲");
    btnSabeeh->setStyleSheet(
        "QPushButton {"
        "  color: #FFFFFF;"
        "  background: #19692C;"
        "  border-radius: 6px;"
        "  padding: 18px 40px;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  font-family: 'Tajawal', 'Cairo', sans-serif;"
        "}"
        "QPushButton:hover {"
        "  background: #208738;"
        "}"
        "QPushButton:pressed {"
        "  background: #124A1F;"
        "}"
        );
    btnSabeeh->setCursor(Qt::PointingHandCursor);
    btnSabeeh->setMinimumHeight(60);
    lay->addWidget(btnSabeeh);

    QHBoxLayout *dayLay = new QHBoxLayout();
    QLabel *dayLbl = new QLabel("اليوم:");
    dayLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");

    tasbeehDay = new QSpinBox();
    tasbeehDay->setRange(1, 30);
    tasbeehDay->setValue(1);
    tasbeehDay->setStyleSheet(INPUT_STYLE);
    tasbeehDay->setMinimumHeight(40);

    dayLay->addStretch();
    dayLay->addWidget(dayLbl);
    dayLay->addWidget(tasbeehDay);
    lay->addLayout(dayLay);

    QPushButton *btnSave  = makeBtn("💾  حفظ التسبيح");
    QPushButton *btnReset = makeBtn("🔄  إعادة ضبط", true);
    QPushButton *btnBack  = makeBtn("🔙  رجوع", true);

    lay->addWidget(btnSave);
    QHBoxLayout *botLay = new QHBoxLayout();
    botLay->addWidget(btnReset);
    botLay->addWidget(btnBack);
    lay->addLayout(botLay);

    connect(btnSabeeh, &QPushButton::clicked, this, &MainWindow::onTasbeeh);
    connect(btnSave,   &QPushButton::clicked, this, &MainWindow::onSaveTasbeeh);
    connect(btnReset,  &QPushButton::clicked, this, [=]{
        tasbeehCounter = 0;
        tasbeehCount->setText("0");
        tasbeehCount->setStyleSheet(
            "color: #28A745; font-size: 80px; font-weight: bold;"
            "border: 3px solid #28A745; border-radius: 12px;"
            "min-width: 220px; min-height: 220px;"
            "background: #0B2411;"
            );
        updateTasbeehTarget();
    });
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);
    connect(tasbeehCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTasbeehComboChanged);

    return page;
}

// ============== صفحة التقدم (تم إعادتها للكود الأصلي السليم) ==============
QWidget* MainWindow::makePage_Progress() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(60, 30, 60, 30);

    lay->addWidget(makeTitle("📊  التقدم اليومي"));

    QHBoxLayout *dayLay = new QHBoxLayout();
    QLabel *dayLbl = new QLabel("يوم رمضان:");
    dayLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    progressDay = new QSpinBox();
    progressDay->setRange(1, 30);
    progressDay->setValue(1);
    progressDay->setStyleSheet(INPUT_STYLE);
    progressDay->setMinimumHeight(45);
    dayLay->addStretch();
    dayLay->addWidget(dayLbl);
    dayLay->addWidget(progressDay);
    lay->addLayout(dayLay);

    QHBoxLayout *juzLay = new QHBoxLayout();
    QLabel *juzLbl = new QLabel("الجزء الحالي:");
    juzLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    progressJuz = new QSpinBox();
    progressJuz->setRange(1, 30);
    progressJuz->setValue(1);
    progressJuz->setStyleSheet(INPUT_STYLE);
    progressJuz->setMinimumHeight(45);
    juzLay->addStretch();
    juzLay->addWidget(juzLbl);
    juzLay->addWidget(progressJuz);
    lay->addLayout(juzLay);

    QLabel *prayLbl = new QLabel("الصلوات:");
    prayLbl->setStyleSheet("color:#FFFFFF; font-size:17px; font-weight:bold; font-family: 'Tajawal', 'Cairo', sans-serif;");
    prayLbl->setAlignment(Qt::AlignRight);
    lay->addWidget(prayLbl);

    QString prayNames[5] = {"الفجر", "الظهر", "العصر", "المغرب", "العشاء"};
    QString prayEmoji[5] = {"🌅", "☀️", "🌤️", "🌇", "🌌"};

    for (int i = 0; i < 5; i++) {
        prayerCheck[i] = new QCheckBox(prayEmoji[i] + "  " + prayNames[i]);
        prayerCheck[i]->setLayoutDirection(Qt::RightToLeft);
        prayerCheck[i]->setStyleSheet(
            "QCheckBox { color: #FFFFFF; font-size: 16px; padding: 5px; font-family: 'Tajawal', 'Cairo', sans-serif; }"
            "QCheckBox::indicator { width: 22px; height: 22px; border: 2px solid #28A745; border-radius: 4px; }"
            "QCheckBox::indicator:checked { background: #28A745; }"
            );
        lay->addWidget(prayerCheck[i]);
    }

    QPushButton *btnSave = makeBtn("💾  حفظ التقدم");
    QPushButton *btnBack = makeBtn("🔙  رجوع", true);

    lay->addSpacing(10);
    lay->addWidget(btnSave);
    lay->addWidget(btnBack);

    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveProgress);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);

    return page;
}

// ============== صفحة التقرير الروحي (تم إضافة المتصدر هنا) ==============
QWidget* MainWindow::makePage_Report() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(40, 30, 40, 30);

    // إضافة خانة المتصدر فوق على الشمال
    QHBoxLayout *topLayout = new QHBoxLayout();
    topUserLabel = new QLabel("جاري التحميل...", this);
    topUserLabel->setStyleSheet("color: #FFD700; font-weight: bold; font-size: 16px; background-color: #123A1B; padding: 10px; border-radius: 6px; border: 1px solid #FFD700;");
    topLayout->addWidget(topUserLabel);
    topLayout->addStretch(); // دفع الخانة لليسار
    lay->addLayout(topLayout);

    lay->addWidget(makeTitle("⭐  تقريري الروحي"));

    reportDisplay = new QTextEdit();
    reportDisplay->setReadOnly(true);
    reportDisplay->setLayoutDirection(Qt::RightToLeft);

    reportDisplay->setStyleSheet(
        "QTextEdit {"
        "  color: #FFFFFF;"
        "  background: #0B2411;"
        "  border: 1px solid #28A745;"
        "  border-radius: 6px;"
        "  padding: 15px;"
        "  font-size: 15px;"
        "  font-family: 'Tajawal', 'Cairo', sans-serif;"
        "  line-height: 1.6;"
        "}"
        );
    lay->addWidget(reportDisplay);

    QPushButton *btnBack = makeBtn("🔙  رجوع", true);
    lay->addWidget(btnBack);

    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);

    return page;
}

QWidget* MainWindow::makePage_Calendar() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(15);
    lay->setContentsMargins(40, 30, 40, 30);

    lay->addWidget(makeTitle("📅  تقويم رمضان"));

    calendarTable = new QTableWidget(30, 7);
    calendarTable->setHorizontalHeaderLabels({"اليوم", "الفجر", "الظهر", "العصر", "المغرب", "العشاء", "ملاحظات"});

    calendarTable->setStyleSheet(
        "QTableWidget { background-color: #0B2411; border: 1px solid #28A745; color: #FFFFFF; font-family: 'Cairo'; }"
        "QHeaderView::section { background-color: #0A0F0D; color: #E6C200; padding: 8px; border: 1px solid #28A745; font-weight: bold; }"
        "QTableWidget::item { border-bottom: 1px solid #123A1B; padding: 5px; }"
        "QTableWidget::item:selected { background-color: #123A1B; }"
        );

    calendarTable->setColumnWidth(6, 200);
    calendarTable->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < 30; i++) {
        calendarTable->setItem(i, 0, new QTableWidgetItem(QString::number(calendar[i].day)));
        calendarTable->setItem(i, 1, new QTableWidgetItem(QString("%1:%2").arg(calendar[i].fajr.h).arg(calendar[i].fajr.m)));
        calendarTable->setItem(i, 2, new QTableWidgetItem(QString("%1:%2").arg(calendar[i].dhuhr.h).arg(calendar[i].dhuhr.m)));
        calendarTable->setItem(i, 3, new QTableWidgetItem(QString("%1:%2").arg(calendar[i].asr.h).arg(calendar[i].asr.m)));
        calendarTable->setItem(i, 4, new QTableWidgetItem(QString("%1:%2").arg(calendar[i].maghrib.h).arg(calendar[i].maghrib.m)));
        calendarTable->setItem(i, 5, new QTableWidgetItem(QString("%1:%2").arg(calendar[i].isha.h).arg(calendar[i].isha.m)));

        calendarTable->setItem(i, 6, new QTableWidgetItem(calendar[i].note));
    }

    lay->addWidget(calendarTable);

    QPushButton *btnBack = makeBtn("🔙  رجوع", true);
    lay->addWidget(btnBack);

    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);

    return page;
}

QWidget* MainWindow::makePage_AddThikr() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(60, 40, 60, 40);

    lay->addWidget(makeTitle("➕  إضافة ذكر جديد"));

    newThikrContent = makeInput("نص الذكر");
    lay->addWidget(newThikrContent);

    QLabel *catLbl = new QLabel("التصنيف:");
    catLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    catLbl->setAlignment(Qt::AlignRight);
    lay->addWidget(catLbl);

    newThikrCat = makeCombo();
    newThikrCat->addItems({"morning", "evening", "afterprayer"});
    lay->addWidget(newThikrCat);

    QLabel *repLbl = new QLabel("عدد التكرار:");
    repLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    repLbl->setAlignment(Qt::AlignRight);
    lay->addWidget(repLbl);

    newThikrRep = new QSpinBox();
    newThikrRep->setRange(1, 1000);
    newThikrRep->setValue(33);
    newThikrRep->setStyleSheet(INPUT_STYLE);
    newThikrRep->setMinimumHeight(45);
    lay->addWidget(newThikrRep);

    lay->addStretch();

    QPushButton *btnAdd  = makeBtn("➕  إضافة");
    QPushButton *btnBack = makeBtn("🔙  رجوع", true);

    lay->addWidget(btnAdd);
    lay->addWidget(btnBack);

    connect(btnAdd,  &QPushButton::clicked, this, &MainWindow::onAddThikr);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);

    return page;
}

QWidget* MainWindow::makePage_EditCal() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(12);
    lay->setContentsMargins(60, 40, 60, 40);

    lay->addWidget(makeTitle("✏️  تعديل التقويم"));

    QHBoxLayout *dayLay = new QHBoxLayout();
    QLabel *dayLbl = new QLabel("اليوم:");
    dayLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    editDay = new QSpinBox();
    editDay->setRange(1, 30);
    editDay->setValue(1);
    editDay->setStyleSheet(INPUT_STYLE);
    editDay->setMinimumHeight(45);
    dayLay->addStretch();
    dayLay->addWidget(dayLbl);
    dayLay->addWidget(editDay);
    lay->addLayout(dayLay);

    QLabel *prayLbl = new QLabel("الصلاة:");
    prayLbl->setStyleSheet("color:#FFFFFF; font-size:15px; font-family: 'Tajawal', 'Cairo', sans-serif;");
    prayLbl->setAlignment(Qt::AlignRight);
    lay->addWidget(prayLbl);

    editPrayer = makeCombo();
    editPrayer->addItems({"الفجر", "الظهر", "العصر", "المغرب", "العشاء"});
    lay->addWidget(editPrayer);

    QHBoxLayout *timeLay = new QHBoxLayout();
    QLabel *hLbl = new QLabel("الساعة:");
    hLbl->setStyleSheet("color:#FFFFFF; font-family: 'Tajawal', 'Cairo', sans-serif;");

    editHour = new QSpinBox();
    editHour->setRange(0, 23);
    editHour->setStyleSheet(INPUT_STYLE);
    editHour->setMinimumHeight(45);

    QLabel *mLbl = new QLabel("الدقيقة:");
    mLbl->setStyleSheet("color:#FFFFFF; font-family: 'Tajawal', 'Cairo', sans-serif;");

    editMin = new QSpinBox();
    editMin->setRange(0, 59);
    editMin->setStyleSheet(INPUT_STYLE);
    editMin->setMinimumHeight(45);

    timeLay->addStretch();
    timeLay->addWidget(hLbl);
    timeLay->addWidget(editHour);
    timeLay->addWidget(mLbl);
    timeLay->addWidget(editMin);
    lay->addLayout(timeLay);

    lay->addStretch();

    QPushButton *btnEdit = makeBtn("💾  حفظ التعديل");
    QPushButton *btnBack = makeBtn("🔙  رجوع", true);

    lay->addWidget(btnEdit);
    lay->addWidget(btnBack);

    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditDay);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);

    return page;
}

void MainWindow::goToLogin() {
    stack->setCurrentIndex(1);
}

void MainWindow::goToSignUpFromWelcome() {
    stack->setCurrentIndex(2);
}

void MainWindow::onExitApp() {
    if (currentUserIndex >= 0)
        savefiles();
    QApplication::quit();
}

void MainWindow::onLogin() {
    QLineEdit *idF   = stack->widget(1)->findChild<QLineEdit*>("loginID");
    QLineEdit *passF = stack->widget(1)->findChild<QLineEdit*>("loginPass");

    if (!idF || !passF) return;

    bool ok;
    int id = idF->text().toInt(&ok);
    if (!ok || idF->text().isEmpty()) {
        QMessageBox::warning(this, "خطأ", "أدخل رقماً تعريفياً صحيحاً.");
        return;
    }

    QString pass = passF->text();
    if (pass.isEmpty()) {
        QMessageBox::warning(this, "خطأ", "كلمة المرور فارغة.");
        return;
    }

    int index = login(id, pass);

    if (index == -1) {
        QMessageBox::warning(this, "فشل الدخول", "الرقم التعريفي أو كلمة المرور خطأ.");
        return;
    }

    currentUserIndex = index;

    QLabel *welcomeLbl = stack->widget(3)->findChild<QLabel*>("welcomeLabel");
    if (welcomeLbl)
        welcomeLbl->setText("أهلاً بك يا " + users[index].userName + "  🌙");

    QLabel *hadithLbl = stack->widget(3)->findChild<QLabel*>("hadithLabel");
    if (hadithLbl)
        hadithLbl->setText("❝  " + getDailyHadith() + "  ❞");

    QPushButton *adminBtn    = stack->widget(3)->findChild<QPushButton*>("adminBtn");
    QPushButton *adminCalBtn = stack->widget(3)->findChild<QPushButton*>("adminCalBtn");
    if (adminBtn)    adminBtn->setVisible(users[index].is_admin);
    if (adminCalBtn) adminCalBtn->setVisible(users[index].is_admin);

    idF->clear();
    passF->clear();

    stack->setCurrentIndex(3);
}

void MainWindow::goToSignUp() {
    stack->setCurrentIndex(2);
}

void MainWindow::onSignUp() {
    QLineEdit *idF   = stack->widget(2)->findChild<QLineEdit*>("signupID");
    QLineEdit *nameF = stack->widget(2)->findChild<QLineEdit*>("signupName");
    QLineEdit *passF = stack->widget(2)->findChild<QLineEdit*>("signupPass");
    QSpinBox  *goal  = stack->widget(2)->findChild<QSpinBox*>("signupGoal");

    if (!idF || !nameF || !passF || !goal) return;

    bool ok;
    int id = idF->text().toInt(&ok);
    if (!ok || idF->text().isEmpty()) {
        QMessageBox::warning(this, "خطأ", "أدخل رقماً تعريفياً صحيحاً.");
        return;
    }

    if (nameF->text().isEmpty() || passF->text().isEmpty()) {
        QMessageBox::warning(this, "خطأ", "الاسم وكلمة المرور مطلوبان.");
        return;
    }

    bool created = signUp(id, nameF->text(), passF->text(), goal->value());

    if (!created) {
        QMessageBox::warning(this, "خطأ", "الرقم التعريفي موجود مسبقاً أو النظام ممتلئ.");
        return;
    }


    int index = login(id, passF->text());
    currentUserIndex = index;

    QLabel *welcomeLbl = stack->widget(3)->findChild<QLabel*>("welcomeLabel");
    if (welcomeLbl)
        welcomeLbl->setText("أهلاً بك يا " + users[index].userName + "  🌙");

    QLabel *hadithLbl = stack->widget(3)->findChild<QLabel*>("hadithLabel");
    if (hadithLbl)
        hadithLbl->setText("❝  " + getDailyHadith() + "  ❞");

    QPushButton *adminBtn    = stack->widget(3)->findChild<QPushButton*>("adminBtn");
    QPushButton *adminCalBtn = stack->widget(3)->findChild<QPushButton*>("adminCalBtn");
    if (adminBtn)    adminBtn->setVisible(users[index].is_admin);
    if (adminCalBtn) adminCalBtn->setVisible(users[index].is_admin);

    QMessageBox::information(this, "تم", "تم إنشاء الحساب بنجاح! 🎉");
    idF->clear(); nameF->clear(); passF->clear();

    stack->setCurrentIndex(3);
}

void MainWindow::goToMain()     { stack->setCurrentIndex(3); }
void MainWindow::goBack()       { stack->setCurrentIndex(3); }

void MainWindow::goToAthkar() {
    showAthkarMorning();
    stack->setCurrentIndex(4);
}

void MainWindow::goToTasbeeh() {
    tasbeehCounter = 0;
    tasbeehCount->setText("0");
    tasbeehCount->setStyleSheet(
        "color: #28A745; font-size: 80px; font-weight: bold;"
        "border: 3px solid #28A745; border-radius: 12px;"
        "min-width: 220px; min-height: 220px;"
        "background: #0B2411;"
        );
    updateTasbeehTarget();
    stack->setCurrentIndex(5);
}

// ============== زر التقدم اليومي تم إرجاعه لحالته الأصلية ==============
void MainWindow::goToProgress() {
    if (currentUserIndex >= 0) {
        progressJuz->setValue(users[currentUserIndex].currentJuz);
    }
    stack->setCurrentIndex(6);
}

// ============== زر التقرير الروحي (تم إضافة تحديث المتصدر هنا) ==============
void MainWindow::goToReport() {
    if (currentUserIndex < 0) return;

    topUserLabel->setText(getTopUserText());
    reportDisplay->setText(getSpiritualReport(currentUserIndex));

    stack->setCurrentIndex(7);
}

void MainWindow::goToCalendar() {

    stack->setCurrentIndex(8);
}

void MainWindow::goToStories() {
    stack->setCurrentIndex(9);
}

void MainWindow::goToAddThikr() {
    stack->setCurrentIndex(10);
}

void MainWindow::goToEditCal() {
    stack->setCurrentIndex(11);
}

void MainWindow::onLogout() {
    savefiles();
    currentUserIndex = -1;
    stack->setCurrentIndex(0);
}

void MainWindow::onLogoutAndSave() {
    savefiles();
    currentUserIndex = -1;
    stack->setCurrentIndex(0);
}

void MainWindow::showAthkarMorning() {
    QStringList list = getAthkarByCategory("morning");
    athkarDisplay->setText("☀️  أذكار الصباح\n\n" + list.join("\n\n"));
}

void MainWindow::showAthkarEvening() {
    QStringList list = getAthkarByCategory("evening");
    athkarDisplay->setText("🌙  أذكار المساء\n\n" + list.join("\n\n"));
}

void MainWindow::showAthkarAfter() {
    QStringList list = getAthkarByCategory("afterprayer");
    athkarDisplay->setText("🕌  أذكار بعد الصلاة\n\n" + list.join("\n\n"));
}

void MainWindow::updateTasbeehTarget() {
    if (currentUserIndex < 0) {
        tasbeehTarget->setText("الهدف: - | سبّحت: " + QString::number(tasbeehCounter) + " | باقي: -");
        return;
    }
    int goal      = users[currentUserIndex].dailyGoal;
    int remaining = goal - tasbeehCounter;
    if (remaining < 0) remaining = 0;

    tasbeehTarget->setText(
        "🎯 الهدف: " + QString::number(goal) +
        "  |  ✅ سبّحت: " + QString::number(tasbeehCounter) +
        "  |  ⏳ باقي: " + QString::number(remaining)
        );
}

void MainWindow::onTasbeeh() {
    tasbeehCounter++;
    tasbeehCount->setText(QString::number(tasbeehCounter));

    updateTasbeehTarget();

    if (currentUserIndex >= 0 &&
        tasbeehCounter == users[currentUserIndex].dailyGoal) {
        tasbeehCount->setStyleSheet(
            "color: #00FF88;"
            "font-size: 80px; font-weight: bold;"
            "border: 3px solid #00FF88; border-radius: 12px;"
            "min-width: 220px; min-height: 220px;"
            "background: #0B2411;"
            );

        if (achieveSound->isLoaded())
            achieveSound->play();

        QMessageBox::information(this, "🎉 مبروك!",
                                 "وصلت لهدفك اليومي!\n"
                                     + QString::number(users[currentUserIndex].dailyGoal) + " تسبيحة ✅\n\n"
                                                                                            "جزاك الله خيراً! 🌙");
    }
}

void MainWindow::onSaveTasbeeh() {
    if (currentUserIndex < 0) return;
    if (tasbeehCounter == 0) {
        QMessageBox::information(this, "تنبيه", "لم تسبّح بعد!");
        return;
    }

    int day = tasbeehDay->value();
    saveTasbeeh(currentUserIndex, day, tasbeehCounter);

    QMessageBox::information(this, "تم الحفظ",
                             "تم حفظ " + QString::number(tasbeehCounter) + " تسبيحة ليوم " +
                                 QString::number(day) + " 🎉");

    tasbeehCounter = 0;
    tasbeehCount->setText("0");
    tasbeehCount->setStyleSheet(
        "color: #28A745;"
        "font-size: 80px; font-weight: bold;"
        "border: 3px solid #28A745; border-radius: 12px;"
        "min-width: 220px; min-height: 220px;"
        "background: #0B2411;"
        );
    updateTasbeehTarget();
}

void MainWindow::onTasbeehComboChanged(int index) {
    if (index >= 0 && index < athkarCount)
        tasbeehThikr->setText(athkarList[index].content);
    tasbeehCounter = 0;
    tasbeehCount->setText("0");
    tasbeehCount->setStyleSheet(
        "color: #28A745; font-size: 80px; font-weight: bold;"
        "border: 3px solid #28A745; border-radius: 12px;"
        "min-width: 220px; min-height: 220px;"
        "background: #0B2411;"
        );
    updateTasbeehTarget();
}

void MainWindow::onSaveProgress() {
    if (currentUserIndex < 0) return;

    int day = progressDay->value();
    int juz = progressJuz->value();

    bool prayers[5];
    for (int i = 0; i < 5; i++)
        prayers[i] = prayerCheck[i]->isChecked();

    updatePrayers(currentUserIndex, day, prayers);
    updateJuz(currentUserIndex, juz);

    QMessageBox::information(this, "تم", "تم حفظ التقدم اليومي ✓");
}

QWidget* MainWindow::makePage_StoryImage() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(15);
    lay->setContentsMargins(40, 20, 40, 20);

    storyImageLabel = new QLabel("جاري تحميل الصورة...");
    storyImageLabel->setAlignment(Qt::AlignCenter);
    storyImageLabel->setStyleSheet("color: #FFFFFF; font-size: 20px; border: 2px dashed #28A745;");
    storyImageLabel->setMinimumSize(600, 400);

    lay->addWidget(storyImageLabel, 1);

    QPushButton *btnBack = makeBtn("🔙  رجوع للقصص", true);
    lay->addWidget(btnBack);

    connect(btnBack, &QPushButton::clicked, this, [=](){
        stack->setCurrentIndex(9);
    });

    return page;
}

QWidget* MainWindow::makePage_Stories() {
    QWidget *page = makeDarkPage();
    QVBoxLayout *lay = new QVBoxLayout(page);


    QString arNames[7] = {"المحسنين", "المتقين", "التوابين", "المتطهرين", "المتوكلين", "المقسطين", "الصابرين"};
    QString enNames[7] = {"AL mohsinin", "AL motaken", "AL tawabin", "AL motathren", "AL motwaklin", "AL moksten", "AL sabren"};

    for (int i = 0; i < 7; i++) {
        QPushButton *btn = makeBtn("الله يحب " + arNames[i]);

        connect(btn, &QPushButton::clicked, [this, i, enNames]() {

            QString basePath = "D:/last gui/editeqtwithc/photos/" + enNames[i];

            QPixmap pix;


            if (QFile::exists(basePath + ".jpeg")) {
                pix.load(basePath + ".jpeg");
            } else if (QFile::exists(basePath + ".jpg")) {
                pix.load(basePath + ".jpg");
            } else if (QFile::exists(basePath + ".png")) {
                pix.load(basePath + ".png");
            }


            if(!pix.isNull()) {
                storyImageLabel->clear();
                storyImageLabel->setPixmap(pix.scaled(storyImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                storyImageLabel->setText("تأكد أن الصورة موجودة بهذا الاسم بالضبط:\n" + enNames[i]);
            }

            stack->setCurrentIndex(12);
        });

        lay->addWidget(btn);
    }

    QPushButton *btnBack = makeBtn("🔙 رجوع", true);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::goBack);
    lay->addWidget(btnBack);

    return page;
}

void MainWindow::onAddThikr() {
    QString content  = newThikrContent->text().trimmed();
    QString category = newThikrCat->currentText();
    int     rep      = newThikrRep->value();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "خطأ", "نص الذكر لا يمكن أن يكون فارغاً.");
        return;
    }

    bool ok = addThikr(content, category, rep);
    if (ok) {
        tasbeehCombo->addItem(
            content + "  (" + QString::number(rep) + " مرة)"
            );
        QMessageBox::information(this, "تم", "تمت إضافة الذكر بنجاح ✓");
        newThikrContent->clear();
    } else {
        QMessageBox::warning(this, "خطأ", "قائمة الأذكار ممتلئة.");
    }
}

void MainWindow::onShowDay() {
    if (!calendarTable) return;

    int day = calDayInput->value();
    if (day >= 1 && day <= 30) {
        calendarTable->selectRow(day - 1);
        QTableWidgetItem *item = calendarTable->item(day - 1, 0);
        if (item) calendarTable->scrollToItem(item);
    }
}

void MainWindow::onEditDay() {

    int day = editDay->value();
    int prayerIndex = editPrayer->currentIndex();
    int h = editHour->value();
    int m = editMin->value();

    editDayPrayer(day, prayerIndex, h, m);


    QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1:%2").arg(h).arg(m));
    newItem->setTextAlignment(Qt::AlignCenter);
    calendarTable->setItem(day - 1, prayerIndex + 1, newItem);

    QMessageBox::information(this, "تم", "تم حفظ التعديل بنجاح ✓");
}