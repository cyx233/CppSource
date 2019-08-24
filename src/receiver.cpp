#include <QTextStream>
#include <QFile>
#include <QString>
#include <QStringList>
#include "receiver.h"
#include "command.h"

using namespace std;

Receiver::Receiver(DMFB* d): dmfb(d){
}

Receiver::~Receiver(){
}

bool Receiver::get_command(){
    QFile file(dmfb->filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        dmfb->Error(DMFB::FILE_NOT_FOUND);
    QTextStream in(&file);
    enum{
        Input = 0,
        Move,
        Split,
        Mix,
        Merge,
        Output,
    };
    QStringList command = in.readAll().split('\n',QString::SkipEmptyParts);
    if(!command.size()){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return 0;
    }
    for(int com=0; com<(int)command.size(); com++)
    {
        QStringList command_argv = command[com].split(QRegExp("\\W+"), QString::SkipEmptyParts);
        if(!command_argv.size()){
            dmfb->Error(DMFB::COMMAND_ERROR);
            return 0;
        }
        int command_id = 0;

        if(command_argv[0] == "Input"){
            command_id = Input;
        }
        else if(command_argv[0] == "Move"){
            command_id = Move;
        }
        else if(command_argv[0] == "Split"){
            command_id = Split;
        }
        else if(command_argv[0] == "Mix"){
            command_id = Mix;
        }
        else if(command_argv[0] == "Merge"){
            command_id = Merge;
        }
        else if(command_argv[0] == "Output"){
            command_id = Output;
        }
        else{
            dmfb->Error(DMFB::COMMAND_ERROR);
            return 0;
        }

        switch (command_id) {
        case Input:
        {
            int argv[4];
            for(int i=1; i<=3; i++){
                bool ok;
                argv[i] = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
            }
            if((int)dmfb->time_command.size()-1 < argv[1])
                dmfb->time_command.resize(argv[1]+1);
            dmfb->time_command[argv[1]].push_back(new InputCommand(argv[2],dmfb->row+1-argv[3]));
            break;
        }

        case Move:
        {
            int argv[6];
            for(int i=1; i<=5; i++){
                bool ok;
                argv[i] = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
            }
            if((int)dmfb->time_command.size()-1 < argv[1]+1)
                dmfb->time_command.resize(argv[1]+2);
            dmfb->time_command[argv[1]].push_back(new MoveCommand(argv[2],dmfb->row+1-argv[3],
                            argv[4],dmfb->row+1-argv[5]));
            break;
        }

        case Split:
        {
            int argv[8];
            for(int i=1; i<=7; i++){
                bool ok;
                argv[i] = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
            }
            if((int)dmfb->time_command.size()-1 < argv[1]+2)
                dmfb->time_command.resize(argv[1]+3);
            dmfb->time_command[argv[1]].push_back(new SplitCommand(argv[2],dmfb->row+1-argv[3],argv[4],
                            dmfb->row+1-argv[5],argv[6],dmfb->row+1-argv[7]));
            dmfb->time_command[argv[1]+1].push_back(new SplitCommandFinish(argv[2],dmfb->row+1-argv[3],argv[4],
                            dmfb->row+1-argv[5],argv[6],dmfb->row+1-argv[7]));
            break;
        }

        case Mix:
        {
            vector<int>argv;
            argv.push_back(0);
            for(int i=1; i<command_argv.size(); i++){
                bool ok;
                int temp = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
                argv.push_back(temp);
            }
            if(dmfb->time_command.size()-1 < argv[1]+argv.size()/2-3){
                dmfb->time_command.resize(argv[1]+argv.size()/2-2);
            }
            for(int i=argv[1];i<=argv[1]+(int)argv.size()/2-3;i++){
                int temp = i - argv[1];
                int x1 = 2 + temp*2;
                int y1 = 3 + temp*2;
                int x2 = x1 + 2;
                int y2 = y1 + 2;
                dmfb->time_command[i].push_back(new MoveCommand(argv[x1],dmfb->row+1-argv[y1],
                                                                argv[x2],dmfb->row+1-argv[y2]));
            }
            break;
        }

        case Merge:
        {
            int argv[6];
            for(int i=1; i<=5; i++){
                bool ok;
                argv[i] = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
            }
            if((int)dmfb->time_command.size()-1 < argv[1]+2)
                dmfb->time_command.resize(argv[1]+3);
            int x1 = (argv[2] + argv[4])/2;
            int y1 = (argv[3] + argv[5])/2;
            dmfb->time_command[argv[1]].push_back(new MergeCommand(x1,dmfb->row+1-y1,argv[2],
                            dmfb->row+1-argv[3],argv[4],dmfb->row+1-argv[5]));
            dmfb->time_command[argv[1]+1].push_back(new MergeCommandFinish(x1,dmfb->row+1-y1,argv[2],
                            dmfb->row+1-argv[3],argv[4],dmfb->row+1-argv[5]));
            break;
        }
        case Output:
        {
            int argv[4];
            for(int i=1; i<=3; i++){
                bool ok;
                argv[i] = command_argv[i].toInt(&ok);
                if(!ok){
                    dmfb->Error(DMFB::COMMAND_ERROR);
                    return 0;
                }
            }
            if((int)dmfb->time_command.size()-1 < argv[1])
                dmfb->time_command.resize(argv[1]+1);
            dmfb->time_command[argv[1]].push_back(new OutputCommand(argv[2],dmfb->row+1-argv[3]));
            break;
        }
        default:
            break;
        }
    }
    file.close();
    return 1;
}

