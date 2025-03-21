import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Chat"

    property string roomName: ""

    Column {
        anchors.fill: parent

        Text {
            text: "Room: " + roomName
            font.pixelSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ListView {
            id: messageList
            anchors.fill: parent
            model: ListModel {}

            delegate: Item {
                width: parent.width
                height: 50

                Text {
                    text: model.message
                    anchors.centerIn: parent
                }
            }
        }

        Row {
            TextField {
                id: messageField
                width: parent.width - 100
                placeholderText: "Type a message"
            }

            Button {
                text: "Send"
                onClicked: {
                    // Send message
                    chatClient.sendMessage(messageField.text)
                    messageField.text = ""
                }
            }
        }
    }

    Connections {
        target: chatClient
        onMessageReceived: {
            messageList.model.append({ "message": message })
        }
    }
}
