import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FluentUI 1.0

FluScrollablePage {
    title: qsTr("Apply")
    launchMode: FluPageType.SingleTask
    Column{
        topPadding: 10
        spacing: 10
        FluText{
            text:qsTr("If you see the data transferring screen")
        }
        FluText{
            text:qsTr("you MUST select Do Not Transfer Apps and Data")
        }
        FluText{
            topPadding: 15
            text:qsTr("iOS 17.2+ Users: Please be careful after using")
        }
        FluText{
            text:qsTr("If you see a screen showing \"iPhone Partially Set Up\"")
        }
        FluText{
            text:qsTr("click the button that says \"Continue with Partial Setup\", otherwise your phone's data will be erased!")
        }
        FluText{
            topPadding: 10
            text:qsTr("Modified Tweaks:")
        }
        FluText{
            text:qsTr("testtesttest!!")
        }
        FluLoadingButton{
            text:qsTr("Apply Tweaks")
            onClicked: {

            }
            anchors{
                horizontalCenter:parent.horizontalCenter
            }
        }
        FluFrame{
            width: 680
            height: 250
            anchors{
                leftMargin: 30
                rightMargin: 30
            }
            Item{
                anchors.fill: parent
                clip: true
                Flickable{
                    id:scrollview
                    /*
                    width: 670
                    height: 240
                    */
                    contentWidth: width
                    boundsBehavior: Flickable.StopAtBounds
                    contentHeight: text_info.height
                    ScrollBar.vertical: FluScrollBar {}
                    anchors{
                        rightMargin: 10
                    }
                    FluText{
                        id:text_info
                        width: scrollview.width
                        wrapMode: Text.WrapAnywhere
                        padding: 14
                        text:""
                        }
                    }
                }
        }
        Row{
            FluText{
                text:qsTr("If you're having trouble with your device")
                anchors{
                    verticalCenter: parent.verticalCenter
                }
            }
            FluFilledButton{
                text:qsTr("Restore All Change")
                onClicked: {

                }
            }
        }
    }
}
