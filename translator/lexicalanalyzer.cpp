#include "lexicalanalyzer.h"
#include <QtDebug>
#include <QMap>

LexicalAnalyzer::LexicalAnalyzer(QObject *parent, const QString textProgram) : QObject(parent)
{
    createTables();

    countId = 0;    //колиичество идентификаторов
    countDigitConsts = 0;
    countStringConsts = 0;
    this->textProgram=textProgram;
    state_s(0);
    qDebug() << lexems;
}

QString LexicalAnalyzer::getLexems() const
{
    return lexems;
}

QMap<QString, QString> LexicalAnalyzer::getServiceWords() const
{
    return serviceWords;
}

QMap<QChar, QString> LexicalAnalyzer::getOperators() const
{
    return operators;
}

QMap<QString, QString> LexicalAnalyzer::getMultiOperators() const
{
    return multiOperators;
}

QMap<QString, QString> LexicalAnalyzer::getIdentifaers() const
{
    return identifaers;
}

QMap<QString, QString> LexicalAnalyzer::getDigitConsts() const
{
    return digitConsts;
}

QMap<QString, QString> LexicalAnalyzer::getStringConsts() const
{
    return stringConsts;
}

QMap<QChar, QString> LexicalAnalyzer::getSeparators() const
{
    return separators;
}

void LexicalAnalyzer::createTables()
{
    serviceWords.insert("if", "W1");
    serviceWords.insert("else", "W2");
    serviceWords.insert("int", "W3");
    serviceWords.insert("float", "W4");
    serviceWords.insert("return", "W5");
    serviceWords.insert("string", "W6");
    serviceWords.insert("goto", "W7");
    serviceWords.insert("void", "W8");
    serviceWords.insert("main", "W9");
    serviceWords.insert("char", "W10");
    serviceWords.insert("for", "W11");
    serviceWords.insert("while", "W12");
    serviceWords.insert("double", "W13");

    operators.insert('=', "O1");
    operators.insert('<',"O2");
    operators.insert('>',"O3");
    operators.insert('/', "O4");
    operators.insert('+', "O5");
    operators.insert('-',"O6");
    operators.insert('*', "O7");
    operators.insert('!', "O8");

    multiOperators.insert("==", "O9");
    multiOperators.insert("!=", "O10");
    multiOperators.insert("<=", "O11");
    multiOperators.insert(">=", "O12");
    multiOperators.insert("//", "O13");
    multiOperators.insert("/*", "O14");
    multiOperators.insert("*/", "O15");
    multiOperators.insert("++", "O16");
    multiOperators.insert("--", "O17");
    multiOperators.insert("+=", "O18");
    multiOperators.insert("-=", "O19");
    multiOperators.insert("&&", "O20");
    multiOperators.insert("||", "O21");

    separators.insert(';', "R1");
    separators.insert('{', "R2");
    separators.insert('}', "R3");
    separators.insert('(', "R4");
    separators.insert(')', "R5");
    separators.insert('[', "R6");
    separators.insert(']', "R7");
    separators.insert(',', "R8");
    separators.insert(':', "R9");
    separators.insert('\"', "R10");
    separators.insert('\'', "R11");
    separators.insert(' ', "R13");
    separators.insert('\\', "R14");
    separators.insert('\n', "R15");
}

int LexicalAnalyzer::deleteComments(int currentIndexSymbol, QString typeCom)
{
    if (typeCom == "//") {
        while (textProgram[currentIndexSymbol]!="\n")
            currentIndexSymbol++;
    } else {
        while (!(textProgram[currentIndexSymbol] == '*' && textProgram[currentIndexSymbol+1] == '/')) {
            qDebug() << textProgram[currentIndexSymbol];
            currentIndexSymbol++;
        }
        currentIndexSymbol+=2;
    }
    return currentIndexSymbol;
}

bool LexicalAnalyzer::isServiceWord(QString word)
{
    return serviceWords.contains(word);
}

bool LexicalAnalyzer::isOperator(QChar operat)
{
    return operators.contains(operat);
}

bool LexicalAnalyzer::isIdentifaer(QString id)
{
    return identifaers.contains(id);
}

bool LexicalAnalyzer::isSeparators(QChar separator)
{
    return separators.contains(separator);
}

bool LexicalAnalyzer::isDigitConsts(QString digit)
{
    return digitConsts.contains(digit);
}

bool LexicalAnalyzer::isStringConsts(QString str)
{
    return stringConsts.contains(str);
}

bool LexicalAnalyzer::isMultiOperators(QString multiop)
{
    return multiOperators.contains(multiop);
}

void LexicalAnalyzer::state_s(int currentIndexSymbol)
{
    if (textProgram[currentIndexSymbol].isLetter()) {
        buffer+=textProgram[currentIndexSymbol];
        state_1(++currentIndexSymbol);
    } else if (textProgram[currentIndexSymbol].isDigit() || textProgram[currentIndexSymbol] == '.') {
        buffer+=textProgram[currentIndexSymbol];
        state_4(++currentIndexSymbol);
    } else if (isOperator(textProgram[currentIndexSymbol])) {
        buffer+=textProgram[currentIndexSymbol];
        state_5(++currentIndexSymbol);
    } else if (isSeparators(textProgram[currentIndexSymbol])) {
        buffer+=textProgram[currentIndexSymbol];
        state_6(++currentIndexSymbol);
    } else if (textProgram.length()<=currentIndexSymbol)
        state_z();
    else {
        state_f(); // если все условия не выполнились
    }
}

void LexicalAnalyzer::state_1(int currentIndexSymbol)
{
    if (textProgram[currentIndexSymbol].isLetter()){
        buffer+=textProgram[currentIndexSymbol];
        state_1(++currentIndexSymbol);
    } else if (textProgram[currentIndexSymbol].isNumber()){
        buffer+=textProgram[currentIndexSymbol];
        state_2(++currentIndexSymbol);
    } else if (isSeparators(textProgram[currentIndexSymbol]) || isOperator(textProgram[currentIndexSymbol])){
        state_3(currentIndexSymbol);
    } else
        state_f();
}

void LexicalAnalyzer::state_2(int currentIndexSymbol)
{
    if (textProgram[currentIndexSymbol].isLetterOrNumber()){
        buffer+=textProgram[currentIndexSymbol];
        state_2(++currentIndexSymbol);
    } else if (isSeparators(textProgram[currentIndexSymbol]) || isOperator(textProgram[currentIndexSymbol])){
        if (!identifaers.contains(buffer)) {
            identifaers.insert(buffer,"I"+ QString::number(++countId));
            lexems+=identifaers.value(buffer)+" ";
            buffer="";
            state_s(currentIndexSymbol);
        } else {
            lexems+=identifaers.value(buffer)+" ";
            buffer="";
            state_s(currentIndexSymbol);
        }
    } else
        state_f();
}

void LexicalAnalyzer::state_3(int currentIndexSymbol)
{
//    qDebug() << buffer;
    if (isServiceWord(buffer)) {
        lexems+=(serviceWords.value(buffer) + " ");
        buffer="";
        state_s(currentIndexSymbol);
    } else
        state_2(currentIndexSymbol);
}

void LexicalAnalyzer::state_4(int currentIndexSymbol)
{
    if (textProgram[currentIndexSymbol].isDigit()) {
        buffer+=textProgram[currentIndexSymbol];
        state_4(++currentIndexSymbol);
    } else if (textProgram[currentIndexSymbol]==".") {
        while (!isSeparators(textProgram[currentIndexSymbol])) {
            buffer+=textProgram[currentIndexSymbol++];
        }
        state_4(currentIndexSymbol);
    } else if (isSeparators(textProgram[currentIndexSymbol]) || isOperator(textProgram[currentIndexSymbol])){
        if (!isDigitConsts(buffer)) {
            digitConsts.insert(buffer,"N"+ QString::number(++countDigitConsts));
            lexems+=digitConsts.value(buffer)+ " ";
            buffer="";
        } else {
            lexems+=digitConsts.value(buffer)+ " ";
            buffer="";

        }
        state_s(currentIndexSymbol);
    } else {
        state_f();
    }
}

void LexicalAnalyzer::state_5(int currentIndexSymbol)
{
    if (isOperator(textProgram[currentIndexSymbol])) {
        buffer+=textProgram[currentIndexSymbol];
        if (isMultiOperators(buffer)) {
            if (buffer=="//" || buffer=="/*") {
                currentIndexSymbol = deleteComments(++currentIndexSymbol, buffer);
                buffer="";
                state_s(currentIndexSymbol);
            } else {
                lexems+=multiOperators.value(buffer) + " ";
                buffer="";
                state_s(++currentIndexSymbol);
            }

        }
    } else {
        lexems+=operators.value(buffer[0])+" ";
        buffer="";
        state_s(currentIndexSymbol);
    }
}

void LexicalAnalyzer::state_6(int currentIndexSymbol)
{
    if (buffer=="\n") {
        lexems+=buffer;
        buffer="";
        state_s(currentIndexSymbol);
    } else if (buffer==" ") {
        lexems+=buffer;
        buffer="";
        state_s(currentIndexSymbol);
    } else if (buffer=="\'" || buffer=="\"") {
        lexems+=separators.value(buffer[0]) + " ";
        QString tempBuffer = "";
        while (textProgram[currentIndexSymbol]!=buffer) {
            tempBuffer+=textProgram[currentIndexSymbol];
            ++currentIndexSymbol;
        }
        if (!isStringConsts(tempBuffer)) {
            stringConsts.insert(tempBuffer,"C"+ QString::number(++countStringConsts));
            lexems+=stringConsts.value(tempBuffer)+ " ";
        } else {
            lexems+=stringConsts.value(buffer)+ " ";
        }
        lexems+=separators.value(buffer[0]) + " ";
        buffer="";
        state_s(++currentIndexSymbol);
    } else {
        lexems+=separators.value(buffer[0])+" ";
        buffer="";
        state_s(currentIndexSymbol);
    }

}

void LexicalAnalyzer::state_z()
{
    qDebug() << "END" ;
}

void LexicalAnalyzer::state_f()
{
    qDebug() << "Ошибка";
}
