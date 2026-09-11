import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: overlayRoot
    color: "#b0000000" // Alpha 0.3 black overlay
    z: 900
    visible: opacity > 0

    property alias message: messageLabel.text
    property alias progress: progressBar.value

    signal cancelRequested()

    // Glassmorphic Dialog Box
    Rectangle {
        anchors.centerIn: parent
        width: 380
        height: 180
        radius: 12
        color: window.cardBg
        border.color: "#3b3b54"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 14

            Text {
                id: messageLabel
                text: "Đang xử lý tác vụ..."
                color: window.textLight
                font.pixelSize: 14
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }

            ProgressBar {
                id: progressBar
                Layout.fillWidth: true
                Layout.preferredHeight: 10
                from: 0
                to: 100
                value: 50

                background: Rectangle {
                    color: "#181824"
                    radius: 5
                }
                contentItem: Item {
                    Rectangle {
                        width: parent.width * (progressBar.value / 100)
                        height: parent.height
                        radius: 5
                        color: window.primaryColor
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12

                Button {
                    text: "Hủy bỏ"
                    implicitWidth: 100
                    implicitHeight: 34
                    onClicked: overlayRoot.cancelRequested()
                    background: Rectangle {
                        color: parent.pressed ? "#c62828" : "#d32f2f"
                        radius: 6
                    }
                    contentItem: Text {
                        text: "Hủy bỏ"
                        color: "#ffffff"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
