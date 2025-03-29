import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: inviteUserDialog
    visible: false
    title: "Invite User"

    property string roomName: ""
    property int roomId: -1

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
                    chatClient.inviteUserToRoom(usernameField.text, roomId);
                    inviteUserDialog.visible = false;
                } else {
                    console.error("Username cannot be empty");
                }
            }
        }

        Button {
            text: "Back"
            onClicked: {
                inviteUserDialog.visible = false;
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
        function onUserInvited(success, username) {
            if (success) {
                chatClient.fetchUsersInRoom(roomId);
            } else {
                console.error("Failed to invite user: " + username);
                // errorMessage.text = "Failed to invite user: " + username;
                // errorMessage.visible = true;
            }
        }
    }
}



