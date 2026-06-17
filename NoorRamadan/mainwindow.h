#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSoundEffect>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTableWidget>
#include "logic.h"



#define BG_COLOR \
"QWidget { background-color: #0A0F0D; }" \
    "QMessageBox { background-color: #0A0F0D; }" \
    "QMessageBox QLabel { color: #FFFFFF; font-size: 16px; font-family: 'Tajawal', 'Cairo', sans-serif; background-color: transparent; }" \
    "QMessageBox QPushButton { color: #FFFFFF; background-color: #0B2411; border: 1px solid #28A745; border-radius: 6px; padding: 8px 25px; font-weight: bold; font-family: 'Tajawal', 'Cairo', sans-serif; }" \
    "QMessageBox QPushButton:hover { background-color: #123A1B; }"

#define BTN_STYLE \
    "QPushButton {" \
    "  color: #FFFFFF;" \
    "  background-color: #0B2411;" \
    "  border: 1px solid #28A745;" \
    "  border-radius: 6px;" \
    "  padding: 14px 20px;" \
    "  font-size: 16px;" \
    "  font-family: 'Tajawal', 'Cairo', sans-serif;" \
    "  font-weight: bold;" \
    "}" \
    "QPushButton:hover {" \
    "  background-color: #123A1B;" \
    "  border: 1px solid #34D058;" \
    "}"


#define BTN_RED_STYLE \
    "QPushButton {" \
    "  color: #FF7A7A;" \
    "  background-color: #1A0D0D;" \
    "  border: 1px solid #FF4D4D;" \
    "  border-radius: 6px;" \
    "  padding: 14px 20px;" \
    "  font-size: 16px;" \
    "  font-family: 'Tajawal', 'Cairo', sans-serif;" \
    "  font-weight: bold;" \
    "}" \
    "QPushButton:hover {" \
    "  background-color: #331A1A;" \
    "}"


#define INPUT_STYLE \
    "QLineEdit, QSpinBox, QComboBox {" \
    "  color: #FFFFFF;" \
    "  background-color: #0A0F0D;" \
    "  border: 1px solid #28A745;" \
    "  border-radius: 6px;" \
    "  padding: 10px 15px;" \
    "  font-size: 15px;" \
    "  font-family: 'Tajawal', 'Cairo', sans-serif;" \
    "}"


#define COMBO_STYLE \
    "QComboBox {" \
    "  color: #FFFFFF;" \
    "  background-color: #0A0F0D;" \
    "  border: 1px solid #28A745;" \
    "  border-radius: 6px;" \
    "  padding: 10px 15px;" \
    "  font-size: 15px;" \
    "  font-family: 'Tajawal', 'Cairo', sans-serif;" \
    "  min-height: 45px;" \
    "}" \
    "QComboBox QAbstractItemView {" \
    "  color: #FFFFFF;" \
    "  background-color: #0A0F0D;" \
    "  selection-background-color: #123A1B;" \
    "  border: 1px solid #28A745;" \
    "}" \
    "QComboBox::drop-down {" \
    "  border: none;" \
    "}"

    class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stack;
    QSoundEffect *achieveSound;
    QMediaPlayer *azanPlayer;       // لفك تشفير وتشغيل ملف الـ MP3
    QAudioOutput *azanAudioOutput;  // للتحكم في الأجهزة الصوتية ومستوى الصوت

    QWidget* makePage_Welcome();
    QWidget* makePage_Login();
    QWidget* makePage_SignUp();
    QWidget* makePage_Main();
    QWidget* makePage_Athkar();
    QWidget* makePage_Tasbeeh();
    QWidget* makePage_Progress();
    QWidget* makePage_Report();
    QWidget* makePage_Calendar();
    QWidget* makePage_Stories();
    QWidget* makePage_AddThikr();
    QWidget* makePage_EditCal();
    QTableWidget *calendarTable;

    QWidget* makeDarkPage();
    QPushButton* makeBtn(QString text, bool red = false);
    QLabel* makeTitle(QString text);
    QLineEdit* makeInput(QString placeholder, bool password = false);
    QComboBox* makeCombo();


    QLabel    *athkarTitle;
    QTextEdit *athkarDisplay;


    QLabel    *tasbeehCount;
    QLabel    *tasbeehThikr;
    QLabel    *tasbeehTarget;
    QSpinBox  *tasbeehDay;
    QComboBox *tasbeehCombo;
    int        tasbeehCounter;

    QSpinBox  *progressDay;
    QSpinBox  *progressJuz;
    QCheckBox *prayerCheck[5];

    QTextEdit *reportDisplay;

    QSpinBox  *calDayInput;

    QWidget* makePage_StoryImage();
    QLabel *storyImageLabel;

    QLineEdit *newThikrContent;
    QComboBox *newThikrCat;
    QSpinBox  *newThikrRep;

    QSpinBox  *editDay;
    QComboBox *editPrayer;
    QSpinBox  *editHour;
    QSpinBox  *editMin;

    QLabel *topUserLabel;
    QLabel *currentDailyPointsLabel;


private slots:

    void goToLogin();
    void goToSignUpFromWelcome();
    void onExitApp();


    void onLogin();
    void goToSignUp();


    void onSignUp();

    void goToMain();
    void goToAthkar();
    void goToTasbeeh();
    void goToProgress();
    void goToReport();
    void goToCalendar();
    void goToStories();
    void goToAddThikr();
    void goToEditCal();
    void goBack();
    void onLogout();
    void onLogoutAndSave();

    void showAthkarMorning();
    void showAthkarEvening();
    void showAthkarAfter();

    void onTasbeeh();
    void onSaveTasbeeh();
    void onTasbeehComboChanged(int index);

    void onSaveProgress();

    void onAddThikr();

    void onEditDay();
    void updateTasbeehTarget();
    void onShowDay();
    void showPrayMessage(QString prayName);

};

#endif