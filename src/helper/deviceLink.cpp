#include"deviceLink.h"
#include "utils.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/restore.h>
#include <plist/plist.h>
#include <QCoreApplication>
#include "LogHelper.h"
#include <QTranslator>
#include <qdir.h>
#include "CreateBackup.h"
#include <QProcess>
#include <QApplication>
#include <synchapi.h>

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
    roles[isSupportRole] = "isSupport_";
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
    case isSupportRole:
        return data.isSupport_;
    default:
        return QVariant();
    }
}

void DeviceInfoModel::append(const QString &UUID,const QString &Name,const QString &Version,const bool &isSupport)
{
    DeviceInfo data;
    data.UUID_ = UUID;
    data.Name_ = Name;
    data.Version_ = Version;
    data.isSupport_= isSupport;

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
bool DeviceInfoModel::get_isSupport(int index)
{
    if(index < 0 || index >= dataList_.count())
    {
        return NULL;
    }
    return dataList_.at(index).isSupport_;
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
                        data.isSupport_ = Version(device_version) >= Version(15) ? true : false ;
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

void CurrentInfo::set(QString uuid,QString name,QString version,bool isDeviceAvailable,bool isSupport)
{
    setuuid(uuid);
    setname(name);
    setversion(version);
    setisDeviceAvailable(isDeviceAvailable);
    setisSupport(isSupport);
    emit uuidChanged();
    emit nameChanged();
    emit versionChanged();
    emit isDeviceAvailableChanged();
    emit isSupportChanged();
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
const bool &CurrentInfo::isSupport() const
{
    return m_isSupport;
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
void CurrentInfo::setisSupport(bool isSupport)
{
    this->m_isSupport = isSupport;
}


currentWorkspace::currentWorkspace(QObject *parent) : QObject(parent) {
}

currentWorkspace::~currentWorkspace() = default;

void currentWorkspace::setCurrentWorkspace(QString str)
{
    this->Workspace = str;
}

void currentWorkspace::configureWorkspace(QString uuid)
{
    // Get the destination directory path
    auto workspaceDir = QCoreApplication::applicationDirPath() + "/JiTou/Workspace/" + uuid;
    // Set the source directory path (assuming it's located in the binary directory)
    auto sourceDir = QCoreApplication::applicationDirPath() + "/files";

    if (Utils::copyDirectory(sourceDir, workspaceDir))
    {
       LogHelper::getInstance()->addlog(tr("Create file success"));
       LogHelper::getInstance()->addlog(tr("Workspace Change to ") + uuid);
       currentWorkspace::getInstance()->set_isWorkspace_make(true);        // fix this idk
    }
    else
    {
        LogHelper::getInstance()->addlog(tr("Create file fail"));
        currentWorkspace::getInstance()->set_isWorkspace_make(false);
    }

    setCurrentWorkspace(workspaceDir);
}

void currentWorkspace::q_setCurrentWorkspace(QString uuid)
{
    configureWorkspace(uuid);
}

const QString currentWorkspace::getCurrentWorkspace() const
{
    return this->Workspace;
}

const bool &currentWorkspace::isWorkspace_make() const
{
    return m_isWorkspace_make;
}


void currentWorkspace::set_isRunning(bool isRunning)
{
    this->m_isRunning = isRunning;
    emit isRunning_Changed();
}
const bool &currentWorkspace::isRunning() const
{
    return m_isRunning;
}


void currentWorkspace::set_isWorkspace_make(bool isWorkspace_make)
{
    this->m_isWorkspace_make = isWorkspace_make;
    emit isWorkspace_make_Changed();
}
void currentWorkspace::setTweakEnabled(Tweak t, bool enabled = true)
{
    if (enabled)
    {
        this->enabledTweaks.insert(t);
    }
    else
    {
        this->enabledTweaks.erase(t);
    }
}

bool currentWorkspace::isTweakEnabled(Tweak t)
{
    return this->enabledTweaks.find(t) != this->enabledTweaks.end();
}
void currentWorkspace::resetCurrentDevice(bool device)
{
    if (!device || !m_isWorkspace_make)
    {
        this->m_isWorkspace_make = false;
        this->enabledTweaks.clear();
    }
    else
    {
        this->enabledTweaks.clear();
        this->enabledTweaks.insert(Tweak::SkipSetup);
        this->enabledTweaks.insert(Tweak::AppleWatchUnlock);
    }
}

std::vector<Tweak> currentWorkspace::getEnabledTweaks()
{
    auto tweaks = std::vector<Tweak>(this->enabledTweaks.begin(), this->enabledTweaks.end());

    // Sort the vector based on the tweak description
    std::sort(tweaks.begin(), tweaks.end(), [](auto a, auto b)
              { return Tweaks::getTweakData(a).description < Tweaks::getTweakData(b).description; });

    return tweaks;
}

QString currentWorkspace::q_getEnabledTweaks()
{
    auto labelText = std::string();
    auto tweaks = getEnabledTweaks();
    if (tweaks.empty())
    {
        labelText = "没有项目";
    }
    else
    {
        std::ostringstream labelTextStream;
        bool firstTweak = true;
        for (auto t : tweaks) {
            if (!firstTweak) {
                labelTextStream << "\n";
            } else {
                firstTweak = false;
            }
            labelTextStream << Tweaks::getTweakData(t).description;
        }
        labelText = labelTextStream.str();
    }
    return QString::fromStdString(labelText);
}
void currentWorkspace::removeTweaks(QString uuid) {
    LogHelper::getInstance()->addlog(tr("Coping the modify file"));
    auto sourceDir = QCoreApplication::applicationDirPath() + "/restore";
    auto enabledTweaksDirectoryPath = QCoreApplication::applicationDirPath() + "/JiTou/EnabledTweaks";
    auto enabledTweaksDirectory = QDir(enabledTweaksDirectoryPath);
    if (enabledTweaksDirectory.exists())
    {
        enabledTweaksDirectory.removeRecursively();
    }

    if (Utils::copyDirectory(sourceDir, enabledTweaksDirectoryPath))
    {
        LogHelper::getInstance()->addlog(tr("Create restore workspace success"));// fix this idk
    }
    else
    {
        LogHelper::getInstance()->addlog(tr("Create restore workspace fail"));
        return;
    }

    auto backupDirectoryPath = QCoreApplication::applicationDirPath() + "/JiTou/Backup";

    auto success_Create = CreateBackup::createBackup(enabledTweaksDirectoryPath, backupDirectoryPath);
    if (success_Create) {
        //
    } else {
        LogHelper::getInstance()->addlog(tr("Fail to CreateBackup file"));
        return;
    }

    LogHelper::getInstance()->addlog(tr("Restoring modify file to the device"));

    restoreBackupToDevice(uuid, QCoreApplication::applicationDirPath().toStdString()+ "/JiTou");
}

void currentWorkspace::applyTweaks(QString uuid)
{
    auto workspace = currentWorkspace::getCurrentWorkspace();

    // Erase backup folder
    auto enabledTweaksDirectoryPath = QCoreApplication::applicationDirPath() + "/JiTou/EnabledTweaks";
    auto enabledTweaksDirectory = QDir(enabledTweaksDirectoryPath);
    if (enabledTweaksDirectory.exists())
    {
        enabledTweaksDirectory.removeRecursively();
    }
    LogHelper::getInstance()->addlog(tr("Coping the modify file"));
    for (auto t : currentWorkspace::getEnabledTweaks())
    {
        auto folderName = Tweaks::getTweakData(t).folderName;
        if (Utils::copyDirectory(workspace + "/" + QString::fromStdString(folderName), enabledTweaksDirectoryPath))
        {
            LogHelper::getInstance()->addlog(tr("Create restore workspace success"));// fix this idk
        }
        else
        {
            LogHelper::getInstance()->addlog(tr("Create restore workspace fail"));
            return;
        }
    }

    auto backupDirectoryPath = QCoreApplication::applicationDirPath() + "/JiTou/Backup";

    auto success_Create = CreateBackup::createBackup(enabledTweaksDirectoryPath, backupDirectoryPath);
    if (success_Create) {
        //
    } else {
        LogHelper::getInstance()->addlog(tr("Fail to CreateBackup file"));
        return;
    }

    LogHelper::getInstance()->addlog(tr("Writing modify file to the device"));

    restoreBackupToDevice(uuid, QCoreApplication::applicationDirPath().toStdString()+ "/JiTou");
}

void currentWorkspace::restoreBackupToDevice(QString uuid, const std::string & backupDirectory)
{
    QStringList arguments;
    arguments << "-u" << uuid << "-s" << "Backup" << "restore" << "--system" << "--skip-apps" << QString::fromStdString(backupDirectory);
    QProcess *process = new QProcess(this);
    process->start("idevicebackup2.exe", arguments);
    process->waitForStarted();
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [process, this](int exitCode, QProcess::ExitStatus exitStatus){
                Q_UNUSED(exitCode);
                Q_UNUSED(exitStatus);
                process->close();
                process->kill();
                process->deleteLater();
                this->set_isRunning(false);
            });
    connect(process,&QProcess::readyRead,this,[process](){
        QString output = process->readAllStandardOutput();
        QString errorOutput = process->readAllStandardError();
        LogHelper::getInstance()->addlog(output);
        LogHelper::getInstance()->addlog(errorOutput);
        if(output.contains("Code 211"))
        {
            LogHelper::getInstance()->addlog(tr("Error!! Please Close \"Find my iphone\"!!"));
        }
        else if (output.contains("Code 1"))
        {
            LogHelper::getInstance()->addlog(tr("Error!! Please Remove All your MDM File!!"));
        }
        else if (output.contains("Code 205"))
        {
            LogHelper::getInstance()->addlog(tr("Error!! NOT Support For below iOS 15.2!!"));
        }
        else if (output.contains("Restore Successful"))
        {
            LogHelper::getInstance()->addlog(tr("BIG Successful \n Your Device will be restart after a min \n Dont worry it will be fine after restart!"));
        }
    });
}
