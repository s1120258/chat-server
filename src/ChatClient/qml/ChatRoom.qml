import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: chatRoomWindow
    visible: true
    width: 800
    height: 600
    title: "Chat Room"

    property string roomName: ""

    Column {
        anchors.fill: parent

        Row {
            width: parent.width
            height: 50
            spacing: 10

            Button {
                text: "Back"
                onClicked: {
                    chatRoomWindow.visible = false;
                    homeWindow.visible = true;
                }
            }

            Text {
                text: "Room: " + roomName
                font.pixelSize: 20
                anchors.verticalCenter: parent.verticalCenter
            }
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
                    chatClient.sendMessage(messageField.text);
                    messageField.text = "";
                }
            }
        }

        Button {
            text: "Invite User"
            onClicked: {
                // Open invite user dialog
                var inviteUserComponent = Qt.createComponent("InviteUser.qml");
                if (inviteUserComponent.status === Component.Ready) {
                    var inviteUserDialog = inviteUserComponent.createObject(null, { "roomName": roomName });
                    if (inviteUserDialog === null) {
                        console.error("Error creating InviteUser dialog");
                    } else {
                        inviteUserDialog.visible = true;
                    }
                } else {
                    console.error("Error loading InviteUser component");
                }
            }
        }
    }

    Connections {
        target: chatClient
        function onMessageReceived(message) {
            messageList.model.append({ "message": message });
        }
    }
}



