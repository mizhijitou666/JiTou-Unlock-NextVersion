#include <plistmanager.h>
#include <QFile>
#include "LinkHelper.h"
#include "deviceLink.h"
#include "LogHelper.h"

LinkHelper::LinkHelper(QObject *parent) : QObject(parent) {
}

LinkHelper::~LinkHelper() = default;



/*
 * 0:maxValueTxt
 * 1:IPCC
 */
int LinkHelper::getPlistValue_Link (int get_index){
    auto workspace = currentWorkspace::getInstance()->getCurrentWorkspace();
    if( get_index == 1)
    {
        auto location = QString(workspace + "/SkipSetup/ConfigProfileDomain/Library/ConfigurationProfiles/CloudConfigurationDetails.plist");
        auto value = PlistManager::getPlistValue(location, "SkipSetup");
        if (value)
        {
            return dynamic_cast<PList::Array *>(value)->GetSize() != 0;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        auto location = QString(workspace + "/AppleWatchUnlock/HomeDomain/Library/Preferences/com.apple.NanoRegistry.plist");
        auto value = PlistManager::getPlistValue(location, "maxPairingCompatibilityVersion");
        if (value)
        {
            return QString::number(dynamic_cast<PList::Integer *>(value)->GetValue()).toInt();
        }
        else
        {
            LogHelper::getInstance()->addlog(tr("read max_value plist fail"));
            return 36;
        }
    }
}
void LinkHelper::set_maxPlistValue(const QString &text)
{
    auto workspace = currentWorkspace::getInstance()->getCurrentWorkspace();
    auto location = QString(workspace + "/AppleWatchUnlock/HomeDomain/Library/Preferences/com.apple.NanoRegistry.plist");
    QFileInfo fileInfo(location);
    if(!fileInfo.isFile())
    {
        LogHelper::getInstance()->addlog(tr("save max_value plist fail"));
    }
    if (!text.isEmpty())
    {
        auto node = PList::Integer(text.toLongLong());
        PlistManager::setPlistValue(location, "maxPairingCompatibilityVersion", node);
    }
}
void LinkHelper::set_skipSetup(bool checked)
{
    auto workspace = currentWorkspace::getInstance()->getCurrentWorkspace();
    auto location = QString(workspace + "/SkipSetup/ConfigProfileDomain/Library/ConfigurationProfiles/CloudConfigurationDetails.plist");
    if (checked)
    {
        // auto node1 = PList::Boolean(checked);
        // PlistManager::setPlistValue(location, "CloudConfigurationUIComplete", node1);

        auto strings = {
                        "Location",
                        "WiFi",
                        "Restore",
                        "SIMSetup",
                        "Android",
                        "AppleID",
                        "IntendedUser",
                        "TOS",
                        "Siri",
                        "ScreenTime",
                        "Diagnostics",
                        "SoftwareUpdate",
                        "Passcode",
                        "Biometric",
                        "Payment",
                        "Zoom",
                        "DisplayTone",
                        "MessagingActivationUsingPhoneNumber",
                        "HomeButtonSensitivity",
                        "CloudStorage",
                        "ScreenSaver",
                        "TapToSetup",
                        "Keyboard",
                        "PreferredLanguage",
                        "SpokenLanguage",
                        "WatchMigration",
                        "OnBoarding",
                        "TVProviderSignIn",
                        "TVHomeScreenSync",
                        "Privacy",
                        "TVRoom",
                        "iMessageAndFaceTime",
                        "AppStore",
                        "Safety",
                        "Multitasking",
                        "ActionButton",
                        "TermsOfAddress",
                        "AccessibilityAppearance",
                        "Welcome",
                        "Appearance",
                        "RestoreCompleted",
                        "UpdateCompleted"};

        auto node2 = PList::Array();
        for (const auto &str : strings)
        {
            node2.Append(new PList::String(str));
        }

        PlistManager::setPlistValue(location, "SkipSetup", node2);
    }
    else
    {
        // PlistManager::deletePlistKey(location, "CloudConfigurationUIComplete");
        PlistManager::deletePlistKey(location, "SkipSetup");
    }
}
