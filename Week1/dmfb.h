#ifndef DMFB_H
#define DMFB_H

#include <QObject>
#include <vector>
#include "command.h"
using std::vector;

class MainWindow;
class State;

struct Item{
   int id=0;
   int type=0;
};

class DMFB : public QObject
{
    Q_OBJECT
public:
    enum{
        TABLE_SMALL_ERROR = 0,
        TABLE_BIG_ERROR,
        TOO_MANY_INPUT,
        FILE_NOT_FOUND
    };
    enum{
        NOTHING = 0,
        INPUT,
        OUTPUT,
        WASH_PUT
    };
    const int length = 50;

    explicit DMFB(QObject *parent = nullptr);
    ~DMFB();

    int row=-1;
    int col=-1;
    int in_num=0;
    int out_num=1;
    int wash_num=0;
    QString filename;
    vector<vector<Item>> table;
    vector<vector<Item>> copy;

    void set_state(State* s);
    int get_state();

    void doCommand(Command* command);
    void undo();
    void redo();
    void show();

private:
    State* state;
    vector<Command>raw_command_list;
    vector<vector<Command>>time_command;
    int time_now, life_span;

signals:
    void MainUpdate();
    void Error(int);

public slots:
    void ReceiveData(int, int, int, QString);
    void next_state();
};

#endif // DMFB_H
