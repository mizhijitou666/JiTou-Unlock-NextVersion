#include <plistmanager.h>
#include <devicemanager.h>
#include "LinkHelper.h"

LinkHelper::LinkHelper(QObject *parent) : QObject(parent) {
}

LinkHelper::~LinkHelper() = default;



/*
 * 0:maxValueTxt
 * 1:IPCC
 */
int LinkHelper::getPlistValue_Link (int get_index){
    return 1;
    /*
    auto workspace = DeviceManager::getInstance().getCurrentWorkspace();
    if( get_index == 1)
    {
        auto location = QString::fromStdString(*workspace + "/SkipSetup/ConfigProfileDomain/Library/ConfigurationProfiles/CloudConfigurationDetails.plist");
        auto value = PlistManager::getPlistValue(location, "SkipSetup");
        if (value)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        auto location = QString::fromStdString(*workspace + "/AppleWatchUnlock/HomeDomain/Library/Preferences/com.apple.NanoRegistry.plist");
        auto value = PlistManager::getPlistValue(location, "maxPairingCompatibilityVersion");
        if (value)
        {
            return QString::number(dynamic_cast<PList::Integer *>(value)->GetValue()).toInt();
        }
        else
        {
            return 36;
        }
    }*/
}
