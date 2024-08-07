import QtQuick 2.15
import FluentUI 1.0

FluScrollablePage {
    title: qsTr("Q&A")
    launchMode: FluPageType.SingleTask
    Column{
        topPadding: 10
        spacing: 10
        FluText{
            font: FluTextStyle.Subtitle
            text:qsTr("Q:It is any bugs ?")
        }
        FluText{
            text:qsTr("Sure Bugs include:\n1、Albums can't be synced(iOS15)\n2、Sleep data is not visible(iOS15)\n3、The new watch face is not visible on the iPhone APP\n4、Motion tracking and map navigation synchronization are not available\n5、Watch screenshots are not available(only iOSX no match Watch OSX)")
        }
        FluText{
            font: FluTextStyle.Subtitle
            text:qsTr("Q:if I still can't pair Watch after using the tool , What can I do ?")
        }
        FluText{
            text:qsTr("1、You can try Higher max value\n2、You can try Del the watch backup\nOpen Settings App on your iPhone\nFind General -> Storage -> Watch App -> delete All watch backup then repair again")
        }
        FluText{
            font: FluTextStyle.Subtitle
            text:qsTr("Q:How can I reset my device ?")
        }
        FluText{
            text:qsTr("You can go to Apply page and Click the reset button")
        }
    }
}
