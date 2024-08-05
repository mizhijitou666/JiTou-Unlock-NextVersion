pragma Singleton

import QtQuick 2.15
import FluentUI 1.0

FluObject{

    property var navigationView

    FluPaneItem{
        id:item_home
        title: qsTr("Home")
        icon: FluentIcons.Home
        url: "qrc:/page/T_Home.qml"
        onTap: {
            navigationView.push(url)
        }
    }

    FluPaneItemSeparator{
        spacing:10
        size:1
    }

    FluPaneItem{
        id: item_expander_Remove_pairing
        title: qsTr("Remove the pairing restriction")
        icon: FluentIcons.Unlock
        url: "qrc:/page/T_Remove.qml"
        disabled: !CurrentInfo.isDeviceAvailable
        onTap: {
            navigationView.push(url)
        }

    }
    FluPaneItemSeparator{
        spacing:10
        size:1
    }

    FluPaneItem{
        id: item_expander_ad_block
        title: qsTr("Apply")
        icon: FluentIcons.CheckboxComposite
        url: "qrc:/page/T_Apply.qml"
        disabled: !CurrentInfo.isDeviceAvailable
        onTap: {
            navigationView.push(url)
        }
    }
}
