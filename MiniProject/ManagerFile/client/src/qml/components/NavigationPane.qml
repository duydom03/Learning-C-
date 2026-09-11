import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: navPaneRoot
    color: "#f8f9fa"
    border.color: "#e0e0e0"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        spacing: 12

        // Google Drive "+ Mới" (+ New) Action Button
        Button {
            id: newBtn
            Layout.leftMargin: 16
            implicitWidth: 120
            implicitHeight: 48
            onClicked: newMenu.open()

            contentItem: RowLayout {
                spacing: 10
                Text { text: "+"; color: "#ea4335"; font.pixelSize: 24; font.bold: true }
                Text { text: "Mới"; color: window.textLight; font.pixelSize: 14; font.bold: true }
            }

            background: Rectangle {
                color: newBtn.pressed ? "#e8eaed" : (newBtn.hovered ? "#f1f3f4" : "#ffffff")
                radius: 16
                border.color: "#dadce0"
                border.width: 1

                // Subtle shadow effect
                Rectangle {
                    anchors.fill: parent
                    radius: 16
                    color: "transparent"
                    border.color: "#1f000000"
                    border.width: 1
                }
            }

            Menu {
                id: newMenu
                y: newBtn.height + 4
                width: 190

                background: Rectangle {
                    color: "#ffffff"
                    radius: 12
                    border.color: "#dadce0"
                    border.width: 1
                }

                delegate: MenuItem {
                    id: newMenuDelegate
                    implicitHeight: 38
                    implicitWidth: 190

                    contentItem: Text {
                        text: newMenuDelegate.text
                        color: newMenuDelegate.highlighted ? window.primaryColor : window.textLight
                        font.pixelSize: 13
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 12
                        rightPadding: 12
                    }

                    background: Rectangle {
                        color: newMenuDelegate.highlighted ? "#f1f3f4" : "transparent"
                        radius: 8
                    }
                }

                MenuItem {
                    text: "📁 Tạo thư mục mới"
                    onTriggered: navFolderDialog.open()
                }
                MenuItem {
                    text: "⬆ Tải tệp lên"
                    onTriggered: appCore.upload()
                }
            }
        }

        // Input Dialog for Create Folder
        Dialog {
            id: navFolderDialog
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
                    text: "📁 Tạo thư mục mới"
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
                    text: "Nhập tên thư mục mới:"
                    color: window.textLight
                    font.pixelSize: 13
                    font.bold: true
                }

                TextField {
                    id: navFolderNameInput
                    Layout.fillWidth: true
                    placeholderText: "Thư mục mới"
                    color: window.textLight
                    placeholderTextColor: window.textMuted
                    focus: true
                    leftPadding: 10
                    background: Rectangle {
                        color: "#f1f3f4"
                        radius: 8
                        border.color: navFolderNameInput.activeFocus ? window.primaryColor : "#dadce0"
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    spacing: 8

                    Button {
                        text: "Hủy"
                        implicitWidth: 80
                        implicitHeight: 32
                        onClicked: navFolderDialog.close()
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
                        text: "Tạo"
                        implicitWidth: 80
                        implicitHeight: 32
                        onClicked: {
                            if (navFolderNameInput.text.trim() !== "") {
                                appCore.createFolder(navFolderNameInput.text.trim())
                                navFolderNameInput.text = ""
                                navFolderDialog.close()
                            }
                        }
                        contentItem: Text {
                            text: "Tạo"
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

        // Quick Drive Section Items
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            // Home Item
            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                height: 36
                radius: 18
                color: "#e8f0fe"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    spacing: 12

                    Text { text: "🏠"; font.pixelSize: 14 }
                    Text { text: "Trang chủ (Home)"; color: window.primaryColor; font.pixelSize: 13; font.bold: true }
                }
            }

            // My Drive Item
            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                height: 36
                radius: 18
                color: fileModel && fileModel.isRoot ? "#e8f0fe" : "transparent"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (fileModel) fileModel.browseTo(fileModel.currentPath.split("/cloud_root")[0] + "/cloud_root")
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    spacing: 12

                    Text { text: "☁"; color: window.primaryColor; font.pixelSize: 14 }
                    Text {
                        text: "My Drive (Bộ nhớ của tôi)"
                        color: window.textLight
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#e0e0e0"
            Layout.leftMargin: 16
            Layout.rightMargin: 16
        }

        // Header Title for Tree View
        Text {
            Layout.leftMargin: 20
            text: "DANH MỤC THƯ MỤC"
            color: window.textMuted
            font.pixelSize: 11
            font.bold: true
        }

        // Directory Tree View listing folders (Fixed: columnWidthProvider renders only Column 0)
        TreeView {
            id: folderTreeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: folderModel

            // Crucial Fix: Force column 0 to take full width and hide columns 1, 2, 3
            columnWidthProvider: function(col) {
                return col === 0 ? folderTreeView.width : 0
            }

            delegate: TreeViewDelegate {
                id: treeDelegate
                implicitHeight: 32
                visible: column === 0 // Ensure only column 0 renders

                contentItem: RowLayout {
                    spacing: 8

                    Text {
                        text: "📁"
                        font.pixelSize: 14
                    }

                    Text {
                        text: model.fileName !== undefined ? model.fileName : ""
                        color: treeDelegate.current ? window.primaryColor : window.textLight
                        font.pixelSize: 12
                        font.bold: treeDelegate.current
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }

                background: Rectangle {
                    color: treeDelegate.current ? "#e8f0fe" : (treeDelegate.hovered ? "#f1f3f4" : "transparent")
                    radius: 16
                }

                onClicked: {
                    if (model.filePath !== undefined) {
                        fileModel.browseTo(model.filePath)
                    }
                }
            }
        }

        // Storage Indicator Footer
        Rectangle {
            Layout.fillWidth: true
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            height: 70
            color: "#ffffff"
            radius: 12
            border.color: "#e0e0e0"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 6

                RowLayout {
                    Text { text: "☁ Bộ nhớ:"; color: window.textMuted; font.pixelSize: 11 }
                    Item { Layout.fillWidth: true }
                    Text { text: "7.02 GB / 20 GB"; color: window.textLight; font.pixelSize: 11; font.bold: true }
                }

                ProgressBar {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 6
                    value: 0.35
                    background: Rectangle { color: "#e8eaed"; radius: 3 }
                    contentItem: Item {
                        Rectangle {
                            width: parent.width * 0.35
                            height: parent.height
                            radius: 3
                            color: window.primaryColor
                        }
                    }
                }
            }
        }
    }
}
