import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// FileItemDelegate.qml — Delegate dùng chung cho Grid và List mode
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root

    // Public properties passed by ContentArea
    property string itemName:     ""
    property string itemSize:     ""
    property string itemModified: ""
    property string itemIcon:     "document"
    property bool   itemIsFolder: false
    property bool   selected:     false
    property string displayMode:  "grid"   // "grid" | "list"

    signal singleClicked()
    signal doubleClicked()
    signal rightClicked(real mouseX, real mouseY)

    // ── Computed ──────────────────────────────────────────────────────
    readonly property color typeColor: AppStyle.fileTypeColor(itemIcon)

    // ── Icon unicode fallback map ─────────────────────────────────────
    readonly property var iconMap: ({
        "folder":      "📁",
        "image":       "🖼",
        "document":    "📝",
        "spreadsheet": "📊",
        "pdf":         "📄",
        "video":       "🎬",
        "archive":     "📦",
        "code":        "💻",
        "audio":       "🎵"
    })
    readonly property string iconChar: iconMap[itemIcon] ?? "📄"

    // ── Mouse area (shared) ───────────────────────────────────────────
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                root.rightClicked(mouse.x + root.x, mouse.y + root.y)
            } else {
                root.singleClicked()
            }
        }
        onDoubleClicked: root.doubleClicked()
    }

    // ── Grid mode ─────────────────────────────────────────────────────
    Rectangle {
        anchors.fill: parent
        visible: root.displayMode === "grid"
        radius: AppStyle.radiusMD
        color: selected
               ? AppStyle.selectedColor
               : (mouseArea.containsMouse ? AppStyle.hoverColor : "transparent")
        border.color: selected ? AppStyle.primaryColor : "transparent"
        border.width: selected ? 1.5 : 0

        Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

        ColumnLayout {
            anchors {
                fill: parent
                topMargin: 10; bottomMargin: 8
                leftMargin: 6; rightMargin: 6
            }
            spacing: 6

            // Icon circle
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 56; height: 56; radius: 14
                color: Qt.rgba(root.typeColor.r, root.typeColor.g, root.typeColor.b, 0.12)

                Text {
                    anchors.centerIn: parent
                    text: root.iconChar
                    font.pixelSize: 28
                }

                // Tooltip on hover
                ToolTip.visible: mouseArea.containsMouse
                ToolTip.text: root.itemName + (root.itemSize !== "—" ? "\n" + root.itemSize : "") + "\n" + root.itemModified
                ToolTip.delay: 600
            }

            // Name
            Text {
                Layout.fillWidth: true
                text: root.itemName
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                font.weight: selected ? Font.Medium : Font.Normal
                color: AppStyle.textPrimary
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                maximumLineCount: 2
                wrapMode: Text.WordWrap
            }

            // Size (only for files)
            Text {
                visible: !root.itemIsFolder
                Layout.alignment: Qt.AlignHCenter
                text: root.itemSize
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                color: AppStyle.textDisabled
            }
        }
    }

    // ── List mode ─────────────────────────────────────────────────────
    Rectangle {
        anchors.fill: parent
        visible: root.displayMode === "list"
        color: selected
               ? AppStyle.selectedColor
               : (mouseArea.containsMouse ? "#FAFAFA" : AppStyle.cardBackground)

        Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

        // Bottom border
        Rectangle {
            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
            height: 1; color: AppStyle.dividerColor
        }

        RowLayout {
            anchors { fill: parent; leftMargin: 16; rightMargin: 16 }
            spacing: 10

            // Icon
            Rectangle {
                width: 32; height: 32; radius: 8
                color: Qt.rgba(root.typeColor.r, root.typeColor.g, root.typeColor.b, 0.12)

                Text {
                    anchors.centerIn: parent
                    text: root.iconChar; font.pixelSize: 16
                }
            }

            // Name
            Text {
                Layout.fillWidth: true
                text: root.itemName
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                font.weight: selected ? Font.Medium : Font.Normal
                color: AppStyle.textPrimary; elide: Text.ElideRight
            }

            // Size
            Text {
                Layout.preferredWidth: 80
                text: root.itemSize
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                color: AppStyle.textSecondary; horizontalAlignment: Text.AlignRight
            }

            // Date
            Text {
                Layout.preferredWidth: 120
                text: root.itemModified
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                color: AppStyle.textSecondary; horizontalAlignment: Text.AlignRight
            }
        }
    }
}
