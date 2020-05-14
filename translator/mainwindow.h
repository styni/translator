#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lexicalanalyzer.h"
#include "reversepolishnotation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isServiceWords();

private slots:
    void on_pushButton_clicked();
    void uploadTables(LexicalAnalyzer *lexem);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
