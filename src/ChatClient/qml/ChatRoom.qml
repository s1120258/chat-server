import QtQuick 2.15
import QtQuick.Controls 2.15
import "qml_utils.js" as Utils

ApplicationWindow {
    id: chatRoomWindow
    visible: true
    width: 800
    height: 600
    title: "Chat Room"

    property string roomName: ""
    property int roomId: -1
    property int userId: -1
    property string userNamesString: ""
    property var userIds: []

    Column {
        anchors.fill: parent

        Row {
            width: parent.width
            height: 20
            spacing: 10

            Text {
                text: "Room: " + roomName
                font.pixelSize: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        /* Row {
            width: parent.width
            height: 80
            spacing: 10

            Text {
                text: "Users: " + userNamesString
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        } */

        ListView {
            id: messageList
            anchors.fill: parent
            model: ListModel {
                ListElement { username: "User 1"; message: "Massage 1" }
                ListElement { username: "User 2"; message: "Massage 2" }
                ListElement { username: "User 3"; message: "Massage 3" }
            }

            delegate: Item {
                width: parent.width
                height: 50

                Text {
                    text: model.username + ": " + model.message
                    anchors.centerIn: parent
                }
            }
        }

        Row {
            TextField {
                id: messageField
                // width: parent.width - sendButton.width - 20
                width: 200
                placeholderText: "Type a message"
            }

            Button {
                text: "Send"
                onClicked: {
                    console.log("Send button clicked");
                    chatClient.sendMessage(userId, roomId, messageField.text);
                    messageField.text = "";
                }
            }
        }

        Button {
            text: "Invite User"
            onClicked: {
                console.log("Invite User button clicked");
                // Open invite user dialog
                Utils.loadDialog(chatRoomWindow, "InviteUser", {
                    roomName: roomName,
                    roomId: roomId
                });
            }
        }

        Button {
            text: "Home"
            onClicked: {
                console.log("Home button clicked");
                Utils.loadWindow(chatRoomWindow, "Home");
            }
        }
    }

    Component.onCompleted: {
        // chatClient.fetchUsersInRoom(roomId);
        chatClient.fetchMessages(roomId);
    }

    Connections {
        target: chatClient

        function onUsersInRoomReceived(users) {
            userIds = [];
            var userNames = [];
            for (var i = 0; i < users.length; ++i) {
                userNames.push(Utils.getJsonValue(users[i], "username"));
                userIds.push(parseInt(Utils.getJsonValue(users[i], "user_id"), 10));
            }
            userNamesString = userNames.join(", ");
        }

        function onMessagesReceived(messages) {
            messageList.model.clear();
            for (var i = 0; i < messages.length; ++i) {
                messageList.model.append({
                    "username": Utils.getJsonValue(messages[i], "username"),
                    "message": Utils.getJsonValue(messages[i], "message")
                });
            }
        }

       function onMessageReceived(success, username, message) {
            if (success) {
                messageList.model.append({ "username": username, "message": message });
            } else {
                console.error("Failed to send message: " + messageField.text);
                // errorMessage.text = "Failed to send message: " + messageField.text;
                // errorMessage.visible = true;
            }
       }
    }
}



