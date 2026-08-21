import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// ContentArea.qml — Vùng hiển thị file/folder (Grid + List)
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root

    property string viewMode: "grid"    // "grid" | "list"
    property int    selectedIndex: -1

    signal itemDoubleClicked(string path, bool isFolder)
    signal contextActionTriggered(string action, string path)

    // Clipboard slots
    function copySelected()   { if (selectedIndex >= 0) contextActionTriggered("copy",   _pathAt(selectedIndex)) }
    function cutSelected()    { if (selectedIndex >= 0) contextActionTriggered("cut",    _pathAt(selectedIndex)) }
    function paste()          { contextActionTriggered("paste", "") }
    function deleteSelected() { if (selectedIndex >= 0) contextActionTriggered("delete", _pathAt(selectedIndex)) }
    function renameSelected() { if (selectedIndex >= 0) contextActionTriggered("rename", _pathAt(selectedIndex)) }

    function _pathAt(idx) {
        const m = fileModel
        return m.data(m.index(idx, 0), 260)  // FilePathRole = Qt.UserRole + 4 → resolved by name below
    }

    // ── Context Menu ──────────────────────────────────────────────────
    Menu {
        id: contextMenu
        property string targetPath: ""
        property bool   targetIsFolder: false

        MenuItem { text: "Mở";            onTriggered: root.contextActionTriggered("open",       contextMenu.targetPath) }
        MenuItem { text: "Tải xuống";     visible: !contextMenu.targetIsFolder
                                          onTriggered: root.contextActionTriggered("download",   contextMenu.targetPath) }
        MenuSeparator {}
        MenuItem { text: "Đổi tên  F2";  onTriggered: root.contextActionTriggered("rename",     contextMenu.targetPath) }
        MenuItem { text: "Sao chép  Ctrl+C"; onTriggered: root.contextActionTriggered("copy",   contextMenu.targetPath) }
        MenuItem { text: "Cắt  Ctrl+X";  onTriggered: root.contextActionTriggered("cut",        contextMenu.targetPath) }
        MenuItem { text: "Di chuyển đến..."; onTriggered: root.contextActionTriggered("move",   contextMenu.targetPath) }
        MenuSeparator {}
        MenuItem { text: "Thuộc tính";   onTriggered: root.contextActionTriggered("properties", contextMenu.targetPath) }
        MenuItem {
            text: "Xóa  Del"
            contentItem: Text {
                text: parent.text
                color: AppStyle.errorColor
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
            }
            onTriggered: root.contextActionTriggered("delete", contextMenu.targetPath)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ── List-mode column header ───────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: 36
            color: AppStyle.backgroundColor
            visible: root.viewMode === "list"

            Rectangle {
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                height: 1; color: AppStyle.borderColor
            }

            RowLayout {
                anchors { fill: parent; leftMargin: 58; rightMargin: 16 }
                spacing: 10

                Text {
                    Layout.fillWidth: true
                    text: "Tên"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    font.weight: Font.Medium; color: AppStyle.textSecondary
                }
                Text {
                    Layout.preferredWidth: 80
                    text: "Kích thước"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    font.weight: Font.Medium; color: AppStyle.textSecondary
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    Layout.preferredWidth: 120
                    text: "Ngày sửa"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    font.weight: Font.Medium; color: AppStyle.textSecondary
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // ── Grid view ─────────────────────────────────────────────────
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            visible: root.viewMode === "grid"
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            GridView {
                id: gridView
                model: fileModel
                anchors.fill: parent
                topMargin: 16; leftMargin: 12; rightMargin: 12; bottomMargin: 12
                cellWidth: 112; cellHeight: 128

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: AppStyle.durationMid; easing.type: Easing.OutCubic }
                }

                delegate: FileItemDelegate {
                    width: 108; height: 124

                    itemName:     model.fileName
                    itemSize:     model.fileSize
                    itemModified: model.fileModified
                    itemIcon:     model.fileIcon
                    itemIsFolder: model.isFolder
                    selected:     root.selectedIndex === index
                    displayMode:  "grid"

                    onSingleClicked: root.selectedIndex = index
                    onDoubleClicked: root.itemDoubleClicked(model.filePath, model.isFolder)
                    onRightClicked: (mx, my) => {
                        root.selectedIndex = index
                        contextMenu.targetPath     = model.filePath
                        contextMenu.targetIsFolder = model.isFolder
                        contextMenu.popup(mx, my)
                    }
                }

                // Empty state
                Text {
                    anchors.centerIn: parent
                    visible: gridView.count === 0
                    text: "Thư mục này trống"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeMD
                    color: AppStyle.textDisabled
                }
            }
        }

        // ── List view ─────────────────────────────────────────────────
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            visible: root.viewMode === "list"
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                id: listView
                model: fileModel
                anchors.fill: parent
                spacing: 0

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: AppStyle.durationMid; easing.type: Easing.OutCubic }
                }

                delegate: FileItemDelegate {
                    width: listView.width; height: 44

                    itemName:     model.fileName
                    itemSize:     model.fileSize
                    itemModified: model.fileModified
                    itemIcon:     model.fileIcon
                    itemIsFolder: model.isFolder
                    selected:     root.selectedIndex === index
                    displayMode:  "list"

                    onSingleClicked: root.selectedIndex = index
                    onDoubleClicked: root.itemDoubleClicked(model.filePath, model.isFolder)
                    onRightClicked: (mx, my) => {
                        root.selectedIndex = index
                        contextMenu.targetPath     = model.filePath
                        contextMenu.targetIsFolder = model.isFolder
                        contextMenu.popup(mx, my)
                    }
                }

                Text {
                    anchors.centerIn: parent
                    visible: listView.count === 0
                    text: "Thư mục này trống"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeMD
                    color: AppStyle.textDisabled
                }
            }
        }
    }

    // Deselect on background click
    MouseArea {
        anchors.fill: parent
        z: -1
        onClicked: root.selectedIndex = -1
    }
}
