import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: inviteUserDialog
    visible: false
    title: "Invite User"

    property string roomName: ""

    Column {
        anchors.centerIn: parent

        TextField {
            id: usernameField
            placeholderText: "Username"
        }

        Button {
            text: "Invite"
            onClicked: {
                if (usernameField.text !== "") {
                    chatClient.inviteUserToRoom(usernameField.text, roomName);
                    inviteUserDialog.visible = false;
                } else {
                    console.error("Username cannot be empty");
                }
            }
        }

        Text {
            id: errorMessage
            color: "red"
            visible: false
        }
    }

    Connections {
        target: chatClient
        function onRoomJoined(success, roomName) {
            if (!success) {
                errorMessage.text = "Failed to invite user to room: " + roomName;
                errorMessage.visible = true;
            }
        }
    }
}



