import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// NavigationPane.qml — Cây thư mục bên trái
// ─────────────────────────────────────────────────────────────────────────────
Rectangle {
    id: root
    color: AppStyle.sidebarBg

    property string currentPath: "/MyDrive"
    signal folderSelected(string path)

    // Border right
    Rectangle {
        anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
        width: 1; color: AppStyle.borderColor
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ── Section label ─────────────────────────────────────────────
        Text {
            Layout.fillWidth: true
            Layout.topMargin: 12; Layout.leftMargin: 16; Layout.rightMargin: 16; Layout.bottomMargin: 4
            text: "BỘ NHỚ CỦA TÔI"
            font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
            font.weight: Font.Medium; color: AppStyle.textDisabled
            font.letterSpacing: 0.5
        }

        // ── Tree view (TreeView from folderModel) ─────────────────────
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TreeView {
                id: treeView
                model: folderModel
                clip: true

                delegate: TreeViewDelegate {
                    id: treeDelegate
                    implicitWidth: treeView.width
                    implicitHeight: 34

                    // Highlight
                    background: Rectangle {
                        color: model.folderPath === root.currentPath
                               ? AppStyle.selectedColor
                               : (treeDelegate.hovered ? AppStyle.hoverColor : "transparent")
                        radius: AppStyle.radiusMD
                        anchors {
                            left: parent.left; right: parent.right
                            top: parent.top; bottom: parent.bottom
                            leftMargin: 6; rightMargin: 6
                        }
                        Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }
                    }

                    contentItem: RowLayout {
                        spacing: 4
                        anchors {
                            left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter
                            leftMargin: 8 + (treeDelegate.depth * 16)
                        }

                        // Expand arrow
                        Text {
                            text: "▶"
                            font.pixelSize: 9
                            color: model.folderPath === root.currentPath
                                   ? AppStyle.primaryColor : AppStyle.textDisabled
                            rotation: treeDelegate.expanded ? 90 : 0
                            visible: model.hasChildren
                            Behavior on rotation { NumberAnimation { duration: AppStyle.durationFast } }
                        }

                        // Placeholder if no children
                        Item { width: 12; visible: !model.hasChildren }

                        // Folder icon
                        Text {
                            text: folderIcon(model.folderName)
                            font.pixelSize: 14
                            color: model.folderPath === root.currentPath
                                   ? AppStyle.primaryColor : AppStyle.colorFolder
                        }

                        // Label
                        Text {
                            Layout.fillWidth: true
                            text: model.folderName
                            font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                            font.weight: model.folderPath === root.currentPath ? Font.Medium : Font.Normal
                            color: model.folderPath === root.currentPath
                                   ? AppStyle.primaryDark : AppStyle.textPrimary
                            elide: Text.ElideRight
                        }
                    }

                    onClicked: {
                        root.currentPath = model.folderPath
                        root.folderSelected(model.folderPath)
                    }
                }
            }
        }

        // ── Storage usage bar ─────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: storageCol.implicitHeight + 24
            color: AppStyle.cardBackground

            Rectangle {
                anchors { left: parent.left; right: parent.right; top: parent.top }
                height: 1; color: AppStyle.borderColor
            }

            ColumnLayout {
                id: storageCol
                anchors { fill: parent; margins: 16 }
                spacing: 6

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: "Dung lượng"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                        color: AppStyle.textSecondary
                    }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: "6.2 / 15 GB"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                        color: AppStyle.textSecondary
                    }
                }

                // Progress bar
                Rectangle {
                    Layout.fillWidth: true; height: 6; radius: 3
                    color: AppStyle.borderColor

                    Rectangle {
                        width: parent.width * 0.41; height: parent.height; radius: parent.radius
                        color: AppStyle.primaryColor
                        Component.onCompleted: {
                            widthAnim.start()
                        }
                        NumberAnimation {
                            id: widthAnim
                            target: parent
                            property: "width"
                            from: 0; to: parent.parent.width * 0.41
                            duration: 800; easing.type: Easing.OutCubic
                        }
                    }
                }

                Text {
                    text: "8.8 GB còn trống"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    color: AppStyle.textDisabled
                }
            }
        }
    }

    function folderIcon(name) {
        if (name === "Shared with me") return "👥"
        if (name === "Thùng rác")      return "🗑"
        return "📁"
    }
}
