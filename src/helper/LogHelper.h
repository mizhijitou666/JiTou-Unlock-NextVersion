#ifndef LOGHELPER_H
#define LOGHELPER_H
#include <QObject>
#include "singleton.h"


class LogHelper : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString log READ log
                   WRITE addlog NOTIFY logChanged)

private:
    explicit LogHelper(QObject *parent = nullptr);
    QString m_log;

signals:
    void logChanged();

public:
    SINGLETON(LogHelper)
    ~LogHelper() override;
    void init(char *argv[]);
    Q_INVOKABLE void addlog(QString log);
    const QString &log() const;
    void add_log(QString log);
};




#endif // LOGHELPER_H
