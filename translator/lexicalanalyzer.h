#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QObject>
#include <QMap>

class LexicalAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit LexicalAnalyzer(QObject *parent = nullptr, const QString textProgram = "");
    QString getLexems() const;

    QMap<QString, QString> getServiceWords() const;

    QMap<QChar, QString> getOperators() const;

    QMap<QString, QString> getMultiOperators() const;

    QMap<QString, QString> getIdentifaers() const;

    QMap<QString, QString> getDigitConsts() const;

    QMap<QString, QString> getStringConsts() const;

    QMap<QChar, QString> getSeparators() const;

private:
    QString buffer;
    QString textProgram;
    int countId;
    int countDigitConsts;
    int countStringConsts;
    QString lexems;

    QMap<QString, QString> serviceWords;
    QMap<QChar, QString> operators;
    QMap<QString, QString> multiOperators;
    QMap<QString, QString> identifaers;
    QMap<QString, QString> digitConsts;
    QMap<QString, QString> stringConsts;
    QMap<QChar, QString> separators;

    void createTables(); //Заполнение всех таблиц
    int deleteComments(int currentIndexSymbol,QString typeCom);
    bool isServiceWord (QString word);
    bool isOperator (QChar operat);
    bool isIdentifaer (QString word);
    bool isSeparators (QChar separator);
    bool isDigitConsts (QString digit);
    bool isStringConsts (QString str);
    bool isMultiOperators (QString multiop);

    void state_s(int currentIndexSymbol);   //начальное состояние
    void state_1(int currentIndexSymbol);   //если буква после начального
    void state_2(int currentIndexSymbol);   //если цифра
    void state_3(int currentIndexSymbol);   //если разделитель или операция

    void state_4(int currentIndexSymbol);   //если цифра после начального
    void state_5(int currentIndexSymbol);   //операция и комменты

    void state_6(int currentIndexSymbol);

    void state_z(); //состояние конца файла
    void state_f(); //состояние ошибки
};

#endif // LEXICALANALYZER_H
