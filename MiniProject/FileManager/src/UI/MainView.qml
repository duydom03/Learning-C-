import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "styles"
import "components"

// ─────────────────────────────────────────────────────────────────────────────
// MainView.qml — Layout chính của ứng dụng
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root

    signal logoutRequested()

    property string currentPath: "/MyDrive"

    // ── Global keyboard shortcuts ─────────────────────────────────────
    Shortcut { sequence: "Ctrl+N";  onActivated: toolbar.createFolderRequested() }
    Shortcut { sequence: "Ctrl+F";  onActivated: toolbar.focusSearch() }
    Shortcut { sequence: "F5";      onActivated: { fileModel.refreshCurrentFolder(); toast.show("Đã làm mới", "success") } }
    Shortcut { sequence: "Ctrl+C";  onActivated: contentArea.copySelected() }
    Shortcut { sequence: "Ctrl+X";  onActivated: contentArea.cutSelected()  }
    Shortcut { sequence: "Ctrl+V";  onActivated: contentArea.paste()        }
    Shortcut { sequence: "Delete";  onActivated: contentArea.deleteSelected() }
    Shortcut { sequence: "F2";      onActivated: contentArea.renameSelected() }

    // ── Root column ───────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ── App Bar ───────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: AppStyle.appBarHeight
            color:  AppStyle.cardBackground

            Rectangle {
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                height: 1; color: AppStyle.borderColor
            }

            RowLayout {
                anchors { fill: parent; leftMargin: 16; rightMargin: 12 }
                spacing: 10

                // Logo
                Rectangle {
                    width: 32; height: 32; radius: 8
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: AppStyle.primaryColor }
                        GradientStop { position: 1; color: AppStyle.primaryDark  }
                    }
                    Text {
                        anchors.centerIn: parent
                        text: "FM"; font.pixelSize: 13; font.weight: Font.Bold; color: "#FFF"
                    }
                }

                Text {
                    text: "FileManager"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeMD; font.weight: Font.Medium
                    color: AppStyle.textPrimary
                }

                // Active badge
                Rectangle {
                    height: 20; radius: AppStyle.radiusFull
                    width: activeBadge.implicitWidth + 12
                    color: AppStyle.successFaint

                    Text {
                        id: activeBadge; anchors.centerIn: parent
                        text: "● Đã kích hoạt"
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                        color: AppStyle.successColor
                    }
                }

                Item { Layout.fillWidth: true }

                // User info
                Text {
                    text: licenseManager.email
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary
                }

                // Avatar
                Rectangle {
                    width: 32; height: 32; radius: 16; color: AppStyle.primaryColor

                    Text {
                        anchors.centerIn: parent
                        text: licenseManager.customer.length > 0
                              ? licenseManager.customer[0].toUpperCase()
                              : "U"
                        font.pixelSize: 14; font.weight: Font.Bold; color: "#FFF"
                    }

                    MouseArea {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: userMenu.open()
                    }
                }

                // User context menu
                Menu {
                    id: userMenu

                    MenuItem {
                        text: "Thông tin License"
                        onTriggered: licenseInfoDialog.open()
                    }
                    MenuItem {
                        text: "Xem Log"
                        onTriggered: logViewerDialog.open()
                    }
                    MenuSeparator {}
                    MenuItem {
                        text: "Đăng xuất / Hủy kích hoạt"
                        onTriggered: root.logoutRequested()
                    }
                }
            }
        }

        // ── Toolbar + Content area ─────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // ── Navigation pane ───────────────────────────────────────
            NavigationPane {
                id: navPane
                Layout.preferredWidth: splitHandle.navWidth
                Layout.minimumWidth:   AppStyle.navPaneMin
                Layout.maximumWidth:   AppStyle.navPaneMax
                Layout.fillHeight:     true
                currentPath: root.currentPath

                onFolderSelected: function(path) {
                    root.currentPath = path
                    fileModel.loadFolder(path)
                }
            }

            // ── Resize handle ─────────────────────────────────────────
            Rectangle {
                id: splitHandle
                property int navWidth: AppStyle.navPaneWidth

                Layout.fillHeight: true
                width: 4
                color: splitHandleMa.containsMouse || splitHandleMa.pressed
                       ? AppStyle.primaryColor
                       : "transparent"

                Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                MouseArea {
                    id: splitHandleMa
                    anchors.fill: parent
                    cursorShape: Qt.SplitHCursor
                    hoverEnabled: true
                    property int startX: 0

                    onPressed: (mouse) => { startX = mouse.x }
                    onPositionChanged: (mouse) => {
                        if (!pressed) return
                        const newW = splitHandle.navWidth + (mouse.x - startX)
                        splitHandle.navWidth = Math.min(AppStyle.navPaneMax,
                                                        Math.max(AppStyle.navPaneMin, newW))
                    }
                }
            }

            // ── Right panel ───────────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                // Toolbar
                Toolbar {
                    id: toolbar
                    Layout.fillWidth: true
                    viewMode: contentArea.viewMode
                    currentPath: root.currentPath

                    onCreateFolderTriggered: {
                        createFolderDialog.open()
                    }
                    onUploadTriggered: {
                        progressOverlay.show("Đang tải tệp lên...", 0)
                        uploadSimTimer.start()
                    }
                    onRefreshTriggered: {
                        fileModel.refreshCurrentFolder()
                        toast.show("Đã làm mới danh sách", "success")
                    }
                    onSearchTriggered: function(keyword) {
                        fileModel.searchFiles(keyword)
                        if (keyword !== "")
                            toast.show("Tìm thấy kết quả cho \"" + keyword + "\"", "info")
                    }
                    onToggleViewTriggered: {
                        contentArea.viewMode = (contentArea.viewMode === "grid") ? "list" : "grid"
                    }
                    onMenuActionTriggered: function(action) {
                        if (action === "viewlog")      logViewerDialog.open()
                        else if (action === "license") licenseInfoDialog.open()
                        else toast.show(action, "info")
                    }
                }

                // Content area
                ContentArea {
                    id: contentArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    onItemDoubleClicked: function(path, isFolder) {
                        if (isFolder) {
                            root.currentPath = path
                            fileModel.loadFolder(path)
                        } else {
                            toast.show("Đang mở tệp...", "info")
                        }
                    }
                    onContextActionTriggered: function(action, path) {
                        handleContextAction(action, path)
                    }
                }
            }
        }
    }

    // ── Upload progress simulation ─────────────────────────────────────
    property int _uploadProgress: 0
    Timer {
        id: uploadSimTimer
        interval: 350; repeat: true
        onTriggered: {
            root._uploadProgress = Math.min(100, root._uploadProgress + Math.round(Math.random() * 15 + 5))
            progressOverlay.update(root._uploadProgress)
            if (root._uploadProgress >= 100) {
                stop()
                root._uploadProgress = 0
                progressOverlay.hide()
                toast.show("Tải lên hoàn thành!", "success")
            }
        }
    }

    // ── Create folder dialog ──────────────────────────────────────────
    Dialog {
        id: createFolderDialog
        title: "Tạo thư mục mới"
        modal: true
        anchors.centerIn: Overlay.overlay
        width: 360

        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            width: parent.width
            spacing: 8

            Text {
                text: "Tên thư mục:"
                font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                color: AppStyle.textSecondary
            }
            TextField {
                id: folderNameField
                Layout.fillWidth: true
                placeholderText: "Nhập tên thư mục..."
                font.family: AppStyle.fontFamily
                onAccepted: createFolderDialog.accept()
            }
        }

        onOpened: { folderNameField.text = ""; folderNameField.forceActiveFocus() }
        onAccepted: {
            if (folderNameField.text.trim() !== "") {
                fileModel.createFolder(root.currentPath, folderNameField.text.trim())
                toast.show("Đã tạo thư mục \"" + folderNameField.text.trim() + "\"", "success")
            }
        }
    }

    // ── Context action handler ────────────────────────────────────────
    function handleContextAction(action, path) {
        switch (action) {
        case "delete":
            fileModel.deleteFile(path)
            toast.show("Đã xóa tệp", "error")
            break
        case "rename":
            renameDialog.targetPath = path
            renameDialog.open()
            break
        case "copy":
            toast.show("Đã sao chép vào clipboard", "info")
            break
        case "cut":
            toast.show("Đã cắt", "info")
            break
        case "paste":
            toast.show("Đã dán", "info")
            break
        case "download":
            toast.show("Đang tải xuống...", "info")
            break
        case "properties":
            toast.show("Hiển thị thuộc tính: " + path, "info")
            break
        }
    }

    // ── Rename dialog ─────────────────────────────────────────────────
    Dialog {
        id: renameDialog
        title: "Đổi tên"
        modal: true
        anchors.centerIn: Overlay.overlay
        width: 360
        property string targetPath: ""
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            width: parent.width; spacing: 8
            Text {
                text: "Tên mới:"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                color: AppStyle.textSecondary
            }
            TextField {
                id: renameField; Layout.fillWidth: true
                font.family: AppStyle.fontFamily; onAccepted: renameDialog.accept()
            }
        }
        onOpened: { renameField.text = ""; renameField.forceActiveFocus() }
        onAccepted: {
            if (renameField.text.trim() !== "") {
                fileModel.renameFile(targetPath, renameField.text.trim())
                toast.show("Đã đổi tên thành \"" + renameField.text.trim() + "\"", "success")
            }
        }
    }

    // ── Progress overlay ──────────────────────────────────────────────
    ProgressOverlay {
        id: progressOverlay
        anchors.fill: parent
        onCancelRequested: {
            uploadSimTimer.stop()
            root._uploadProgress = 0
            hide()
            toast.show("Đã hủy thao tác", "error")
        }
    }

    // ── Log viewer ────────────────────────────────────────────────────
    LogViewer {
        id: logViewerDialog
    }

    // ── Toast notification ────────────────────────────────────────────
    ToastNotification {
        id: toast
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 24
        }
    }
}
