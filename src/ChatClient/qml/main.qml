import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Home {
        id: homeWindow
    }

    InvitationDialog {
        id: invitationDialog
    }

    Connections {
        target: chatClient
        function onInvitationReceived(roomName, invitedBy) {
            invitationDialog.roomName = roomName;
            invitationDialog.invitedBy = invitedBy;
            invitationDialog.visible = true;
        }
    }
}



