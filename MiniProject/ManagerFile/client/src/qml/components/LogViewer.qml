import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Dialog {
    id: logViewerRoot
    anchors.centerIn: parent
    modal: true
    width: 680
    height: 480
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    header: Rectangle {
        height: 48
        color: "#f8f9fa"
        radius: 16
        border.color: "#dadce0"

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 16
            color: "#f8f9fa"
        }

        Text {
            anchors.centerIn: parent
            text: "📜 Nhật ký thao tác ứng dụng (filemanager.log)"
            color: window.textLight
            font.pixelSize: 14
            font.bold: true
        }
    }

    background: Rectangle {
        color: "#ffffff"
        radius: 16
        border.color: "#dadce0"
        border.width: 1
    }

    function openLogViewer(logContent) {
        logTextArea.text = logContent
        logViewerRoot.open()
    }

    contentItem: ColumnLayout {
        spacing: 12

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Chi tiết nhật ký hoạt động hệ thống:"
                color: window.textMuted
                font.pixelSize: 12
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "⟳ Tải lại"
                implicitHeight: 30
                implicitWidth: 80
                onClicked: {
                    logTextArea.text = appCore.readLogFile()
                }
                contentItem: Text {
                    text: "⟳ Tải lại"
                    color: window.primaryColor
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.hovered ? "#e8f0fe" : "#ffffff"
                    radius: 6
                    border.color: window.primaryColor
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: logTextArea
                readOnly: true
                color: "#202124"
                font.family: "Consolas"
                font.pixelSize: 11
                wrapMode: Text.WrapAnywhere
                selectByMouse: true
                leftPadding: 12
                topPadding: 12

                background: Rectangle {
                    color: "#f8f9fa"
                    radius: 8
                    border.color: "#dadce0"
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Đóng"
                implicitWidth: 90
                implicitHeight: 34
                onClicked: logViewerRoot.close()
                contentItem: Text {
                    text: "Đóng"
                    color: "#ffffff"
                    font.pixelSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.hovered ? "#1557b0" : window.primaryColor
                    radius: 8
                }
            }
        }
    }
}
