#include "reversepolishnotation.h"
#include "QDebug"

ReversePolishNotation::ReversePolishNotation(QObject *parent, const QString textProgram) : QObject(parent)
{
    createPriorityTable();
    this->textProgram=textProgram;
    QList<QString> temp = textProgram.split("\n");
    for(int i=0;i<temp.length();i++){
        data.append(temp[i].split(" "));
    }
    data.removeAll("");
    foreach (QString i, data) {

        qDebug() << i;
    }

    state_0(0);
    qDebug() << "ОПЗ = " << buffer;
}

QString ReversePolishNotation::makeRPN()
{
    return buffer;
}

void ReversePolishNotation::createPriorityTable()
{
    priority.insert("W1", 0);       // if
    priority.insert("W12",0);       // while
    priority.insert("R4", 0);       // (
    priority.insert("R6", 0);       // [
    priority.insert("R2", 0);       // {
    priority.insert("АЭМ", 0);      // АЭМ
    priority.insert("Ф", 0);        // Ф

    priority.insert("R8", 1);        // ,
    priority.insert("R1", 1);        // ;
    priority.insert("R5", 1);        // )
    priority.insert("R7", 1);        // ]
    priority.insert("R3", 1);        // }
    priority.insert("W2", 1);        // else

    priority.insert("O1", 2);        // =
    priority.insert("W7", 2);        // goto

    priority.insert("O21", 3);       // ||

    priority.insert("O20", 4);       // &&

    priority.insert("O8", 5);        // !

    priority.insert("O3", 6);        // >
    priority.insert("O2", 6);        // <
    priority.insert("O12", 6);       // >=
    priority.insert("O11", 6);       // <=
    priority.insert("O9", 6);        // ==
    priority.insert("O10", 6);       // !=

    priority.insert("O5", 7);        // +
    priority.insert("O6", 7);        // -
    priority.insert("O18",7);        // +=
    priority.insert("O19",7);        // -=

    priority.insert("O4", 8);        // /
    priority.insert("O7", 8);        // *

}

void ReversePolishNotation::state_0(int currentLexId)
{
    qDebug() << currentLexId ;
    if (currentLexId >= data.length()){
            state_z();
    } else if (data[currentLexId][0]=="I") {    // если идентификатор, то просто пишем
        buffer+=data[currentLexId] + " ";
        if (data[currentLexId+1]=="R4") {
            countOfArgs++;
            stack.push("Ф");
        }
        state_0(++currentLexId);
    } else if (data[currentLexId][0]=="O") { // если оператор
        state_1(currentLexId);
    }  else if (data[currentLexId]=="W1" || (stack.contains("W1") && data[currentLexId]=="R2")
                || (stack.contains("W1") && data[currentLexId]=="R3")) {  // if
        if (stack.contains("W1")) {
            state_2(currentLexId);
//            countOfArray++;
        } else {
            stack.push(data[currentLexId]);
            state_0(++currentLexId);
        }
    } else if (data[currentLexId]=="W12" || (stack.contains("W12") && data[currentLexId]=="R2")
               || (stack.contains("W12") && data[currentLexId]=="R3")) {  // while
       if (stack.contains("W12")) {
           state_5(currentLexId);
//           countOfArray++;
       } else {
           buffer+= "Ц2: ";
           stack.push(data[currentLexId]);
           state_0(++currentLexId);
        }
    } else if (data[currentLexId][0]=="R") {
        state_3(currentLexId);
    } else if (data[currentLexId][0]=="W") {
//        stack.push(data[currentLexId]);
        state_4(currentLexId);
    } else if (!priority.contains(data[currentLexId])) {
        buffer+=data[currentLexId] + " ";
        state_0(++currentLexId);
    }  else {
        state_f();  // error
    }


}

void ReversePolishNotation::state_1(int currentLexId)
{
    if (priority.contains(data[currentLexId])) {

        while (!stack.isEmpty() && priority.value(stack.top()) >= priority.value(data[currentLexId])) {
                buffer+= stack.pop() + " ";
        }

        stack.push(data[currentLexId]);
        state_0(++currentLexId);
    } else {
        buffer += data[currentLexId] + " ";
        state_0(++currentLexId);
    }
}

void ReversePolishNotation::state_2(int currentLexId)
{
    if (data[currentLexId]=="R2") {
        buffer += "M1 УПЛ ";
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R3") {
        if (!stack.contains("W2") && currentLexId!=data.length()-1) {
            if (data[currentLexId+1]=="W2") {
                buffer += "M2 БП M1: ";
                stack.push(data[currentLexId+1]);
                state_0(currentLexId+3);
            }
        } else if (stack.contains("W2")) {
            buffer += "M2: ";
            while(!stack.isEmpty() && stack.top()!="W1"){
                stack.pop();
            }
            if (!stack.isEmpty())
                stack.pop();
            state_0(++currentLexId);
        } else {
            buffer += "M1: ";
            while(!stack.isEmpty() && stack.top()!="W1"){
                stack.pop();
            }
            if (!stack.isEmpty())
                stack.pop();
            state_0(++currentLexId);
        }


    }
}

void ReversePolishNotation::state_3(int currentLexId) // разделители
{
    if (data[currentLexId]=="R1"){ // ;

        if (stack.contains(isTypeNow)) {
            if (isTypeNow == "W5") {
                buffer+= stack.pop() + " "  + isTypeNow + " ";
                if (!stack.isEmpty())
                    stack.pop();
                countId=0;
                isTypeNow="";
            } else {
                if (stack.contains("O1")) {
                    buffer+= stack.pop() + " " + QString::number(countId) + " " ;
                    buffer+= stack.pop() + " ";
                    countId=0;
                    isTypeNow="";
                } else {
                    buffer+= QString::number(countId) + " " + stack.pop() + " ";
                    countId=0;
                    isTypeNow="";
                }
            }
        } else {
            while (!stack.isEmpty() && priority.value(stack.top()) > priority.value(data[currentLexId])) {
                buffer+= stack.pop() + " ";
            }
        }
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R4") {
        stack.push(data[currentLexId]);
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R5") {
        while (!stack.isEmpty() && stack.top()!="R4") {
            buffer+= stack.pop() + " ";
        }
        if (!stack.isEmpty())
            stack.pop();
        if (stack.contains("Ф")) {
            buffer+= QString::number(++countOfArgs) + stack.pop() + " ";
            countOfArgs=0;
        }
        countId=0;
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R6") {
        countOfArray++;
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R7") {
        countOfArray++;
        while (!stack.isEmpty()){
            if (priority.value(stack.top()) >= priority.value(data[currentLexId])) {
                buffer+=stack.pop() + " ";
            }
        }
        buffer += QString::number(countOfArray) + "АЭМ";
        countOfArray=0;
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R8") {  // ,
        if (countOfArray > 0) {
            countOfArray++;
        } else if (stack.contains("Ф")) {
            countOfArgs++;
        } else if (countId > 0) {
            countId++;
        }
        if (!stack.contains("Ф")) {
            while (!stack.isEmpty() && priority.value(stack.top(), 0) >= priority.value(data[currentLexId])){

                    buffer+=stack.pop() + " ";

            }
        } else {
            while (!stack.isEmpty() && priority.value(stack.top(), 1) >= priority.value(data[currentLexId])){

                    buffer+=stack.pop() + " ";

            }
        }
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R2") {
        while (!stack.isEmpty() && priority.value(stack.top(), 0) >= priority.value(data[currentLexId])){

                buffer+=stack.pop() + " ";

        }
        buffer+= QString::number(countOfFunc) + " " + QString::number(countLvl) + " НП ";
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R3") {
        buffer+= QString::number(countOfFunc++) + " " + QString::number(countLvl) + " КП ";
        while(!stack.isEmpty()) {
            buffer += stack.pop() + " ";
        }
        state_0(++currentLexId);
    }
}

void ReversePolishNotation::state_4(int currentLexId)
{
    if (data[currentLexId]=="W3" || data[currentLexId]=="W4" || data[currentLexId]=="W6"
            || data[currentLexId]=="W10" || data[currentLexId]=="W13" || data[currentLexId]=="W9" || data[currentLexId]=="W5") {
        isTypeNow = data[currentLexId];
        countId++;
        stack.push(data[currentLexId]);

    } else {
        buffer += data[currentLexId] + " ";
    }
    state_0(++currentLexId);
}

void ReversePolishNotation::state_5(int currentLexId)
{
    if (data[currentLexId]=="R2") {
        buffer += "Ц1 УПЛ ";
        state_0(++currentLexId);
    } else if (data[currentLexId]=="R3") {
        while (!stack.isEmpty() && stack.top()!="W12"){
            buffer += stack.pop() + " ";
        }
        if (!stack.isEmpty())
            stack.pop();
        buffer+= "Ц2 БП Ц1:";

        state_0(++currentLexId);
    }
}

void ReversePolishNotation::state_f()
{
    qDebug() << "Ошибка";
}

void ReversePolishNotation::state_z()
{
    qDebug() << "END";

    while(!stack.isEmpty()) {
        buffer += stack.pop() + " ";
    }
}


// пример для прогона
//int fact(int x){
//return x*fact(x--);
//}
//void main(){
//int a=0,sum=0;
//while(a<10){
//sum+=fact(a++);
//}
//if (sum>100){
//sum-=100;
//} else {
//sum+=100;
//}
//}
