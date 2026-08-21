import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "styles"

// ─────────────────────────────────────────────────────────────────────────────
// LicenseInfoDialog.qml
// Hiển thị đồng thời khi vào MainView: thông tin license vừa kích hoạt,
// thời hạn còn lại, và cảnh báo nếu sắp hết hạn.
// ─────────────────────────────────────────────────────────────────────────────
Dialog {
    id: root

    title: ""
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    width:  520
    height: dialogContent.implicitHeight + 32

    anchors.centerIn: Overlay.overlay

    // Không dùng header mặc định của Dialog
    header: Item { height: 0 }

    padding: 0

    background: Rectangle {
        radius: AppStyle.radiusXL
        color:  AppStyle.cardBackground
        border.color: AppStyle.borderColor
        border.width: 1
    }

    // ── Tính toán thông tin từ licenseManager ─────────────────────────
    readonly property int  remaining:   licenseManager.remainingDays
    readonly property bool expiringSoon: remaining >= 0 && remaining <= 30
    readonly property bool expired:      remaining < 0

    readonly property color statusColor: expired
        ? AppStyle.errorColor
        : (expiringSoon ? AppStyle.warningColor : AppStyle.successColor)

    readonly property string statusText: expired
        ? "Đã hết hạn"
        : (expiringSoon
           ? "Sắp hết hạn (" + remaining + " ngày)"
           : "Đang hoạt động")

    readonly property string statusIcon: expired ? "✕" : (expiringSoon ? "⚠" : "✓")

    // ── Enter animation ───────────────────────────────────────────────
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 280; easing.type: Easing.OutCubic }
        NumberAnimation { property: "scale";   from: 0.92; to: 1; duration: 280; easing.type: Easing.OutCubic }
    }
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 180 }
    }

    contentItem: ColumnLayout {
        id: dialogContent
        spacing: 0

        // ── Header strip ─────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: 80
            radius: AppStyle.radiusXL

            // Only round top corners
            Rectangle {
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                height: parent.radius
                color:  parent.color
            }

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0; color: AppStyle.primaryColor }
                GradientStop { position: 1; color: AppStyle.primaryDark  }
            }

            RowLayout {
                anchors { fill: parent; leftMargin: 24; rightMargin: 16 }
                spacing: 14

                // FM logo mark
                Rectangle {
                    width: 44; height: 44
                    radius: 10
                    color: Qt.rgba(1,1,1,0.2)

                    Text {
                        anchors.centerIn: parent
                        text:           "FM"
                        font.pixelSize: 16
                        font.weight:    Font.Bold
                        color:          "#FFFFFF"
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Text {
                        text:           "Thông tin License"
                        font.family:    AppStyle.fontFamily
                        font.pixelSize: AppStyle.fontSizeLG
                        font.weight:    Font.Medium
                        color:          "#FFFFFF"
                    }
                    Text {
                        text:           licenseManager.edition
                        font.family:    AppStyle.fontFamily
                        font.pixelSize: AppStyle.fontSizeSM
                        color:          Qt.rgba(1,1,1,0.8)
                    }
                }

                // Close button
                RoundButton {
                    flat: true
                    width: 32; height: 32
                    padding: 0
                    text: "✕"
                    font.pixelSize: 14
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: Qt.rgba(1,1,1,0.8)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: parent.radius
                        color: parent.hovered ? Qt.rgba(1,1,1,0.15) : "transparent"
                    }
                    onClicked: root.close()
                }
            }
        }

        // ── Status badge ─────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 20
            Layout.leftMargin: 24; Layout.rightMargin: 24
            height: 48
            radius: AppStyle.radiusMD
            color: Qt.rgba(root.statusColor.r, root.statusColor.g, root.statusColor.b, 0.1)
            border.color: Qt.rgba(root.statusColor.r, root.statusColor.g, root.statusColor.b, 0.3)
            border.width: 1

            RowLayout {
                anchors { fill: parent; leftMargin: 16; rightMargin: 16 }
                spacing: 10

                Rectangle {
                    width: 28; height: 28
                    radius: 14
                    color: Qt.rgba(root.statusColor.r, root.statusColor.g, root.statusColor.b, 0.15)

                    Text {
                        anchors.centerIn: parent
                        text:           root.statusIcon
                        font.pixelSize: 14
                        color:          root.statusColor
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: root.expired
                          ? "License này đã hết hạn. Vui lòng gia hạn để tiếp tục sử dụng."
                          : (root.expiringSoon
                             ? "License sắp hết hạn! Còn " + root.remaining + " ngày. Vui lòng gia hạn sớm."
                             : "License đang hoạt động bình thường. Còn " + root.remaining + " ngày sử dụng.")
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    color:          root.statusColor
                    wrapMode:       Text.WordWrap
                }
            }
        }

        // ── Info rows ─────────────────────────────────────────────────
        ColumnLayout {
            Layout.fillWidth: true
            Layout.topMargin: 16
            Layout.leftMargin: 24; Layout.rightMargin: 24
            spacing: 0

            // Helper: one info row
            component InfoRow: RowLayout {
                property string label: ""
                property string value: ""
                property bool   mono:  false
                property color  valueColor: AppStyle.textPrimary

                Layout.fillWidth: true
                height: 40
                spacing: 0

                // Divider top
                Rectangle {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    height: 1
                    color: AppStyle.dividerColor
                    visible: index > 0
                }

                Text {
                    Layout.preferredWidth: 150
                    Layout.fillHeight: true
                    text:           label
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    color:          AppStyle.textSecondary
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text:           value
                    font.family:    mono ? AppStyle.fontFamilyMono : AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    font.weight:    Font.Medium
                    color:          valueColor
                    verticalAlignment: Text.AlignVCenter
                    elide:          Text.ElideRight
                }
            }

            // Divider top of table
            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150
                    Layout.fillHeight: true
                    text: "Khách hàng"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Text {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    text: licenseManager.customer
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                    color: AppStyle.textPrimary; verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Email"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Text {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    text: licenseManager.email
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                    color: AppStyle.primaryColor; verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Product Key"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Text {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    text: licenseManager.productKey
                    font.family: AppStyle.fontFamilyMono; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                    color: AppStyle.textPrimary; verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Gói dịch vụ"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    height: 22; radius: AppStyle.radiusFull
                    width: editionLabel.implicitWidth + 16
                    color: AppStyle.primaryFaint

                    Text {
                        id: editionLabel
                        anchors.centerIn: parent
                        text: licenseManager.edition
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                        color: AppStyle.primaryDark
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Ngày kích hoạt"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Text {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    text: licenseManager.activationDate
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                    color: AppStyle.textPrimary; verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            RowLayout {
                Layout.fillWidth: true
                height: 40
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Ngày hết hạn"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                Text {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    text: licenseManager.expirationDate.toLocaleDateString(Qt.locale("vi_VN"), "dd/MM/yyyy")
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM; font.weight: Font.Medium
                    color: root.expired ? AppStyle.errorColor : AppStyle.textPrimary
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }

            // Remaining days — highlight row
            RowLayout {
                Layout.fillWidth: true
                height: 48
                Text {
                    Layout.preferredWidth: 150; Layout.fillHeight: true
                    text: "Thời hạn còn lại"; font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    color: AppStyle.textSecondary; verticalAlignment: Text.AlignVCenter
                }
                RowLayout {
                    spacing: 8
                    Text {
                        text: root.expired ? "Đã hết hạn" : (root.remaining + " ngày")
                        font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeLG; font.weight: Font.Bold
                        color: root.statusColor
                    }
                    // Mini progress bar
                    Rectangle {
                        width: 100; height: 6; radius: 3
                        color: AppStyle.borderColor
                        visible: !root.expired

                        Rectangle {
                            width: Math.min(100, Math.max(0, root.remaining / 365 * 100))
                            height: parent.height; radius: parent.radius
                            color: root.statusColor
                            Behavior on width { NumberAnimation { duration: 600; easing.type: Easing.OutCubic } }
                        }
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: AppStyle.dividerColor }
        }

        // ── Footer buttons ────────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 16
            Layout.bottomMargin: 24
            Layout.leftMargin: 24; Layout.rightMargin: 24
            spacing: 10

            // Gia hạn button (placeholder)
            Rectangle {
                height: 38
                width: renewLabel.implicitWidth + 24
                radius: AppStyle.radiusMD
                color: AppStyle.backgroundColor
                border.color: AppStyle.borderColor
                border.width: 1

                Text {
                    id: renewLabel
                    anchors.centerIn: parent
                    text: "Gia hạn License"
                    font.family: AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    font.weight: Font.Medium
                    color: AppStyle.textSecondary
                }
                MouseArea {
                    anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: { /* emit signal to C++ */ }
                    onEntered: parent.color = AppStyle.hoverColor
                    onExited:  parent.color = AppStyle.backgroundColor
                }
            }

            Item { Layout.fillWidth: true }

            // Đóng button
            Rectangle {
                height: 38
                width: closeLabel.implicitWidth + 28
                radius: AppStyle.radiusMD
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0; color: AppStyle.primaryColor }
                    GradientStop { position: 1; color: AppStyle.primaryDark  }
                }

                Text {
                    id: closeLabel
                    anchors.centerIn: parent
                    text: "Đóng"
                    font.family: AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    font.weight: Font.Medium
                    color: "#FFFFFF"
                }
                MouseArea {
                    anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: root.close()
                }
            }
        }
    }
}
