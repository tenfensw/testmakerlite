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

#include "mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>
#include <iostream>

#define canFindArgument(in) (arguments.contains(QString(in), Qt::CaseInsensitive) && (arguments.lastIndexOf(QString(in)) + 1) < arguments.size())
#define argumentValue(in) (arguments.at(arguments.lastIndexOf(QString(in)) + 1))

QString fetchNetworkTest(const QString& url2) {
    QString url = QString(url2).replace("test.csv", "");
    if (url.contains("http://") == false && url.contains("https://") == false)
        return "";
    QString temporaryDir = QDir::tempPath() + "/tmklite-networktest-" + QString::number(qrand());
    if (QDir(temporaryDir).exists() && QDir(temporaryDir).removeRecursively() == false)
        return "";
    QDir().mkdir(temporaryDir);
    QDir().mkdir(temporaryDir + "/media");
    if (fetchAndSaveFile(url + "/test.csv", temporaryDir + "/test.csv") == false)
        return "";
    QFile* readTestCsv = new QFile(temporaryDir + "/test.csv");
    if (readTestCsv->open(QIODevice::Text | QIODevice::ReadOnly) == false) {
        readTestCsv->close();
        delete readTestCsv;
        return "";
    }
    QString wholeTestCsv = readTestCsv->readAll();
    wholeTestCsv.replace("\r\n", "\n");
    readTestCsv->close();
    delete readTestCsv;
    if (wholeTestCsv.toLower().startsWith("error"))
        return "";
    QChar networkTestsSep = ';';
    if (wholeTestCsv.contains(networkTestsSep) == false)
        return "";
    int questionsCount = wholeTestCsv.split("\n", QString::SkipEmptyParts).size() - 1;
    if (questionsCount < 1)
        return "";
    for (int i = 1; i <= questionsCount; i++) {
        QString kartName = "kart" + QString::number(i) + "_1.png";
        if (fetchAndSaveFile(url + "/media/" + kartName, temporaryDir + "/media/" + kartName) == false)
            return "";
    }
    return QString(temporaryDir + "/test.csv");
}

void showHelp() {
    std::cerr << "TestMaker Lite - open-source test system that is capable of parsing TestMaker Plus tests" << std::endl;
    std::cerr << "Copyright (c) 2019 Tim K <timprogrammer@rambler.ru>. Licensed under GNU GPLv3." << std::endl;
    std::cerr << std::endl;
    std::cerr << "Usage: testmakerlite [arguments]" << std::endl;
    std::cerr << "  -f [path to test.csv]           directly open test specified in the" << std::endl;
    std::cerr << "                                  second argument" << std::endl;
    std::cerr << "  -network [test URL]             take an online test with the URL specified" << std::endl;
    std::cerr << "                                  in the second argument" << std::endl;
    std::cerr << "  -fullscreen                     run in full-screen mode" << std::endl;
    std::cerr << "  -help                           display this help message and exit" << std::endl;
    std::cerr << "Please report bugs to <timprogrammer@rambler.ru>." << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList arguments = a.arguments();
    if (arguments.contains("--help", Qt::CaseInsensitive) || arguments.contains("-help", Qt::CaseInsensitive) || arguments.contains("-h", Qt::CaseInsensitive) || arguments.contains("-?", Qt::CaseInsensitive)) {
        showHelp();
        return 0;
    }
    bool fullScreen = arguments.contains("-fullscreen", Qt::CaseInsensitive);
    QString fileToOpen = "";
    if (canFindArgument("-f"))
        fileToOpen = argumentValue("-f");
    else if (canFindArgument("-network"))
        fileToOpen = fetchNetworkTest(argumentValue("-network"));
    if (fileToOpen.isEmpty())
        fileToOpen = QFileDialog::getOpenFileName(nullptr, "TestMaker Lite", QDir::homePath(), "TestMaker Plus 1.1 test files (test.csv)");
    if (fileToOpen.isEmpty())
        return 1;
    QString name = QInputDialog::getText(nullptr, "TestMaker Lite", "What is your name?");
    if (name.isEmpty())
        return 1;
    MainWindow w(fileToOpen, name);
    if (fullScreen)
        w.showFullScreen();
    else
        w.show();
    w.startTest();

    return a.exec();
}
