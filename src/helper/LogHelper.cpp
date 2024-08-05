#include "LogHelper.h"



LogHelper::LogHelper(QObject *parent) : QObject(parent) {
}

LogHelper::~LogHelper() = default;


void LogHelper::addlog(QString log){
    add_log(log);
    emit logChanged();
}


const QString &LogHelper::log() const
{
    return m_log;
}

void LogHelper::add_log(QString log)
{
    log += "\n";
    m_log += log;
}
