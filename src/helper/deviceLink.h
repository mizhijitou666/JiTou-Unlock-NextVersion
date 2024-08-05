#ifndef DEVICELINK_H
#define DEVICELINK_H

#include <QObject>
#include <QAbstractListModel>
#include "singleton.h"

class DeviceInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DeviceInfoModel(QObject *parent = nullptr);

    //重写下面的函数，一定要重写
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QHash<int,QByteArray>roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //下面是往这个list添加的函数
    Q_INVOKABLE void append(const QString &title,const QString &name,const QString &version);
    Q_INVOKABLE QString get(int index,int type);
    Q_INVOKABLE bool loadDevices();
    void remove(int index);

private:
    enum DeviceInfoRoles{
        UUIDRole,
        NameRole,
        VersionRole,
    };

    struct DeviceInfo{
        QString UUID_;
        QString Name_;
        QString Version_;
    };

    QList<DeviceInfo>dataList_;

};

class CurrentInfo : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid
                   WRITE setuuid NOTIFY uuidChanged)
    Q_PROPERTY(QString name READ name
                   WRITE setname NOTIFY nameChanged)
    Q_PROPERTY(QString version READ version
                   WRITE setversion NOTIFY versionChanged)
    Q_PROPERTY(bool isDeviceAvailable READ isDeviceAvailable
                   WRITE setisDeviceAvailable NOTIFY isDeviceAvailableChanged)

private:
    explicit CurrentInfo(QObject *parent = nullptr);
    QString m_uuid;
    QString m_name;
    QString m_version;
    bool m_isDeviceAvailable;

signals:
    void uuidChanged();
    void nameChanged();
    void versionChanged();
    void isDeviceAvailableChanged();

public:
    SINGLETON(CurrentInfo)
    ~CurrentInfo() override;
    void init(char *argv[]);
    Q_INVOKABLE void set(QString uuid,QString name,QString version,bool isDeviceAvailable);
    const QString &uuid() const;
    void setuuid(QString uuid);
    const QString &name() const;
    void setname(QString name);
    const QString &version() const;
    void setversion(QString version);
    const bool &isDeviceAvailable() const;
    void setisDeviceAvailable(bool isDeviceAvailable);
};


#endif
