import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: contentAreaRoot
    color: "#ffffff"
    focus: true

    property string selectedFilePath: ""
    property string selectedFileName: ""

    // Context Menu for right-click item actions
    Menu {
        id: contextMenu
        width: 190

        background: Rectangle {
            color: "#ffffff"
            radius: 12
            border.color: "#dadce0"
            border.width: 1
        }

        delegate: MenuItem {
            id: ctxDelegate
            implicitHeight: 36
            implicitWidth: 190

            contentItem: Text {
                text: ctxDelegate.text
                color: ctxDelegate.highlighted ? window.primaryColor : window.textLight
                font.pixelSize: 13
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
                rightPadding: 12
            }

            background: Rectangle {
                color: ctxDelegate.highlighted ? "#f1f3f4" : "transparent"
                radius: 8
            }
        }

        MenuItem {
            text: "📂 Mở (Open)"
            onTriggered: {
                if (selectedFilePath !== "") appCore.fileModel.browseTo(selectedFilePath)
            }
        }
        MenuItem {
            text: "✏️ Đổi tên (F2)"
            onTriggered: renameDialog.open()
        }
        MenuItem {
            text: "🗑️ Xóa (Delete)"
            onTriggered: {
                if (selectedFileName !== "") appCore.fileModel.deleteItem(selectedFileName)
            }
        }
        MenuItem {
            text: "📋 Sao chép (Ctrl+C)"
            onTriggered: appCore.logOperation("Copied: " + selectedFileName)
        }
        MenuItem {
            text: "✂️ Cắt (Ctrl+X)"
            onTriggered: appCore.logOperation("Cut: " + selectedFileName)
        }
        MenuItem {
            text: "ℹ️ Thuộc tính (Properties)"
            onTriggered: appCore.menuAction("Properties", selectedFilePath)
        }
    }

    // Rename Dialog
    Dialog {
        id: renameDialog
        anchors.centerIn: parent
        modal: true
        width: 360
        height: 200

        header: Rectangle {
            height: 42
            color: "#f8f9fa"
            radius: 12
            border.color: "#dadce0"

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 12
                color: "#f8f9fa"
            }

            Text {
                anchors.centerIn: parent
                text: "✏️ Đổi tên tệp / thư mục"
                color: window.textLight
                font.pixelSize: 14
                font.bold: true
            }
        }

        background: Rectangle {
            color: "#ffffff"
            radius: 12
            border.color: "#dadce0"
            border.width: 1
        }

        contentItem: ColumnLayout {
            spacing: 14

            Text {
                text: "Nhập tên mới:"
                color: window.textLight
                font.pixelSize: 13
                font.bold: true
            }

            TextField {
                id: renameInput
                Layout.fillWidth: true
                text: selectedFileName
                color: window.textLight
                focus: true
                leftPadding: 10
                background: Rectangle {
                    color: "#f1f3f4"
                    radius: 8
                    border.color: renameInput.activeFocus ? window.primaryColor : "#dadce0"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 8

                Button {
                    text: "Hủy"
                    implicitWidth: 80
                    implicitHeight: 32
                    onClicked: renameDialog.close()
                    contentItem: Text {
                        text: "Hủy"
                        color: window.textLight
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? "#e8eaed" : "#f1f3f4"
                        radius: 8
                    }
                }

                Button {
                    text: "Lưu"
                    implicitWidth: 80
                    implicitHeight: 32
                    onClicked: {
                        if (renameInput.text.trim() !== "" && selectedFileName !== "") {
                            appCore.fileModel.renameItem(selectedFileName, renameInput.text.trim())
                            renameDialog.close()
                        }
                    }
                    contentItem: Text {
                        text: "Lưu"
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

    // Shortcut handlers
    Shortcut { sequence: "Delete"; onActivated: { if (selectedFileName !== "") appCore.fileModel.deleteItem(selectedFileName) } }
    Shortcut { sequence: "F2"; onActivated: { if (selectedFileName !== "") renameDialog.open() } }
    Shortcut { sequence: "Ctrl+N"; onActivated: appCore.createFolder("Thư mục mới") }
    Shortcut { sequence: "Ctrl+F"; onActivated: appCore.search("") }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Top Banner / Welcome to Drive Title
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: fileModel && fileModel.isRoot ? "Welcome to Drive" : "My Drive"
                color: window.textLight
                font.pixelSize: 22
                font.bold: true
            }

            Item { Layout.fillWidth: true }
        }

        // Breadcrumb Path Bar (Google Drive Dropdown / Breadcrumbs Style)
        Rectangle {
            Layout.fillWidth: true
            height: 38
            color: "#f8f9fa"
            radius: 8
            border.color: "#e0e0e0"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                spacing: 6

                Text {
                    text: "📁"
                    font.pixelSize: 14
                }

                // Interactive Breadcrumb Segments
                RowLayout {
                    spacing: 4

                    Text {
                        text: "My Drive"
                        color: window.primaryColor
                        font.pixelSize: 13
                        font.bold: true

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (fileModel) fileModel.browseTo(fileModel.currentPath.split("/cloud_root")[0] + "/cloud_root")
                            }
                        }
                    }

                    // Render subpath if inside subfolder
                    Text {
                        text: {
                            if (!fileModel) return ""
                            var path = fileModel.currentPath
                            if (path.indexOf("/cloud_root") >= 0) {
                                var sub = path.substring(path.indexOf("/cloud_root") + 11)
                                return sub.length > 0 ? " › " + sub.replace(/\//g, " › ") : ""
                            }
                            return " › " + path
                        }
                        color: window.textLight
                        font.pixelSize: 13
                        font.bold: true
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }

        // Empty Folder Hint Display
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: fileModel ? fileModel.count === 0 : true
            color: "#fafafa"
            radius: 12
            border.color: "#e0e0e0"

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 12

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "☁️"
                    font.pixelSize: 48
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Thư mục hiện tại đang trống"
                    color: window.textLight
                    font.pixelSize: 16
                    font.bold: true
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Bấm '+ Mới' hoặc 'Tải lên' để bắt đầu lưu trữ tệp tin."
                    color: window.textMuted
                    font.pixelSize: 13
                }
            }
        }

        // GridView mode
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: appCore.viewMode === "grid" && (fileModel ? fileModel.count > 0 : false)

            GridView {
                id: fileGridView
                anchors.fill: parent
                cellWidth: 165
                cellHeight: 155
                clip: true
                model: fileModel

                delegate: FileItemDelegate {
                    width: fileGridView.cellWidth - 12
                    height: fileGridView.cellHeight - 12
                    viewMode: "grid"

                    onItemClicked: function(path, name) {
                        contentAreaRoot.selectedFilePath = path
                        contentAreaRoot.selectedFileName = name
                    }
                    onItemDoubleClicked: function(path, isDir) {
                        if (isDir) {
                            fileModel.browseTo(path)
                        } else {
                            appCore.logOperation("Opened file: " + path)
                            appCore.menuAction("Properties", path)
                        }
                    }
                    onItemRightClicked: function(path, name) {
                        contentAreaRoot.selectedFilePath = path
                        contentAreaRoot.selectedFileName = name
                        contextMenu.popup()
                    }
                }
            }
        }

        // ListView mode
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: appCore.viewMode === "list" && (fileModel ? fileModel.count > 0 : false)

            ListView {
                id: fileListView
                anchors.fill: parent
                spacing: 6
                clip: true
                model: fileModel

                delegate: FileItemDelegate {
                    width: fileListView.width - 24
                    height: 46
                    viewMode: "list"

                    onItemClicked: function(path, name) {
                        contentAreaRoot.selectedFilePath = path
                        contentAreaRoot.selectedFileName = name
                    }
                    onItemDoubleClicked: function(path, isDir) {
                        if (isDir) {
                            fileModel.browseTo(path)
                        } else {
                            appCore.logOperation("Opened file: " + path)
                            appCore.menuAction("Properties", path)
                        }
                    }
                    onItemRightClicked: function(path, name) {
                        contentAreaRoot.selectedFilePath = path
                        contentAreaRoot.selectedFileName = name
                        contextMenu.popup()
                    }
                }
            }
        }
    }
}
