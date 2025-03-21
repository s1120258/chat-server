import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
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
                // For now, just open the chat room window
                Qt.createComponent("ChatRoom.qml").createObject(parent)
                parent.destroy()
            }
        }
    }
}
