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
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString openedFile, QString userName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (QFile::exists(openedFile) == false) {
        QMessageBox::critical(this, "TestMaker Lite", openedFile + " does not exist.", QMessageBox::Ok);
        std::abort();
    }
    testBaseDir = QFileInfo(openedFile).canonicalFilePath();
    testBaseDir = testBaseDir.left(testBaseDir.lastIndexOf("/"));
    studentName = userName;
    if (loadQuestions() == false) {
        QMessageBox::critical(this, "TestMaker Lite", openedFile + " is either corrupt or read-only. Notice, that if this is a test made in TestMaker Plus 1.0 or earlier, it is not supported.", QMessageBox::Ok);
        std::abort();
    }
    setWindowTitle(testName + " [" + studentName + " is taking a test] - TestMaker Lite " + QString(TESTMAKERLITE_VERSION));
    currentQuestion = -1;
}


void MainWindow::startTest() {
    nextQuestion();
}

void MainWindow::checkAndShowResults() {
    currentQuestion = -1;
    int testPoints = 0;
    QString message = "You've scored %POINTSCOUNT% points out of %TOTALPOINTS% points.\n";
    for (int i = 0; i < questions.size(); i++) {
        message = message + "\nQuestion " + QString::number(i + 1) + ": ";
        if (questions.at(i).userAnswer.toLower() == questions.at(i).answer.toLower()) {
            testPoints = testPoints + 1;
            message = message + "correct answer (1 point earned)";
        }
        else
            message = message + "wrong answer (you've entered \"" + questions.at(i).userAnswer + "\", but the correct answer was \"" + questions.at(i).answer + "\", no points earned)";
    }
    if (testPoints == 0)
        message = "You did all the questions wrong. Sorry.";
    else if (testPoints == questions.size())
        message = "You did everything correct. Congratulations!";
    else
        message = message.replace("%POINTSCOUNT%", QString::number(testPoints)).replace("%TOTALPOINTS%", QString::number(questions.size()));
    QDate saveDate = QDateTime::currentDateTime().date();
    QString savedFileName = testBaseDir + "/test_result_" + studentName + "_" + QString::number(saveDate.year()) + "_" + QString::number(saveDate.month()) + "_" + QString::number(saveDate.day()) + ".txt";
    QFile* fWriter = new QFile(savedFileName);
    if (fWriter->open(QIODevice::Text | QIODevice::WriteOnly))
        fWriter->write(message.toUtf8());
    else
        message = message + "\nYour results could not be saved, though. Sorry for that.";
    fWriter->close();
    delete fWriter;
    QString journal = testBaseDir + "/journal.html";
    if (QFile::exists(journal) == false) {
        QFile* journalEditorFile = new QFile(journal);
        if (journalEditorFile->open(QIODevice::Text | QIODevice::WriteOnly)) {
            QString journalHtmlWrite = "<!-- TestMaker Lite journal for test " + testName + " -->\n<html>\n<head>\n  <title>TestMaker Lite test results</title>\n</head><body><table style=\"border: 1px solid;\"><tr><td style=\"border: 1px solid; background-color: grey;\"><b>Student name</b></td><td style=\"border: 1px solid; background-color: grey;\"><b>Test taken on</b></td><td style=\"border: 1px solid; background-color: grey;\"><b>Points scored (out of " + QString::number(questions.size()) + ")</b></td><td style=\"border: 1px solid; background-color: grey;\"><b>Percentage</b></td></tr>\n<!-- TMKLITE_INSERT -->\n</table>\n</body>\n</html>";
            journalEditorFile->write(journalHtmlWrite.toUtf8());
        }
        journalEditorFile->close();
        delete journalEditorFile;
    }
    QFile* journalAppendFile1 = new QFile(journal);
    QString readJournal = "";
    if (journalAppendFile1->open(QIODevice::Text | QIODevice::ReadOnly))
        readJournal = QString(journalAppendFile1->readAll());
    journalAppendFile1->close();
    delete journalAppendFile1;
    QFile* journalAppendFile2 = new QFile(journal);
    readJournal.replace("<!-- TMKLITE_INSERT -->", "<tr><td style=\"border: 1px solid;\">" + studentName + "</td><td style=\"border: 1px solid;\">" + QDateTime::currentDateTime().toString() + "</td><td style=\"border: 1px solid;\">" + QString::number(testPoints) + "</td><td style=\"border: 1px solid;\">" + QString::number((int)((testPoints * 100) / questions.size())) + "</td></tr>\n<!-- TMKLITE_INSERT -->");
    if (journalAppendFile2->open(QIODevice::Text | QIODevice::WriteOnly))
        journalAppendFile2->write(readJournal.toUtf8());
    journalAppendFile2->close();
    delete journalAppendFile2;
    QMessageBox::information(this, "TestMaker Lite", "Results:\n" + message, QMessageBox::Ok);
    close();
}

void MainWindow::nextQuestion() {
    currentQuestion = currentQuestion + 1;
    if (currentQuestion >= questions.size()) {
        checkAndShowResults();
        return;
    }
    if (QFile::exists(questions.at(currentQuestion).image) == false) {
        QMessageBox::critical(this, "TestMaker Lite", "Image file for question " + QString::number(currentQuestion) + " does not exist.", QMessageBox::Ok);
        std::abort();
    }
    ui->imagesLabel->setPixmap(QPixmap(questions.at(currentQuestion).image).scaled(ui->imagesLabel->width(), ui->imagesLabel->height(), Qt::IgnoreAspectRatio));
    ui->lineEdit->setText("");
    ui->questionsLabel->setText("Question " + QString::number(currentQuestion + 1) + " out of " + QString::number(questions.size()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    ui->lineEdit->resize(width() - 59, ui->lineEdit->height());
    ui->lineEdit->move(ui->lineEdit->x(), height() - 47);
    ui->nextButton->move(ui->lineEdit->x() + ui->lineEdit->width() + 1, ui->lineEdit->y());
    ui->imagesLabel->resize(width() - 19, height() - 81);
    if (currentQuestion > -1 && currentQuestion < questions.size()) {
        qWarning() << currentQuestion;
        ui->imagesLabel->setPixmap(QPixmap(questions.at(currentQuestion).image).scaled(ui->imagesLabel->width(), ui->imagesLabel->height(), Qt::IgnoreAspectRatio));
    }
    ui->aboutButton->move(ui->imagesLabel->x() + ui->imagesLabel->width() - ui->aboutButton->width() - 2, ui->aboutButton->y());
    event->accept();
}

bool MainWindow::loadQuestions() {
    QFile* rFile = new QFile(testBaseDir + "/test.csv");
    if (rFile->open(QIODevice::Text | QIODevice::ReadOnly) == false) {
        rFile->close();
        delete rFile;
        return false;
    }
    QString fileContents(rFile->readAll());
    rFile->close();
    delete rFile;
    questions.clear();
    fileContents.replace("\r\n", "\n"); // files made using TestMaker Plus Editor for Windows tend to use \r\n instead of \n
    QStringList splitList = fileContents.split("\n", QString::SkipEmptyParts);
    QChar sep = ';'; // TestMaker Plus 1.0 used a comma, but newer versions of TestMaker Plus use a semicolon
    QStringList headerSplit = QString(splitList.at(0)).toLower().split(sep);
    bool endsWithScore = (headerSplit.last() == "баллы");
    int parserSizeDecrease = 1;
    if (endsWithScore)
        parserSizeDecrease = 2;
    int availableVariants = headerSplit.size() - parserSizeDecrease;
    testName = headerSplit.at(0);
    for (int i = 1; i < splitList.size(); i++) {
        QStringList lineSplit = splitList.at(i).split(sep);
        lineSplit.removeFirst();
        if (endsWithScore)
            lineSplit.removeLast();
        int variant = (qrand() % availableVariants);
        if (variant == 0 || variant > (lineSplit.size()))
            variant = 1;
        variant = variant - 1;
        TestMakerLiteQuestion questionStruct;
        questionStruct.answer = lineSplit.at(variant);
        questionStruct.image = testBaseDir + "/media/kart" + QString::number(i) + "_" + QString::number(variant + 1) + ".png";
        questions.push_back(questionStruct);
    }
    if (questions.size() < 1)
        return false;
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aboutDialog() {
    QMessageBox::information(this, "TestMaker Lite", "TestMaker Lite version " + QString(TESTMAKERLITE_VERSION) + "(running on " + detectCurrentOS() + ")\nTestMaker Plus, but it's lightweight, efficient and non-cluttered.\nCopyright (C) Tim K <timprogrammer@rambler.ru> 2018-2019. Licensed under GNU GPLv3.\n\nThis program is written in C++ and Qt version " + QString(QT_VERSION_STR) + ".");
}

void MainWindow::on_aboutButton_clicked()
{
    aboutDialog();
}

void MainWindow::on_nextButton_clicked()
{
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "TestMaker Lite", "Please answer the question first.", QMessageBox::Ok);
        return;
    }
    questions[currentQuestion].userAnswer = ui->lineEdit->text();
    nextQuestion();
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_nextButton_clicked();
}
