import QtQuick 2.15
import FluentUI 1.0

FluScrollablePage {
    title: qsTr("Remove the pairing restriction")
    launchMode: FluPageType.SingleTask
    Column{
        topPadding: 5
        spacing: 15
        Row{
            topPadding: 5
            spacing: 10
            FluIcon{
                iconSource: FluentIcons.Settings
                iconSize: 30
            }

            FluText{
                text: qsTr("Remove the Apple Wtach pairing restriction (only support old iOS to pair new watchOS)")
                anchors{
                    verticalCenter: parent.verticalCenter
                }
            }
        }


        FluText{
            text: qsTr("If there is an update to the watchOS version \nthat prevents the default max value from pairing with the latest watchOS")
        }
        FluText{
            text: qsTr("You can change the max value below, the default is 36, Of course, you can also change it to 99 directly")
        }
        Row{
            topPadding: 10
            leftPadding: 5
            spacing: 10
            FluText{
                text: qsTr("max value:")
            }
            FluTextBox{
                id:max_value
                width:80
                height:30
                text:"36"
                maximumLength:2
                validator: RegExpValidator {
                                regExp: /^[0-9]+$/
                            }
                anchors{
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        FluText{
            topPadding: 20
            text: qsTr("Some iPhone were unable to lock IPCC after using the tool (may have been fixed)")
        }
        FluText{
            text: qsTr("If you have a need to lock the IPCC please manually tick on the following options")
        }
        FluText{
            text: qsTr("If the IPCC cannot be locked after removing the option, or if a situation suggests a regulatory model\nplease contact the authors")
        }
        FluCheckBox{
            text:qsTr("I need to lock down the IPCC")
        }
    }
    Component.onCompleted:{
        max_value.text = LinkHelper.getPlistValue_Link(1)
    }
}
