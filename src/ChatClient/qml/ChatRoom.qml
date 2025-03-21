import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "Chat Rooms"

    ListView {
        id: chatRoomList
        anchors.fill: parent
        model: ListModel {
            ListElement { name: "Room 1" }
            ListElement { name: "Room 2" }
        }

        delegate: Item {
            width: parent.width
            height: 50

            Text {
                text: model.name
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // Open chat window
                    var chatWindow = Qt.createComponent("ChatWindow.qml").createObject(parent, { "roomName": model.name })
                    parent.destroy()
                }
            }
        }
    }
}
