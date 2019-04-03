/*
    TestMaker Lite - an open-source test system that is compatible with TestMaker Plus
    Copyright (C) 2019 Tim K <timprogrammer@rambler.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include <QVector>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QtDebug>
#include <QResizeEvent>
#include <cstdlib>

#define TESTMAKERLITE_VERSION QString(QString::number(TESTMAKERLITE_VERSION_MAJOR) + "." + QString::number(TESTMAKERLITE_VERSION_MINOR) + " (update " + QString::number(TESTMAKERLITE_VERSION_UPDATE) + ")")

namespace Ui {
class MainWindow;
}

inline QByteArray fetchFile(const QString& url) {
    QNetworkAccessManager* mgr = new QNetworkAccessManager(nullptr);
    QNetworkReply* reply = mgr->get(QNetworkRequest(url));
    QEventLoop* dlLoop = new QEventLoop(nullptr);
    QObject::connect(reply, SIGNAL(finished()), dlLoop, SLOT(quit()));
    dlLoop->exec();
    QByteArray result = reply->readAll();
    reply->close();
    delete reply;
    delete dlLoop;
    delete mgr;
    return result;
}

inline QString detectCurrentOS() {
#ifdef Q_OS_WIN
    return "Microsoft Windows";
#elif defined(Q_OS_MACOS)
    return "Apple Mac OS X";
#elif defined(Q_OS_LINUX)
    if (QFile::exists("/etc/debian_version")) {
        if (QFile::exists("/usr/bin/ubuntu-drivers"))
            return "Ubuntu Linux";
        return "Debian GNU/Linux";
    }
    else if (QFile::exists("/etc/mandriva-release")) {
        if (QFile::exists("/bin/dnf") || QFile::exists("/usr/bin/dnf")) {
            if (QFile::exists("/etc/profile.d/10mageia-release.sh"))
                return "Mageia Linux";
            else
                return "OpenMandriva Lx";
        }
        return "Mandriva Linux";
    }
    else if (QFile::exists("/etc/redhat-release")) {
        if (QFile::exists("/usr/bin/dnf") == false)
            return "RedHat Enterprise Linux";
        return "Fedora Linux";
    }
    else if (QFile::exists("/etc/SuSE-release"))
        return "SUSE Linux Enterprise";
    return "Linux";
#elif defined(Q_OS_FREEBSD)
    return "FreeBSD";
#elif defined(Q_OS_OPENBSD)
    return "OpenBSD";
#elif defined(Q_OS_HAIKU)
    return "Haiku OS";
#else
    return "Unknown operating system";
#endif
}

inline bool fetchAndSaveFile(const QString& url, const QString& destination) {
    QByteArray saveCtnt = fetchFile(url);
    QFile* fWriter = new QFile(destination);
    bool result = true;
    if (fWriter->open(QIODevice::Text | QIODevice::WriteOnly))
        fWriter->write(saveCtnt);
    else
        result = false;
    fWriter->close();
    delete fWriter;
    return result;
}

struct TestMakerLiteQuestion {
    QString image;
    QString answer;
    QString userAnswer;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString openedFile, QString userName, QWidget *parent = 0);
    ~MainWindow();
    bool loadQuestions();
    void startTest();
    void nextQuestion();
    void checkAndShowResults();
    void aboutDialog();
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_aboutButton_clicked();

    void on_nextButton_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QString testBaseDir;
    QString studentName;
    int currentQuestion;
    QString testName;
    QVector<TestMakerLiteQuestion> questions;
};

#endif // MAINWINDOW_H
