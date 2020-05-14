#ifndef REVERSEPOLISHNOTATION_H
#define REVERSEPOLISHNOTATION_H

#include <QObject>
#include <QMap>
#include <QStack>

class ReversePolishNotation : public QObject
{
    Q_OBJECT
public:
    QMap <QString,int> priority;
    QStack <QString> stack;
    QString textProgram;
    QString result;
    QString buffer;
    QList<QString> data;
    int countOfArray = 0;
    int countOfArgs = 0;
    int countId = 0;
    int countLvl = 1;
    int countOfFunc = 1;
    QString isTypeNow = "";

    explicit ReversePolishNotation(QObject *parent = nullptr,  const QString textProgram = "");
    QString makeRPN();
private:

    void createPriorityTable();
    void state_0(int currentLexId);
    void state_1(int currentLexId);
    void state_2(int currentLexId);
    void state_3(int currentLexId);
    void state_4(int currentLexId);
    void state_5(int currentLexId);

    void state_f();
    void state_z();

signals:

public slots:
};

#endif // REVERSEPOLISHNOTATION_H
