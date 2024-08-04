#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqml.h>
#include <QSettings>
#include <QScopedPointer>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include "singleton.h"

class LinkHelper : public QObject {
    Q_OBJECT
private:
    explicit LinkHelper(QObject *parent = nullptr);
public:
    SINGLETON(LinkHelper)
    ~LinkHelper() override;
    void init(char *argv[]);
    Q_INVOKABLE int getPlistValue_Link (int get_index);
};
