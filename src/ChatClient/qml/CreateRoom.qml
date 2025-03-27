import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: createRoomDialog
    visible: false
    width: 400
    height: 600
    title: "Create Room"

    Column {
        anchors.centerIn: parent

        TextField {
            id: roomNameField
            placeholderText: "Room Name"
        }

        Button {
            text: "Create"
            onClicked: {
                if (roomNameField.text !== "") {
                    chatClient.createRoom(roomNameField.text);
                    createRoomDialog.visible = false;
                } else {
                    console.error("Room name cannot be empty");
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
        function onRoomCreated(success, roomName) {
            if (!success) {
                errorMessage.text = "Failed to create room: " + roomName;
                errorMessage.visible = true;
            }
        }
    }
}



