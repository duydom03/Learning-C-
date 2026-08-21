import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// Toolbar.qml — Thanh công cụ chính bên trên ContentArea
// ─────────────────────────────────────────────────────────────────────────────
Rectangle {
    id: root
    height: implicitHeight
    implicitHeight: toolbarRow.height + breadcrumbRow.height
    color: AppStyle.cardBackground

    property string viewMode: "grid"
    property string currentPath: "/MyDrive"

    signal createFolderTriggered()
    signal uploadTriggered()
    signal refreshTriggered()
    signal searchTriggered(string keyword)
    signal toggleViewTriggered()
    signal menuActionTriggered(string action)

    function createFolderRequested() { createFolderTriggered() }
    function focusSearch() { searchField.forceActiveFocus() }

    // Bottom border
    Rectangle {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 1; color: AppStyle.borderColor
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ── Main toolbar row ──────────────────────────────────────────
        RowLayout {
            id: toolbarRow
            Layout.fillWidth: true
            height: AppStyle.toolbarHeight
            spacing: 4

            Item { width: 8 }

            // Tạo thư mục mới
            Rectangle {
                height: 32; radius: AppStyle.radiusFull
                width: newFolderRow.implicitWidth + 20
                color: newFolderMa.containsMouse ? AppStyle.primaryLight : AppStyle.primaryFaint
                border.color: AppStyle.primaryLight; border.width: 1

                Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                RowLayout {
                    id: newFolderRow
                    anchors.centerIn: parent; spacing: 6

                    Text {
                        text: "＋"; font.pixelSize: 14; color: AppStyle.primaryDark
                        font.weight: Font.Medium
                    }
                    Text {
                        text: "Thư mục mới"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                        font.weight: Font.Medium; color: AppStyle.primaryDark
                    }
                }
                MouseArea {
                    id: newFolderMa; anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: root.createFolderTriggered()
                }

                ToolTip.visible: newFolderMa.containsMouse
                ToolTip.text: "Tạo thư mục mới (Ctrl+N)"
                ToolTip.delay: 800
            }

            // Tải lên
            Rectangle {
                height: 32; radius: AppStyle.radiusFull
                width: uploadRow.implicitWidth + 16
                color: uploadMa.containsMouse ? AppStyle.hoverColor : "transparent"
                Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                RowLayout {
                    id: uploadRow; anchors.centerIn: parent; spacing: 6
                    Text { text: "⬆"; font.pixelSize: 14; color: AppStyle.textSecondary }
                    Text {
                        text: "Tải lên"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; color: AppStyle.textSecondary
                    }
                }
                MouseArea {
                    id: uploadMa; anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: root.uploadTriggered()
                }
            }

            // Separator
            Rectangle { width: 1; height: 20; color: AppStyle.borderColor }

            // Search field
            Rectangle {
                id: searchBg
                Layout.fillWidth: true; height: 36; radius: AppStyle.radiusFull
                color: searchField.activeFocus ? AppStyle.cardBackground : "#F1F3F4"
                border.color: searchField.activeFocus ? AppStyle.primaryColor : "transparent"
                border.width: searchField.activeFocus ? 2 : 0
                Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                RowLayout {
                    anchors { fill: parent; leftMargin: 12; rightMargin: 8 }; spacing: 8

                    Text { text: "🔍"; font.pixelSize: 14; color: AppStyle.textDisabled }

                    TextField {
                        id: searchField
                        Layout.fillWidth: true; Layout.fillHeight: true
                        placeholderText: "Tìm kiếm trong Drive..."
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                        color: AppStyle.textPrimary; background: Item {}

                        Keys.onReturnPressed: root.searchTriggered(text)
                        Keys.onEscapePressed: { text = ""; root.searchTriggered("") }
                    }

                    // Clear button
                    Text {
                        visible: searchField.text !== ""
                        text: "✕"; font.pixelSize: 12; color: AppStyle.textSecondary
                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                            onClicked: { searchField.text = ""; root.searchTriggered("") }
                        }
                    }
                }
            }

            // Icon buttons row
            RowLayout {
                spacing: 2; Layout.rightMargin: 8

                // Refresh
                RoundButton {
                    flat: true; width: 36; height: 36; padding: 0
                    contentItem: Text {
                        text: "⟳"; font.pixelSize: 18; horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter; color: AppStyle.textSecondary
                    }
                    background: Rectangle {
                        radius: 18
                        color: parent.hovered ? AppStyle.hoverColor : "transparent"
                    }
                    onClicked: root.refreshTriggered()
                    ToolTip.visible: hovered; ToolTip.text: "Làm mới (F5)"; ToolTip.delay: 800
                }

                // Grid/List toggle
                RoundButton {
                    flat: true; width: 36; height: 36; padding: 0
                    contentItem: Text {
                        text: root.viewMode === "grid" ? "☰" : "⊞"
                        font.pixelSize: 17; horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter; color: AppStyle.textSecondary
                    }
                    background: Rectangle {
                        radius: 18
                        color: parent.hovered ? AppStyle.hoverColor : "transparent"
                    }
                    onClicked: root.toggleViewTriggered()
                    ToolTip.visible: hovered
                    ToolTip.text: root.viewMode === "grid" ? "Chuyển sang danh sách" : "Chuyển sang lưới"
                    ToolTip.delay: 800
                }

                // More menu
                RoundButton {
                    id: moreBtn
                    flat: true; width: 36; height: 36; padding: 0
                    contentItem: Text {
                        text: "⋮"; font.pixelSize: 20; horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter; color: AppStyle.textSecondary
                    }
                    background: Rectangle {
                        radius: 18
                        color: parent.hovered || moreMenu.visible ? AppStyle.hoverColor : "transparent"
                    }
                    onClicked: moreMenu.open()
                }

                Menu {
                    id: moreMenu
                    x: moreBtn.x - width + moreBtn.width
                    y: moreBtn.height + 4

                    MenuItem { text: "Đổi tên (F2)";           onTriggered: root.menuActionTriggered("rename")     }
                    MenuItem { text: "Sao chép (Ctrl+C)";       onTriggered: root.menuActionTriggered("copy")       }
                    MenuItem { text: "Cắt (Ctrl+X)";            onTriggered: root.menuActionTriggered("cut")        }
                    MenuItem { text: "Dán (Ctrl+V)";            onTriggered: root.menuActionTriggered("paste")      }
                    MenuItem { text: "Xóa (Del)";               onTriggered: root.menuActionTriggered("delete")     }
                    MenuSeparator {}
                    MenuItem { text: "Thuộc tính";              onTriggered: root.menuActionTriggered("properties") }
                    MenuSeparator {}
                    MenuItem { text: "Xem Log";                 onTriggered: root.menuActionTriggered("viewlog")    }
                    MenuItem { text: "Thông tin License";       onTriggered: root.menuActionTriggered("license")    }
                }
            }
        }

        // ── Breadcrumb row ────────────────────────────────────────────
        RowLayout {
            id: breadcrumbRow
            Layout.fillWidth: true
            Layout.leftMargin: 16; Layout.rightMargin: 16
            height: 30; spacing: 4

            Repeater {
                model: root.currentPath.replace(/^\//, "").split("/")
                delegate: RowLayout {
                    spacing: 4
                    Text {
                        visible: index > 0
                        text: "›"; font.pixelSize: 14; color: AppStyle.textDisabled
                    }
                    Text {
                        text: modelData
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                        color: index === (root.currentPath.replace(/^\//, "").split("/").length - 1)
                               ? AppStyle.textPrimary : AppStyle.textSecondary
                        font.weight: index === (root.currentPath.replace(/^\//, "").split("/").length - 1)
                                     ? Font.Medium : Font.Normal
                    }
                }
            }

            Item { Layout.fillWidth: true }

            Text {
                text: fileModel.rowCount() + " mục"
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                color: AppStyle.textDisabled
            }
        }
    }
}
