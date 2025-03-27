import QtQuick 2.15
import QtQuick.Controls 2.15
import "qml_utils.js" as Utils

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
                console.error("Logged in successfully");
                // Open the chat room window
                Utils.loadWindow(loginWindow, "Home");
            } else {
                console.error("Login failed");
                errorMessage.text = errorMessage;
                errorMessage.visible = true;
            }
        }
    }
}

