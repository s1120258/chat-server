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
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ListView {
            id: roomList
            anchors.fill: parent
            model: ListModel {
                ListElement { name: "Room 0"; roomId: 0 }
            }

            delegate: Item {
                width: roomList.width
                height: 40

                Text {
                    text: model.name
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Chat Room clicked, Name: " + model.name);
                        // Open chat room window
                        Utils.loadWindow(homeWindow, "ChatRoom", {
                            roomName: model.name,
                            roomId: model.roomId
                        });
                    }
                }
            }
        }

        Button {
            text: "Create Room"
            onClicked: {
                console.log("Create Room button clicked");
                // Open create room dialog
                Utils.loadDialog(homeWindow, "CreateRoom");
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
            for (var i = 0; i < rooms.length; ++i) {
                roomList.model.append({
                    "name": Utils.getJsonValue(rooms[i], "room_name"),
                    "roomId": parseInt(Utils.getJsonValue(rooms[i], "room_id"), 10)
                });
            }
        }
    }
}
