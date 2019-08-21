#include <QTextStream>
#include <QFile>
#include <QString>
#include "receiver.h"
#include "command.h"

using namespace std;

Receiver::Receiver(DMFB* d): dmfb(d){
}

Receiver::~Receiver(){
}

void Receiver::get_command(){
    QFile file(dmfb->filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        dmfb->Error(dmfb->FILE_NOT_FOUND);
    QTextStream in(&file);
    QString command;
    while(in.atEnd()){
        command = in.readLine();

    }
}

