import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// LogViewer.qml — Dialog xem file log, mở từ menu hoặc user menu
// ─────────────────────────────────────────────────────────────────────────────
Dialog {
    id: root
    title: ""
    modal: true
    width: 760
    height: 520
    anchors.centerIn: Overlay.overlay
    closePolicy: Popup.CloseOnEscape

    header: Item { height: 0 }
    padding: 0

    background: Rectangle {
        radius: AppStyle.radiusXL
        color:  AppStyle.cardBackground
        border.color: AppStyle.borderColor; border.width: 1
    }

    property string logContent: "[2024-08-20 09:15:32] INFO  LicenseService - License validated successfully\n" +
        "[2024-08-20 09:15:33] INFO  FileIndexer - Starting index scan on /MyDrive (12 items)\n" +
        "[2024-08-20 09:15:34] INFO  SyncManager - Connected to sync.filemanager.io:443\n" +
        "[2024-08-20 09:16:01] INFO  FileIndexer - Index complete: 12 files, 3 folders\n" +
        "[2024-08-20 09:18:45] INFO  SearchWorker - Search query: \"báo cáo\" — 2 results found\n" +
        "[2024-08-20 09:22:10] INFO  FileIndexer - File uploaded: Thiết kế UI v3.2.png (5.1 MB)\n" +
        "[2024-08-20 09:25:00] WARN  SyncManager - Sync delay detected: 250ms latency\n" +
        "[2024-08-20 09:30:15] INFO  FileIndexer - File deleted: old_backup.zip\n" +
        "[2024-08-20 09:35:42] INFO  SearchWorker - Search query: \"hợp đồng\" — 1 result found\n" +
        "[2024-08-20 09:40:00] INFO  SyncManager - All changes synced successfully\n"

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 260; easing.type: Easing.OutCubic }
        NumberAnimation { property: "scale";   from: 0.94; to: 1; duration: 260; easing.type: Easing.OutCubic }
    }
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 160 }
    }

    contentItem: ColumnLayout {
        spacing: 0

        // ── Header ────────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true; height: 56
            radius: AppStyle.radiusXL
            color: "#1E1E2E"

            // Flatten bottom corners
            Rectangle {
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                height: parent.radius; color: parent.color
            }

            RowLayout {
                anchors { fill: parent; leftMargin: 18; rightMargin: 12 }
                spacing: 12

                // Icon
                Rectangle {
                    width: 32; height: 32; radius: 8
                    color: Qt.rgba(1,1,1,0.1)
                    Text {
                        anchors.centerIn: parent; text: "📄"
                        font.pixelSize: 16
                    }
                }

                ColumnLayout {
                    spacing: 1
                    Text {
                        text: "Log Viewer"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeMD
                        font.weight: Font.Medium; color: "#CDD6F4"
                    }
                    Text {
                        text: "filemanager.log"
                        font.family: AppStyle.fontFamilyMono; font.pixelSize: AppStyle.fontSizeXS
                        color: "#6C7086"
                    }
                }

                Item { Layout.fillWidth: true }

                // Refresh button
                Rectangle {
                    height: 28; radius: AppStyle.radiusFull
                    width: refreshLabel.implicitWidth + 20
                    color: refreshMa.containsMouse ? Qt.rgba(1,1,1,0.15) : Qt.rgba(1,1,1,0.08)
                    Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                    RowLayout {
                        anchors.centerIn: parent; spacing: 6
                        Text { text: "⟳"; font.pixelSize: 13; color: "#CDD6F4" }
                        Text { id: refreshLabel; text: "Làm mới"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; color: "#CDD6F4" }
                    }
                    MouseArea {
                        id: refreshMa; anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            const now = new Date()
                            const ts  = "[" + now.toLocaleString(Qt.locale("vi_VN"), "yyyy-MM-dd hh:mm:ss") + "]"
                            root.logContent += ts + " INFO  System - Log refreshed by user\n"
                            logScrollView.ScrollBar.vertical.position = 1.0
                        }
                    }
                }

                // Close
                RoundButton {
                    flat: true; width: 32; height: 32; padding: 0
                    contentItem: Text {
                        text: "✕"; font.pixelSize: 13; color: "#6C7086"
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 16; color: parent.hovered ? Qt.rgba(1,1,1,0.12) : "transparent"
                    }
                    onClicked: root.close()
                }
            }
        }

        // ── Log content (dark terminal) ───────────────────────────────
        ScrollView {
            id: logScrollView
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true

            Rectangle {
                width: logScrollView.width
                implicitHeight: logText.implicitHeight + 24
                color: "#1E1E2E"

                Text {
                    id: logText
                    anchors { left: parent.left; right: parent.right; top: parent.top; margins: 16 }
                    text: formatLog(root.logContent)
                    font.family: AppStyle.fontFamilyMono; font.pixelSize: AppStyle.fontSizeSM
                    color: "#CDD6F4"
                    lineHeight: 1.6
                    wrapMode: Text.WrapAnywhere
                    textFormat: Text.RichText

                    function formatLog(raw) {
                        return raw.split("\n").filter(l => l.trim() !== "").map(line => {
                            if (line.includes(" ERROR ")) return "<span style='color:#F38BA8'>" + line + "</span>"
                            if (line.includes(" WARN  ")) return "<span style='color:#F9E2AF'>" + line + "</span>"
                            if (line.includes(" INFO  ")) {
                                // Color timestamp differently
                                return line.replace(
                                    /^(\[.*?\])/,
                                    "<span style='color:#6C7086'>$1</span>"
                                ).replace(
                                    / INFO  /,
                                    " <span style='color:#A6E3A1'>INFO </span> "
                                )
                            }
                            return "<span style='color:#CDD6F4'>" + line + "</span>"
                        }).join("<br/>")
                    }
                }
            }
        }

        // ── Footer ────────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true; height: 52
            radius: AppStyle.radiusXL
            color: AppStyle.backgroundColor
            border.color: AppStyle.borderColor; border.width: 1

            // Flatten top corners
            Rectangle {
                anchors { left: parent.left; right: parent.right; top: parent.top }
                height: parent.radius; color: parent.color
            }

            RowLayout {
                anchors { fill: parent; leftMargin: 20; rightMargin: 16 }

                Text {
                    text: root.logContent.split("\n").filter(l => l.trim() !== "").length + " dòng log"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    color: AppStyle.textDisabled
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    height: 34; radius: AppStyle.radiusMD
                    width: closeBtnLabel.implicitWidth + 24
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: AppStyle.primaryColor }
                        GradientStop { position: 1; color: AppStyle.primaryDark }
                    }

                    Text {
                        id: closeBtnLabel; anchors.centerIn: parent
                        text: "Đóng"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                        font.weight: Font.Medium; color: "#FFFFFF"
                    }
                    MouseArea {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: root.close()
                    }
                }
            }
        }
    }
}
