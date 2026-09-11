import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: toolbarRoot
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1

    signal openLicensePopup()

    // Dialog popup for LogViewer
    LogViewer {
        id: logViewerDialog
    }

    // Input Dialog for Create Folder
    Dialog {
        id: folderDialog
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
                id: folderNameInput
                Layout.fillWidth: true
                placeholderText: "Thư mục mới"
                color: window.textLight
                placeholderTextColor: window.textMuted
                focus: true
                leftPadding: 10
                background: Rectangle {
                    color: "#f1f3f4"
                    radius: 8
                    border.color: folderNameInput.activeFocus ? window.primaryColor : "#dadce0"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 8

                Button {
                    text: "Hủy"
                    implicitWidth: 80
                    implicitHeight: 32
                    onClicked: folderDialog.close()
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
                        if (folderNameInput.text.trim() !== "") {
                            appCore.createFolder(folderNameInput.text.trim())
                            folderNameInput.text = ""
                            folderDialog.close()
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

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 10

        // Google Drive Logo Brand
        RowLayout {
            spacing: 8
            Rectangle {
                width: 32
                height: 32
                radius: 8
                color: "#e8f0fe"
                Text {
                    anchors.centerIn: parent
                    text: "☁"
                    color: window.primaryColor
                    font.pixelSize: 20
                }
            }
            Text {
                text: "Drive"
                color: window.textLight
                font.pixelSize: 18
                font.bold: true
            }
        }

        // Navigation Back & Forward Buttons
        RowLayout {
            spacing: 4

            // Back Button (◀)
            Button {
                id: backBtn
                implicitWidth: 32
                implicitHeight: 32
                enabled: fileModel ? fileModel.canGoBack : false
                onClicked: appCore.goBack()

                contentItem: Text {
                    text: "◀"
                    color: backBtn.enabled ? window.textLight : "#bdc1c6"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: backBtn.hovered && backBtn.enabled ? "#f1f3f4" : "transparent"
                    radius: 16
                }
            }

            // Forward Button (▶)
            Button {
                id: forwardBtn
                implicitWidth: 32
                implicitHeight: 32
                enabled: fileModel ? fileModel.canGoForward : false
                onClicked: appCore.goForward()

                contentItem: Text {
                    text: "▶"
                    color: forwardBtn.enabled ? window.textLight : "#bdc1c6"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: forwardBtn.hovered && forwardBtn.enabled ? "#f1f3f4" : "transparent"
                    radius: 16
                }
            }
        }

        // Search Bar (Google Drive Pill Style)
        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 520
            height: 44
            radius: 22
            color: "#f1f3f4"
            border.color: searchInput.activeFocus ? window.primaryColor : "transparent"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 14
                anchors.rightMargin: 14
                spacing: 8

                Text {
                    text: "🔍"
                    font.pixelSize: 14
                    color: window.textMuted
                }

                TextField {
                    id: searchInput
                    Layout.fillWidth: true
                    placeholderText: "Search in Drive (Tìm kiếm trong Drive)..."
                    color: window.textLight
                    placeholderTextColor: window.textMuted
                    font.pixelSize: 13
                    background: null

                    onTextChanged: {
                        appCore.search(text.trim())
                    }
                }

                // Clear Search Button (✕)
                Text {
                    text: "✕"
                    font.pixelSize: 14
                    color: window.textMuted
                    visible: searchInput.text.length > 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            searchInput.text = ""
                            appCore.search("")
                        }
                    }
                }
            }
        }

        Item { Layout.fillWidth: true }

        // Refresh Button (⟳)
        Button {
            id: refreshBtn
            implicitHeight: 36
            implicitWidth: 36
            onClicked: appCore.refresh()

            contentItem: Text {
                text: "⟳"
                color: window.textMuted
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: refreshBtn.hovered ? "#f1f3f4" : "transparent"
                radius: 18
            }
        }

        // Grid/List View Toggle
        Button {
            id: viewToggleBtn
            implicitHeight: 36
            implicitWidth: 36
            onClicked: appCore.toggleView()

            contentItem: Text {
                text: appCore.viewMode === "grid" ? "☰" : "⊞"
                color: window.textMuted
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: viewToggleBtn.hovered ? "#f1f3f4" : "transparent"
                radius: 18
            }
        }

        // License badge button
        Rectangle {
            implicitHeight: 32
            implicitWidth: 105
            radius: 16
            color: "#e6f4ea"
            border.color: window.accentGreen

            MouseArea {
                anchors.fill: parent
                onClicked: toolbarRoot.openLicensePopup()
            }

            RowLayout {
                anchors.centerIn: parent
                spacing: 4
                Text { text: "●"; color: window.accentGreen; font.pixelSize: 10 }
                Text {
                    text: (licenseInfo ? licenseInfo.remainingDays : 365) + " ngày"
                    color: window.accentGreen
                    font.pixelSize: 11
                    font.bold: true
                }
            }
        }

        // Action Menu Button (⋮)
        Button {
            id: menuBtn
            implicitHeight: 36
            implicitWidth: 36
            onClicked: actionMenu.open()

            contentItem: Text {
                text: "⋮"
                color: window.textMuted
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: menuBtn.hovered ? "#f1f3f4" : "transparent"
                radius: 18
            }

            Menu {
                id: actionMenu
                y: menuBtn.height + 4
                width: 200

                background: Rectangle {
                    color: "#ffffff"
                    radius: 12
                    border.color: "#dadce0"
                    border.width: 1
                }

                delegate: MenuItem {
                    id: menuDelegate
                    implicitHeight: 38
                    implicitWidth: 200

                    contentItem: Text {
                        text: menuDelegate.text
                        color: menuDelegate.highlighted ? window.primaryColor : window.textLight
                        font.pixelSize: 13
                        font.bold: menuDelegate.highlighted
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 12
                        rightPadding: 12
                    }

                    background: Rectangle {
                        color: menuDelegate.highlighted ? "#f1f3f4" : "transparent"
                        radius: 8
                    }
                }

                MenuItem {
                    text: "📜 Xem Nhật Ký (Log)"
                    onTriggered: logViewerDialog.openLogViewer(appCore.readLogFile())
                }
                MenuItem {
                    text: "🔑 Thông tin License"
                    onTriggered: toolbarRoot.openLicensePopup()
                }
                MenuItem {
                    text: "⟳ Tải lại thư mục"
                    onTriggered: appCore.refresh()
                }
                MenuItem {
                    text: "🚪 Đăng xuất tài khoản"
                    onTriggered: appCore.logout()
                }
            }
        }
    }
}
