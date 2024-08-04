#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqml.h>
#include <QSettings>
#include <QScopedPointer>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include "qcolor.h"
#include "singleton.h"

class SettingsHelper : public QObject {
    Q_OBJECT
private:
    explicit SettingsHelper(QObject *parent = nullptr);

public:
    SINGLETON(SettingsHelper)
    ~SettingsHelper() override;
    void init(char *argv[]);
    Q_INVOKABLE void saveDarkMode(int darkModel) {
        save("darkMode", darkModel);
    }
    Q_INVOKABLE int getDarkMode() {
        return get("darkMode", QVariant(0)).toInt();
    }
    Q_INVOKABLE void saveAccentColor(QString accentColor) {
        save("accentColor", accentColor);
    }
    Q_INVOKABLE QString getAccentColor() {
        return get("accentColor", QVariant("#00b294")).toString();
    }

private:
    void save(const QString &key, QVariant val);
    QVariant get(const QString &key, QVariant def = {});

private:
    QScopedPointer<QSettings> m_settings;
};
