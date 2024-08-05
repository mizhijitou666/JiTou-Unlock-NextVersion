import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0
import "./global"

FluWindow {
    id:windows
    width: 1020
    height: 668
    minimumWidth: 1020
    minimumHeight: 668
    maximumWidth: 1020
    maximumHeight: 668
    title: qsTr("JiTou-Unlock-NextVersion-master")
    launchMode: FluWindowType.SingleTask
    fitsAppBarWindows: true
    appBar: FluAppBar {
            width: windows.width
            height: 30
            showDark: true
            showMaximize: false
            showStayTop: false
            z:7
        }
    function getdecive(pho_set) {
        var available = phoneInfoModel.loadDevices();// function body
        var Index =  pho_set.currentIndex
        if(available)
        {
            pho_set.currentIndex = 0;
            Index =  pho_set.currentIndex;
            pho_set.displayText = phoneInfoModel.get(Index,1);
            CurrentInfo.set(phoneInfoModel.get(Index,2),phoneInfoModel.get(Index,1),phoneInfoModel.get(Index,3),available,phoneInfoModel.get_isSupport(Index))
            currentWorkspace.q_setCurrentWorkspace(phoneInfoModel.get(Index,2))
        }
        else
        {
            pho_set.displayText = qsTr("No device")
            CurrentInfo.set(qsTr("No device"),qsTr("No device"),"",available,false);
        }
    }
    Flipable{
        id:flipable
        anchors.fill: parent
        front: Item{
            id:page_front
            visible: flipable.flipAngle !== 180
            anchors.fill: flipable
            FluNavigationView{
                id:nav_view
                width: parent.width
                height: parent.height
                z:999
                //Stack模式，每次切换都会将页面压入栈中，随着栈的页面增多，消耗的内存也越多，内存消耗多就会卡顿，这时候就需要按返回将页面pop掉，释放内存。该模式可以配合FluPage中的launchMode属性，设置页面的启动模式
                //                pageMode: FluNavigationViewType.Stack
                //NoStack模式，每次切换都会销毁之前的页面然后创建一个新的页面，只需消耗少量内存
                pageMode: FluNavigationViewType.NoStack
                items: ItemsOriginal
                footerItems:ItemsFooter
                topPadding:{
                    return FluTools.isMacos() ? 20 : 0
                }
                displayMode: FluNavigationViewType.Auto
                logo: "qrc:/logo.ico"
                title:"JiTou-Unlock"
                autoSuggestBox:
                    Column{
                        leftPadding: 5
                        spacing: 10
                        Row{
                            spacing: 10
                            FluComboBox{
                                textRole:"Name_"
                                id:pho_set
                                width: 240
                                height:35
                                editable: false
                                disabled: !CurrentInfo.isDeviceAvailable
                                model: phoneInfoModel
                                Component.onCompleted: {
                                    getdecive(pho_set)
                                }
                                onActivated: {
                                    var Index =  pho_set.currentIndex;
                                    CurrentInfo.set(phoneInfoModel.get(Index,2),phoneInfoModel.get(Index,1),phoneInfoModel.get(Index,3),true,phoneInfoModel.get_isSupport(Index))
                                }
                            }
                            FluIconButton{
                                id:fla_btn
                                width: 35
                                height:35
                                iconSource:FluentIcons.Refresh
                                onClicked: {
                                    getdecive(pho_set)
                                }
                            }
                        }
                        Row{
                            spacing: 10
                            leftPadding: 10
                            Image {
                                id:iPh_pic
                                width: 64
                                height:100
                                source: CurrentInfo.isDeviceAvailable ? "qrc:/res/iPhone_Conntect.png" : "qrc:/res/iPhone_NoConntect.png"

                            }
                            Column{
                                topPadding: 5
                                spacing: 10
                                FluText{
                                    font: FluTextStyle.BodyStrong
                                    text: CurrentInfo.name
                                }
                                FluText{
                                    font.pixelSize: 10
                                    font.family: FluTextStyle.family
                                    text: CurrentInfo.uuid
                                }
                                FluText{
                                    id:version_text
                                    font.pixelSize: 10
                                    font.family: FluTextStyle.family
                                    visible: CurrentInfo.isDeviceAvailable
                                    textColor: CurrentInfo.isSupport ? "Green" : "Red"
                                    text: CurrentInfo.isSupport ? CurrentInfo.version : qsTr("No Support iOS Version")
                                }
                            }
                        }
                    }
                Component.onCompleted: {
                    ItemsOriginal.navigationView = nav_view
                    ItemsFooter.navigationView = nav_view
                    windows.setHitTestVisible(nav_view.buttonMenu)
                    windows.setHitTestVisible(nav_view.buttonBack)
                    windows.setHitTestVisible(nav_view.imageLogo)
                    setCurrentIndex(0)
                }
            }
        }
    }
}
