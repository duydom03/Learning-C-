import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "components"

Rectangle {
    id: mainViewRoot
    color: window.bgDark

    // License Popup Dialog showing User Name, Machine ID, Remaining Duration & Logout
    Dialog {
        id: licensePopup
        anchors.centerIn: parent
        modal: true
        width: 480
        height: 360
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        header: Rectangle {
            height: 48
            color: "#f8f9fa"
            radius: 16
            border.color: "#dadce0"
            border.width: 1

            // Cover bottom radius of header rectangle
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 16
                color: "#f8f9fa"
            }

            Text {
                anchors.centerIn: parent
                text: "Thông tin Bản quyền Ứng dụng"
                color: window.textLight
                font.pixelSize: 15
                font.bold: true
            }
        }

        background: Rectangle {
            color: "#ffffff"
            radius: 16
            border.color: "#dadce0"
            border.width: 1
        }

        contentItem: ColumnLayout {
            spacing: 14
            anchors.margins: 8

            Rectangle {
                Layout.fillWidth: true
                height: 32
                color: "#e6f4ea"
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: "✓ LICENSE ĐÃ ĐƯỢC XÁC THỰC THÀNH CÔNG"
                    color: window.accentGreen
                    font.pixelSize: 13
                    font.bold: true
                }
            }

            Grid {
                columns: 2
                spacing: 12
                Layout.fillWidth: true

                Text { text: "👤 Người sử dụng:"; color: window.textMuted; font.pixelSize: 12 }
                Text { text: licenseInfo ? licenseInfo.customer : ""; color: window.textLight; font.bold: true; font.pixelSize: 13 }

                Text { text: "💻 Mã máy (Machine ID):"; color: window.textMuted; font.pixelSize: 12; Layout.alignment: Qt.AlignVCenter }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 260
                    spacing: 6

                    Rectangle {
                        Layout.fillWidth: true
                        height: 32
                        color: "#f1f3f4"
                        radius: 6
                        clip: true

                        Text {
                            id: popupMidText
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 8
                            verticalAlignment: Text.AlignVCenter
                            text: licenseInfo ? licenseInfo.machineId : ""
                            color: window.primaryColor
                            font.family: "Consolas"
                            font.pixelSize: 11
                            font.bold: true
                            elide: Text.ElideMiddle
                        }
                    }

                    Button {
                        text: "📋 Copy"
                        implicitHeight: 32
                        implicitWidth: 60
                        onClicked: appCore.copyToClipboard(popupMidText.text)
                        contentItem: Text {
                            text: "📋 Copy"
                            color: window.textLight
                            font.pixelSize: 11
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.hovered ? "#e8eaed" : "#f1f3f4"
                            radius: 6
                            border.color: "#dadce0"
                        }
                    }
                }

                Text { text: "🛡️ Gói bản quyền:"; color: window.textMuted; font.pixelSize: 12 }
                Text { text: licenseInfo ? licenseInfo.plan : ""; color: "#e37400"; font.bold: true; font.pixelSize: 12 }

                Text { text: "⏳ Thời hạn còn lại:"; color: window.textMuted; font.pixelSize: 12 }
                Text { text: (licenseInfo ? licenseInfo.remainingDays : 0) + " ngày"; color: window.accentGreen; font.bold: true; font.pixelSize: 14 }

                Text { text: "📅 Ngày hết hạn:"; color: window.textMuted; font.pixelSize: 12 }
                Text { text: licenseInfo ? licenseInfo.expirationDate : ""; color: window.textLight; font.pixelSize: 12 }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#e0e0e0"
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10

                // Logout Button
                Button {
                    text: "🚪 Đăng xuất tài khoản"
                    implicitWidth: 160
                    implicitHeight: 36
                    onClicked: {
                        licensePopup.close()
                        appCore.logout()
                    }
                    contentItem: Text {
                        text: "🚪 Đăng xuất tài khoản"
                        color: "#d93025"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? "#fce8e6" : "#ffffff"
                        radius: 8
                        border.color: "#f5c6cb"
                    }
                }

                // Close Button
                Button {
                    text: "Đóng"
                    implicitWidth: 90
                    implicitHeight: 36
                    onClicked: licensePopup.close()
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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Row 1: Toolbar (fixed height 56px)
        Toolbar {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            onOpenLicensePopup: licensePopup.open()
        }

        // Row 2: Horizontal SplitView (NavigationPane & ContentArea)
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            handle: Rectangle {
                implicitWidth: 4
                color: SplitHandle.pressed ? window.primaryColor : "#e0e0e0"
            }

            // Left: NavigationPane (min width 200)
            NavigationPane {
                SplitView.minimumWidth: 200
                SplitView.preferredWidth: 240
                SplitView.maximumWidth: 400
                Layout.fillHeight: true
            }

            // Right: ContentArea (remaining width)
            ContentArea {
                SplitView.fillWidth: true
                Layout.fillHeight: true
            }
        }

        // Row 3: ThreadMonitor (collapsible, default height 120px)
        Rectangle {
            id: threadMonitorContainer
            Layout.fillWidth: true
            Layout.preferredHeight: isExpanded ? 140 : 36
            color: "#ffffff"
            border.color: "#e0e0e0"
            border.width: 1

            property bool isExpanded: true

            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // Header Bar
                Rectangle {
                    Layout.fillWidth: true
                    height: 36
                    color: "#f8f9fa"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12

                        Text {
                            text: "⚙ THÀNH PHẦN THEO DÕI ĐA LUỒNG (THREAD MONITOR)"
                            color: window.textLight
                            font.pixelSize: 12
                            font.bold: true
                        }

                        Rectangle {
                            height: 18
                            width: 24
                            radius: 9
                            color: threadModel && threadModel.activeCount > 0 ? window.primaryColor : "#dadce0"
                            Text {
                                anchors.centerIn: parent
                                text: threadModel ? threadModel.activeCount : 0
                                color: "#ffffff"
                                font.pixelSize: 10
                                font.bold: true
                            }
                        }

                        Item { Layout.fillWidth: true }

                        Text {
                            text: threadMonitorContainer.isExpanded ? "▼ Thu gọn" : "▲ Mở rộng"
                            color: window.textMuted
                            font.pixelSize: 11
                            MouseArea {
                                anchors.fill: parent
                                onClicked: threadMonitorContainer.isExpanded = !threadMonitorContainer.isExpanded
                            }
                        }
                    }
                }

                // ListView of active threads
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: threadMonitorContainer.isExpanded
                    clip: true
                    model: threadModel

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 36
                        color: index % 2 === 0 ? "#ffffff" : "#f8f9fa"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12
                            spacing: 12

                            Text {
                                text: model.threadName
                                color: window.textLight
                                font.pixelSize: 11
                                font.bold: true
                                Layout.preferredWidth: 160
                                elide: Text.ElideRight
                            }

                            Text {
                                text: "Priority: " + model.priority
                                color: window.textMuted
                                font.pixelSize: 10
                                Layout.preferredWidth: 90
                            }

                            Text {
                                text: "CPU: " + model.cpuUsage.toFixed(1) + "%"
                                color: window.accentGreen
                                font.pixelSize: 10
                                Layout.preferredWidth: 70
                            }

                            // Progress Bar
                            ProgressBar {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 12
                                from: 0
                                to: 100
                                value: model.progress

                                background: Rectangle {
                                    color: "#e8eaed"
                                    radius: 6
                                }
                                contentItem: Item {
                                    Rectangle {
                                        width: parent.width * (parent.parent.value / 100)
                                        height: parent.height
                                        radius: 6
                                        color: model.status === "Completed" ? window.accentGreen : window.primaryColor
                                    }
                                }
                            }

                            Text {
                                text: model.progress + "%"
                                color: window.textLight
                                font.pixelSize: 11
                                font.bold: true
                                Layout.preferredWidth: 40
                            }

                            Button {
                                text: "Hủy"
                                implicitWidth: 45
                                implicitHeight: 22
                                visible: model.status === "Running"
                                onClicked: appCore.cancelUpload(model.taskId)
                            }
                        }
                    }
                }
            }
        }
    }

    // Stacked ProgressOverlay for long heavy tasks
    ProgressOverlay {
        anchors.fill: parent
        visible: appCore ? appCore.isBusy : false
        message: appCore ? appCore.busyMessage : ""
        progress: appCore ? appCore.busyProgress : 0
        onCancelRequested: {
            appCore.setBusy(false)
        }
    }
}
