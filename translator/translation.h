#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QMap>
#include <QStack>

class Translation : public QObject
{
    Q_OBJECT
public:
    explicit Translation(QObject *parent = nullptr, QString rpn = "", QMap <QString, QString>* lexems = nullptr);
    QString getResult();
private:
    int pCount = 0; // счетчик вспомогательных переменных
    QList <QString> RPN; // список лексем из опз
    QStack <QString> stack;
    QMap <QString, QString> *lexems;
    bool isCycle = false;
    bool isCondition = false;
    bool isMain = false;
    QString func = "";
    QString declareFunc = "";
    QString result = "";
    QString buffer = "";
    QString var = "";
    QString nameFanc = "";

    QString getKeyByLexems(QString value);
    void addOperationToOperands(QString operation);
    QString buildArgs(QString fanc);
    void translate();

signals:

public slots:
};

#endif // TRANSLATION_H
