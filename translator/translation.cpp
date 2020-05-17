#include "translation.h"
#include <QDebug>

Translation::Translation(QObject *parent, QString rpn, QMap <QString,QString> *lexems) : QObject(parent)
{
    RPN = rpn.split(" ");
    RPN.removeAll("");
    stack = *new QStack <QString>;
    this->lexems = lexems;

    translate();
    qDebug() << "Паскаль:" << result;
}

QString Translation::getResult()
{
    return result;
}

QString Translation::getKeyByLexems(QString value)
{
    if (value=="W3") {
        return "integer";
    } else if (value=="W4" || value=="W13") {
        return "real";
    } else if (value=="O1") {
        return ":=";
    } else if (value=="O4") {
        return "div";
    } else if (value=="O8") {
        return "not";
    } else if (value=="O9") {
        return "=";
    } else if (value=="O10") {
        return "<>";
    } else if (value=="O20") {
        return "and";
    } else if (value=="O21") {
        return "or";
    } else {
        return lexems->key(value,value);
    }
}

void Translation::addOperationToOperands(QString operation)
{
    QString secondOperand = stack.pop();
    QString firstOperand = stack.pop();
    stack.push(firstOperand + " " +  getKeyByLexems(operation) + " " + secondOperand);
}

QString Translation::buildArgs(QString fanc)
{
    QString buffer="(";
    int countVar =fanc.replace("Ф","").toInt();
    QList<QString> listOperands;
    for (int i=0;i<countVar;i++){
        listOperands.append(stack.pop());
    }
    nameFanc = listOperands.takeLast();
    for(int i=0; i < countVar - 1; i++){
        buffer+= listOperands.takeLast() +",";
    }
    buffer.replace(buffer.length()-1,1,")");
    buffer.insert(0,nameFanc + " ");
    return buffer;
}

void Translation::translate()
{
    for (QString lex : RPN) {
        if (lex[0]=="I" || lex[0]=="C" || lex[0]=="N"){
            stack.push(getKeyByLexems(lex));
        } else if (lex.contains("Ф")) {
            func=buildArgs(lex);
            stack.push(func);
//            stack.push(lex);
        } else if (!stack.isEmpty() && stack.top()==func && lex[0]=="W") {
            declareFunc=stack.pop();
            // объявление функции
            if (lex=="W8") {
                declareFunc.insert(0,"procedure ");
                declareFunc+=";\n";
            } else {
                declareFunc.insert(0,"function ");
                declareFunc+=": " + getKeyByLexems(lex) + ";\n";
            }
        } else if (lex.toInt()) {
            stack.push(lex);
        } else if (lex=="НП") {
            stack.pop();
            stack.pop();
            buffer="begin\n";
        } else if (lex=="КП") {
            stack.pop();
            stack.pop();
            buffer+="end";
            if (isMain){
                buffer+=".\n";
                result.insert(0,"var\n" + var);
                result += buffer;
            } else {
                buffer+=";\n";
                result+=declareFunc + "var\n" + var + buffer;
            }

            var="";
            buffer="";
        } else if (lex[0]=="O"){
            if (lex=="O1") {
                addOperationToOperands(lex);
                buffer+=stack.pop() + ";\n";
            } else {
                addOperationToOperands(lex);
            }

        } else if (lex[0]=="W") {
            if (!stack.isEmpty() && stack.top().toInt() && lex!="W5") {
                int countVar = stack.pop().toInt();
                for(int i=0;i<countVar;i++){
                    var+=stack.pop() + ",";
                }
                var.replace(var.length()-1, 1,": " + getKeyByLexems(lex) + ";\n");
            } else if (lex=="W5") {
                if (!isMain) {
                    buffer+=nameFanc + " := " + stack.pop() + ";\n";
                }
            } else if (lex=="W9") {
                isMain=true;
            } else {
                if (!stack.isEmpty()) {
                    QString temp = stack.pop() + ": " + getKeyByLexems(lex);
                    stack.push(temp);
                }
            }
        } else if (lex=="M1") {
            buffer+="if (" + stack.pop() + ") then begin\n";
        } else if (lex=="M1:" && !isCondition) {
            buffer+="end;\n";
        } else if (lex=="M2") {
            buffer+="end;\nelse begin\n";
            isCondition = true;
        } else if (lex=="M2:") {
            buffer+="end;\n";
            isCondition = false;
        } else if (lex=="Ц1") {
            buffer+="while (" + stack.pop() + ") do begin\n";
        } else if (lex=="Ц1:") {
            buffer+="end;\n";
        }


    } // конец всего цикла
}



