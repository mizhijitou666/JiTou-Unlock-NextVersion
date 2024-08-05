#include"deviceLink.h"
#include "utils.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/restore.h>
#include <plist/plist.h>


DeviceInfoModel::DeviceInfoModel(QObject *parent)
{

}


/**ListView需要获取的数量*/

int DeviceInfoModel::rowCount(const QModelIndex &parent) const
{
    return dataList_.size();
}

//下面的函数用于qml中，List可以直接使用别名获取数据
QHash<int,QByteArray> DeviceInfoModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[UUIDRole] = "UUID_";
    roles[NameRole] = "Name_";
    roles[VersionRole] = "Version_";
    return roles;
}


//重新View获取数据函数
QVariant DeviceInfoModel::data(const QModelIndex &index,int role) const
{
    int row = index.row();
    if(row < 0 || row >= dataList_.count()){
        return QVariant();
    }

    const DeviceInfo &data = dataList_[row];
    switch(role){
    case UUIDRole:
        return data.UUID_;
    case NameRole:
        return data.Name_;
    case VersionRole:
        return data.Version_;
    default:
        return QVariant();
    }
}

void DeviceInfoModel::append(const QString &UUID,const QString &Name,const QString &Version)
{
    DeviceInfo data;
    data.UUID_ = UUID;
    data.Name_ = Name;
    data.Version_ = Version;

    //更改数据之后需要通报一下QML，才能让数据实时刷新

    emit beginInsertRows(QModelIndex(),dataList_.size(),dataList_.size());
    dataList_.append(data);
    emit endInsertRows();
}

void DeviceInfoModel::remove(int index)
{
    if(index < 0 || index >= dataList_.count())
    {
        return;
    }
    emit beginRemoveRows(QModelIndex(),index,index);
    dataList_.removeAt(index);
    emit endRemoveRows();
}
/*
 * get函数
 * type：
 * 1 为 name
 * 2 为 uuid
 * 3 为 版本
 */
QString DeviceInfoModel::get(int index,int type)
{
    if(index < 0 || index >= dataList_.count())
    {
        return NULL;
    }
    switch (type) {
    case 1:
        return dataList_.at(index).Name_;
    case 2:
        return dataList_.at(index).UUID_;
    case 3:
        return dataList_.at(index).Version_;
    default:
        return NULL;
    }
}

bool DeviceInfoModel::loadDevices(){
    emit beginRemoveRows(QModelIndex(),0,dataList_.size());
    dataList_.clear();
    emit endRemoveRows();
    bool isDeviceAvailable = false;
    char **device_list = nullptr;
    int device_count = 0;
    idevice_get_device_list(&device_list, &device_count);

    for (int i = 0; i < device_count; ++i)
    {
        char *uuid = device_list[i];

        if (uuid)
        {
            idevice_t device = nullptr;
            idevice_error_t error = idevice_new_with_options(&device, uuid, IDEVICE_LOOKUP_USBMUX);
            lockdownd_client_t client = nullptr;
            lockdownd_error_t lockdownd_error = lockdownd_client_new_with_handshake(device, &client, "DeviceManager");

            if (error == IDEVICE_E_SUCCESS && lockdownd_error == LOCKDOWN_E_SUCCESS)
            {
                plist_t device_info = nullptr;
                lockdownd_error_t info_error = lockdownd_get_value(client, nullptr, "ProductVersion", &device_info);

                if (info_error == LOCKDOWN_E_SUCCESS)
                {
                    char *device_version = nullptr;
                    plist_get_string_val(device_info, &device_version);

                    plist_t device_name_plist = nullptr;
                    lockdownd_error_t name_error = lockdownd_get_value(client, nullptr, "DeviceName", &device_name_plist);

                    if (name_error == LOCKDOWN_E_SUCCESS && device_name_plist && plist_get_node_type(device_name_plist) == PLIST_STRING)
                    {
                        char *device_name = nullptr;
                        plist_get_string_val(device_name_plist, &device_name);
                        DeviceInfo data;
                        data.UUID_ = uuid;
                        data.Name_ = (device_name != nullptr) ? device_name : "";
                        data.Version_ = (device_version != nullptr) ? QString::fromStdString(Version(device_version).toString()) : "";
                        isDeviceAvailable = true;
                        emit beginInsertRows(QModelIndex(),dataList_.size(),dataList_.size());
                        dataList_.append(data);
                        emit endInsertRows();

                        if (device_name)
                            free(device_name);
                    }

                    if (device_name_plist)
                        plist_free(device_name_plist);

                    if (device_version)
                        free(device_version);
                }

                if (device_info)
                    plist_free(device_info);
            }

            if (client)
                lockdownd_client_free(client);

            if (device)
                idevice_free(device);
        }
    }

    if (device_list)
        idevice_device_list_free(device_list);
    return isDeviceAvailable;
}

CurrentInfo::CurrentInfo(QObject *parent) : QObject(parent) {
}

CurrentInfo::~CurrentInfo() = default;

void CurrentInfo::set(QString uuid,QString name,QString version,bool isDeviceAvailable)
{
    setuuid(uuid);
    setname(name);
    setversion(version);
    setisDeviceAvailable(isDeviceAvailable);
    emit uuidChanged();
    emit nameChanged();
    emit versionChanged();
    emit isDeviceAvailableChanged();
}
const QString &CurrentInfo::uuid() const
{
    return m_uuid;
}
const QString &CurrentInfo::name() const
{
    return m_name;
}
const QString &CurrentInfo::version() const
{
    return m_version;
}
const bool &CurrentInfo::isDeviceAvailable() const
{
    return m_isDeviceAvailable;
}

void CurrentInfo::setuuid(QString uuid)
{
    this->m_uuid = uuid;
}
void CurrentInfo::setname(QString name)
{
    this->m_name = name;
}
void CurrentInfo::setversion(QString version)
{
    this->m_version = version;
}
void CurrentInfo::setisDeviceAvailable(bool isDeviceAvailable)
{
    this->m_isDeviceAvailable = isDeviceAvailable;
}
