#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <QMainWindow>

#include<QFile>
#include<QFileDialog>
#include<QTextStream>
#include<QMessageBox>
#include<QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrinter>
#include<QString>

#include<iostream>
#include<fstream>
#include<math.h>
#include<string>
#include<time.h>
#include<vector>
#include<sstream>
#include<unistd.h>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class SpellChecker; }
QT_END_NAMESPACE

class SpellChecker : public QMainWindow
{
    Q_OBJECT

public:
    SpellChecker(QWidget *parent = nullptr);
    ~SpellChecker();

private:
    Ui::SpellChecker *ui;

private slots:
    void BttnCheckPressed();
};
#endif // SPELLCHECKER_H
