import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FluentUI 1.0

FluLauncher {
    id: app
    Connections{
        target: FluTheme
        function onDarkModeChanged(){
            SettingsHelper.saveDarkMode(FluTheme.darkMode)
        }
    }
    Connections{
        target: FluTheme
        function onAccentColorChanged(){
            SettingsHelper.saveAccentColor(FluTheme.accentColor.normal)
        }
    }
    Component.onCompleted: {
        FluApp.init(app)
        FluApp.useSystemAppBar = false
        FluTheme.accentColor = FluColors.createAccentColor(SettingsHelper.getAccentColor())
        FluTheme.darkMode = SettingsHelper.getDarkMode()
        FluTheme.animationEnabled = true
        FluApp.windowIcon = "qrc:/logo.ico"
        FluRouter.routes = {
            "/":"qrc:/main.qml",
        }
        FluRouter.navigate("/")
    }
}
