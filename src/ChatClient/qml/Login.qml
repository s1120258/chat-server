import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: loginWindow
    visible: true
    width: 400
    height: 300
    title: "Login"

    Column {
        anchors.centerIn: parent

        TextField {
            id: usernameField
            placeholderText: "Username"
        }

        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        Button {
            text: "Login"
            onClicked: {
                // Handle login
                if (usernameField.text !== "" && passwordField.text !== "") {
                    chatClient.login(usernameField.text, passwordField.text);
                } else {
                    console.error("Username and password cannot be empty");
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
        onLoginResult: {
            if (success) {
                // Open the chat room window
                var chatRoomComponent = Qt.createComponent("ChatRoom.qml");
                if (chatRoomComponent.status === Component.Ready) {
                    var chatRoom = chatRoomComponent.createObject(null);
                    if (chatRoom === null) {
                        console.error("Error creating ChatRoom object");
                    } else {
                        loginWindow.visible = false;
                        chatRoom.visible = true;
                    }
                } else {
                    console.error("Error loading ChatRoom component");
                }
            } else {
                errorMessage.text = errorMessage;
                errorMessage.visible = true;
            }
        }
    }
}

