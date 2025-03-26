import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: chatRoomWindow
    visible: true
    width: 600
    height: 400
    title: "Chat Rooms"

    ListView {
        id: chatRoomList
        anchors.fill: parent
        model: ListModel {
            ListElement { name: "Room 1" }
            ListElement { name: "Room 2" }
        }

        delegate: Item {
            width: chatRoomList.width
            height: 50

            Text {
                text: model.name
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // Open chat window
                    var chatWindowComponent = Qt.createComponent("ChatWindow.qml");
                    if (chatWindowComponent.status === Component.Ready) {
                        var chatWindow = chatWindowComponent.createObject(chatRoomWindow, { "roomName": model.name });
                        if (chatWindow === null) {
                            console.error("Error creating ChatWindow object");
                        } else {
                            chatRoomWindow.visible = false;
                            chatWindow.visible = true;
                        }
                    } else {
                        console.error("Error loading ChatWindow component");
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        // Fetch rooms from the server
        chatClient.fetchRooms();
    }

    Connections {
        target: chatClient
        onRoomsReceived: {
            chatRoomList.model.clear();
            for (var i = 0; i < rooms.length; i++) {
                chatRoomList.model.append({ "name": rooms[i] });
            }
        }
    }
}
