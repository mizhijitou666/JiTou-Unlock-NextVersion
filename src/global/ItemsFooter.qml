pragma Singleton

import QtQuick 2.15
import FluentUI 1.0

FluObject{

    property var navigationView

    id:footer_items

    FluPaneItemSeparator{}
    FluPaneItem{
        title:qsTr("Q&A")
        icon:FluentIcons.ReadingMode
        url:"qrc:/page/T_Help.qml"
        onTap:{
            navigationView.push(url)
        }
    }
    FluPaneItemSeparator{
        spacing:10
        size:1
    }
    FluPaneItem{
        title:qsTr("Settings")
        icon:FluentIcons.Settings
        url:"qrc:/page/T_Settings.qml"
        onTap:{
            navigationView.push(url)
        }
    }

}
