#ifndef TASK_H
#define TASK_H

#include <QtCore>

namespace task {
    typedef enum {
        NORMAL,
        BREAK,
        UNKNOWN
    } Type;
    static const char * TypeNames[3] = {"NORMAL","BREAK","UNKNOWN"};

    typedef enum {
        ACTIVE,
        ENDED
    } State;
    static const char * StateNames[2] = {"ACTIVE","ENDED"};
}

class Task
{
public:
    Task(QString name, QDateTime startDateTime, long timeElapsed, task::Type type = task::NORMAL, task::State state = task::ACTIVE);
    QString toCSVLine(QChar separator = ';');

    QString name;
    QDateTime startDateTime;
    QDateTime endedDateTime;
    long timeElapsed;

private:
    task::Type _type;
    task::State _state;
    bool restored;

public:
    inline task::Type type(){ return _type; }
    inline task::State state(){ return _state; }
    inline const char * typeString(){ return task::TypeNames[_type]; }
    inline const char * stateString(){ return task::StateNames[_state]; }
    inline bool isRestored(){ return restored; }
    void setRestored();
    inline void ended(){ _state = task::ENDED; endedDateTime= QDateTime::currentDateTime();}
};

#endif // TASK_H


