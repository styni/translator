#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString textProgram = ui->input_text->toPlainText();
    LexicalAnalyzer *lexem = new LexicalAnalyzer(parent(),textProgram);
    QString result = lexem->getLexems();
    ui->lexem_out->setText(result);
    QMap <QString, QString> allLexems=uploadTables(lexem);

    ReversePolishNotation *RPN = new ReversePolishNotation(parent(),result);
    QString rpn = RPN->makeRPN();
    ui->reverse_polish_natation->setText(rpn);

    Translation *translate = new Translation(parent(),rpn, &allLexems);
    ui->pascal_text->setText(translate->getResult());
}

QMap <QString, QString> MainWindow::uploadTables(LexicalAnalyzer *lexem)
{

    QMap <QString, QString> allLexems;

    //Загрузка зарезервированных слов

    QMap <QString, QString> serviceWords =  lexem->getServiceWords();
    int rowCount = 0;

    QMap <QString,QString>::iterator i;
    allLexems = serviceWords;
    ui->tableServiceWords->setRowCount(serviceWords.size());
    ui->tableServiceWords->setColumnCount(2);
    for (i = serviceWords.begin();i!=serviceWords.end();i++){

        ui->tableServiceWords->setItem(rowCount,1,new QTableWidgetItem(i.key()));
        ui->tableServiceWords->setItem(rowCount,0,new QTableWidgetItem(i.value()));
        rowCount++;
    }
    ui->tableServiceWords->sortByColumn(0,Qt::AscendingOrder);
    ui->tableServiceWords->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableServiceWords->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));
//    ui->tableServiceWords->resizeColumnsToContents();

    //Загрузка операторов

    QMap <QChar, QString> operators =  lexem->getOperators();
    rowCount = 0;
    QMap <QString, QString> multiOperators = lexem->getMultiOperators();
    QMap <QChar,QString>::iterator j;

    ui->tableOperators->setRowCount(operators.size()+multiOperators.size());
    ui->tableOperators->setColumnCount(2);
    for (j = operators.begin();j!=operators.end();j++){
        allLexems.insert(j.key(),j.value());
        ui->tableOperators->setItem(rowCount,1,new QTableWidgetItem(j.key()));
        ui->tableOperators->setItem(rowCount,0,new QTableWidgetItem(j.value()));
        rowCount++;
    }

    for (i = multiOperators.begin();i!=multiOperators.end();i++){
        allLexems.insert(i.key(),i.value());
        ui->tableOperators->setItem(rowCount,1,new QTableWidgetItem(i.key()));
        ui->tableOperators->setItem(rowCount,0,new QTableWidgetItem(i.value()));
        rowCount++;
    }
    ui->tableOperators->sortByColumn(0,Qt::AscendingOrder);
    ui->tableOperators->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableOperators->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));
//    ui->tableOperators->resizeColumnsToContents();

    //Загрузка разделителей

    QMap <QChar, QString> separators=lexem->getSeparators();
    rowCount = 0;
    ui->tableSeparators->setRowCount(separators.size());
    ui->tableSeparators->setColumnCount(2);
    for (j = separators.begin();j!=separators.end();j++){
        allLexems.insert(j.key(),j.value());
        ui->tableSeparators->setItem(rowCount,1,new QTableWidgetItem(j.key()));
        ui->tableSeparators->setItem(rowCount,0,new QTableWidgetItem(j.value()));
        rowCount++;
    }
    ui->tableSeparators->sortByColumn(0,Qt::AscendingOrder);
    ui->tableSeparators->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableSeparators->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));
//    ui->tableSeparators->resizeColumnsToContents();

    //Загрузка строковых констант

    rowCount = 0;
    QMap <QString, QString> stringConsts=lexem->getStringConsts();

    ui->tableStringConsts->setRowCount(stringConsts.size());
    ui->tableStringConsts->setColumnCount(2);
    for (i = stringConsts.begin();i!=stringConsts.end();i++){
        allLexems.insert(i.key(),i.value());
        ui->tableStringConsts->setItem(rowCount,1,new QTableWidgetItem(i.key()));
        ui->tableStringConsts->setItem(rowCount,0,new QTableWidgetItem(i.value()));
        rowCount++;
    }
    ui->tableStringConsts->sortByColumn(0,Qt::AscendingOrder);
    ui->tableStringConsts->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableStringConsts->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));
//    ui->tableStringConsts->resizeColumnsToContents();

    //Загрузка числовых констант

    rowCount = 0;
    QMap <QString, QString> digitConsts=lexem->getDigitConsts();

    ui->tableDigitConsts->setRowCount(digitConsts.size());
    ui->tableDigitConsts->setColumnCount(2);
    for (i = digitConsts.begin();i!=digitConsts.end();i++){
        allLexems.insert(i.key(),i.value());
        ui->tableDigitConsts->setItem(rowCount,1,new QTableWidgetItem(i.key()));
        ui->tableDigitConsts->setItem(rowCount,0,new QTableWidgetItem(i.value()));
        rowCount++;
    }
    ui->tableDigitConsts->sortByColumn(0,Qt::AscendingOrder);
    ui->tableDigitConsts->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableDigitConsts->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));

//    ui->tableDigitConsts->resizeColumnsToContents();

    //Загрузка идентификаторов

    rowCount = 0;
    QMap <QString, QString> identifaers=lexem->getIdentifaers();

    ui->tableIdentifaers->setRowCount(identifaers.size());
    ui->tableIdentifaers->setColumnCount(2);
    for (i = identifaers.begin();i!=identifaers.end();i++){
        allLexems.insert(i.key(),i.value());
        ui->tableIdentifaers->setItem(rowCount,1,new QTableWidgetItem(i.key()));
        ui->tableIdentifaers->setItem(rowCount,0,new QTableWidgetItem(i.value()));
        rowCount++;
    }
    ui->tableIdentifaers->sortByColumn(0,Qt::AscendingOrder);
    ui->tableIdentifaers->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->tableIdentifaers->setHorizontalHeaderItem(1,new QTableWidgetItem("Слово"));
//    ui->tableIdentifaers->resizeColumnsToContents();

    return allLexems;
}


