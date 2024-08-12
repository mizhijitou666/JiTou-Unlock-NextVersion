import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import FluentUI 1.0
import "../global"


FluScrollablePage{

    launchMode: FluPageType.SingleTask
    header: Item{}

    ListModel{
        id: model_header
        ListElement{
            icon: "qrc:/res/mizhijitou.jpg"
            title: qsTr("mizhijitou")
            desc: qsTr("The author of the applications.\nAdd \"Remove the pairing restriction\" Option")
            url: "https://space.bilibili.com/21520149"
            clicked: function(model){
                Qt.openUrlExternally(model.url)
            }
        }
    }
    Item{
        Layout.fillWidth: true
        Layout.preferredHeight: 380
        Image {
            id: bg
            fillMode:Image.PreserveAspectCrop
            anchors.fill: parent
            verticalAlignment: Qt.AlignTop
            sourceSize: Qt.size(960,600)
            source: "qrc:/res/bg.png"
        }
        Rectangle{
            anchors.fill: parent
            gradient: Gradient{
                GradientStop { position: 0.8; color: FluTheme.dark ? Qt.rgba(0,0,0,0) : Qt.rgba(1,1,1,0) }
                GradientStop { position: 1.0; color: FluTheme.dark ? Qt.rgba(0,0,0,1) : Qt.rgba(1,1,1,1) }
            }
        }
        Component{
            id:com_grallery
            Item{
                id: control
                width: 220
                height: 240
                FluShadow{
                    radius:5
                    anchors.fill: item_content
                }
                FluClip{
                    id:item_content
                    radius: [5,5,5,5]
                    width: 200
                    height: 220
                    anchors.centerIn: parent
                    FluAcrylic{
                        anchors.fill: parent
                        tintColor: FluTheme.dark ? Qt.rgba(0,0,0,1) : Qt.rgba(1,1,1,1)
                        target: bg
                        tintOpacity: FluTheme.dark ? 0.8 : 0.9
                        blurRadius : 80
                        targetRect: Qt.rect(list.x-list.contentX+10+(control.width)*index,list.y+10,width,height)
                    }
                    Rectangle{
                        anchors.fill: parent
                        radius: 5
                        color:FluTheme.itemHoverColor
                        visible: item_mouse.containsMouse
                    }
                    Rectangle{
                        anchors.fill: parent
                        radius: 5
                        color:Qt.rgba(0,0,0,0.0)
                        visible: !item_mouse.containsMouse
                    }
                    ColumnLayout{
                        Image {
                            Layout.topMargin: 20
                            Layout.leftMargin: 20
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 50
                            source: model.icon
                        }
                        FluText{
                            text: model.title
                            font: FluTextStyle.Body
                            Layout.topMargin: 20
                            Layout.leftMargin: 20
                        }
                        FluText{
                            text: model.desc
                            Layout.topMargin: 5
                            Layout.preferredWidth: 160
                            Layout.leftMargin: 20
                            color: FluColors.Grey120
                            font.pixelSize: 12
                            font.family: FluTextStyle.family
                            wrapMode: Text.WrapAnywhere
                        }
                    }
                    FluIcon{
                        iconSource: FluentIcons.OpenInNewWindow
                        iconSize: 15
                        anchors{
                            bottom: parent.bottom
                            right: parent.right
                            rightMargin: 10
                            bottomMargin: 10
                        }
                    }
                    MouseArea{
                        id:item_mouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onWheel:
                            (wheel)=>{
                                if (wheel.angleDelta.y > 0) scrollbar_header.decrease()
                                else scrollbar_header.increase()
                            }
                        onClicked: {
                            model.clicked(model)
                        }
                    }
                }
            }
        }

        ListView{
            id: list
            anchors{
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            orientation: ListView.Horizontal
            height: 240
            model: model_header
            header: Item{height: 10;width: 10}
            footer: Item{height: 10;width: 10}
            ScrollBar.horizontal: FluScrollBar{
                id: scrollbar_header
            }
            clip: false
            delegate: com_grallery
        }
    }

    Component{
        id:com_item
        Item{
            width: 230
            height: 100
            FluFrame{
                radius: 8
                width: 210
                height: 80
                anchors.centerIn: parent
                Rectangle{
                    anchors.fill: parent
                    radius: 8
                    color:{
                        if(item_mouse.containsMouse){
                            return FluTheme.itemHoverColor
                        }
                        return FluTheme.itemNormalColor
                    }
                }
                Image{
                    id:item_icon
                    height: 45
                    width: 45
                    source: image
                    anchors{
                        left: parent.left
                        leftMargin: 10
                        verticalCenter: parent.verticalCenter
                    }
                }
                FluText{
                    id:item_title
                    text:title
                    font: FluTextStyle.BodyStrong
                    anchors{
                        left: item_icon.right
                        leftMargin: 10
                        top: parent.top
                        topMargin: 15
                    }
                }
                FluText{
                    id:item_desc
                    text:desc
                    color:FluColors.Grey120
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    font: FluTextStyle.Caption
                    maximumLineCount: 2
                    anchors{
                        left: item_title.left
                        right: parent.right
                        rightMargin: 10
                        top: item_title.bottom
                        topMargin: 2
                    }
                }
                FluIcon{
                    iconSource: FluentIcons.OpenInNewWindow
                    iconSize: 15
                    anchors{
                        right: parent.right
                        top: parent.top
                        rightMargin: 10
                        topMargin: 10
                    }
                }

                MouseArea{
                    id:item_mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:
                        Qt.openUrlExternally(url)
                }
            }
        }
    }
    FluText{
        text: qsTr("Acknowledgement")
        font: FluTextStyle.Title
        Layout.topMargin: 20
        Layout.leftMargin: 20
    }

    GridView{
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
        cellHeight: 100
        cellWidth: 230
        model:ListModel{
            ListElement{image:"qrc:/res/Avangelista.jpg";title:qsTr("Avangelista");desc:qsTr("Cowabunga Developer\nBackup Generator");url: "https://github.com/Avangelista"}
            ListElement{image:"qrc:/res/LeminLimez.png";title:qsTr("LeminLimez");desc:qsTr("Cowabunga MDC and Mac Version Developer");url: "https://github.com/leminlimez"}
            ListElement{image:"qrc:/res/zhuzichu.png";title:qsTr("zhuzichu520");desc:qsTr("FluentUI Designer");url: "https://github.com/zhuzichu520"}
        }
        interactive: false
        delegate: com_item
    }

    FluText{
        text: qsTr("Help and Support")
        font: FluTextStyle.Title
        Layout.topMargin: 5
        Layout.leftMargin: 20
    }

    GridView{
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
        cellHeight: 100
        cellWidth: 230
        model:ListModel{
            ListElement{image:"qrc:/res/i4_logo.png";title:qsTr("i4 Tools");desc:qsTr("A tool for connecting \niPhone to Windows");url: "https://www.i4.cn/"}
            ListElement{image:"qrc:/res/bilibili_logo.png";title:qsTr("Bilibili video");desc:qsTr("A video to help use of \nthe software");url: "https://www.bilibili.com/video/BV1GFeAegEEj/"}
            ListElement{image:"qrc:/res/coolapk_logo.png";title:qsTr("Coolapk course");desc:qsTr("A Graphics and texts to help use the software");url: "https://www.coolapk.com/feed/58160978"}
            ListElement{image:"qrc:/res/QQ_logo.png";title:qsTr("QQ Group Chats");desc:qsTr("QQ Group Chats \nnumber: 665784074");url: "https://qm.qq.com/q/jck4QLs5sk"}
            ListElement{image:"qrc:/res/mizhijitou_qq.jpg";title:qsTr("QQ");desc:qsTr("QQ number : \n1249863413");url: "https://qm.qq.com/q/9LXTVULnuU"}
            ListElement{image:"qrc:/res/mizhijitou_wechat.jpg";title:qsTr("Wechat");desc:qsTr("Wechat ID : mizhijitou");url:""}
        }
        interactive: false
        delegate: com_item
    }


}

