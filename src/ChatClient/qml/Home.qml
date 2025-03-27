import QtQuick 2.15
import QtQuick.Controls 2.15
import "qml_utils.js" as Utils

ApplicationWindow {
    id: homeWindow
    visible: true
    width: 600
    height: 400
    title: "Home"

    Column {
        anchors.fill: parent

        Button {
            text: "Create Room"
            onClicked: {
                console.log("Create Room button clicked");
                // Open create room dialog
                Utils.loadDialog(homeWindow, "CreateRoom");
            }
        }

        ListView {
            id: roomList
            anchors.fill: parent
            model: ListModel {}

            delegate: Item {
                width: roomList.width
                height: 50

                Text {
                    text: model.name
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Chat Room clicked");
                        // Open chat room window
                        Utils.loadDialog(homeWindow, "ChatRoom");
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        // Fetch joined rooms from the server
        chatClient.fetchJoinedRooms();
    }

    Connections {
        target: chatClient
        function onJoinedRoomsReceived(rooms) {
            roomList.model.clear();
            for (var i = 0; i < rooms.length; i++) {
                roomList.model.append({ "name": rooms[i] });
            }
        }
    }
}
