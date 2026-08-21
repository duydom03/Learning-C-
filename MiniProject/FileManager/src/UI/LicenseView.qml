import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "styles"

// ─────────────────────────────────────────────────────────────────────────────
// LicenseView.qml — Màn hình kích hoạt license (trang đầu tiên)
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root

    signal activationSucceeded()

    // Kết nối tín hiệu từ C++ LicenseManager
    Connections {
        target: licenseManager

        function onActivationSucceeded() {
            statusLabel.color   = AppStyle.successColor
            statusLabel.text    = "✓ Kích hoạt thành công! Đang khởi động..."
            activateBtn.enabled = false
            successTimer.start()
        }

        function onActivationFailed(reason) {
            statusLabel.color = AppStyle.errorColor
            statusLabel.text  = "✕ " + reason
            shakeAnimation.start()
        }
    }

    Timer {
        id: successTimer
        interval: 900
        onTriggered: root.activationSucceeded()
    }

    // ── Background gradient ───────────────────────────────────────────
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            orientation: Gradient.Vertical
            GradientStop { position: 0.0; color: "#F8F9FA" }
            GradientStop { position: 1.0; color: "#E8F0FE" }
        }
    }

    // ── Center card ───────────────────────────────────────────────────
    Rectangle {
        id: card
        anchors.centerIn: parent
        width:  460
        height: cardLayout.implicitHeight + 64
        radius: AppStyle.radiusXL
        color:  AppStyle.cardBackground

        layer.enabled: true
        layer.effect: null   // используйте DropShadow из Qt.labs.graphicaleffects при наличии

        border.color: AppStyle.borderColor
        border.width: 1

        // Shake animation on error
        SequentialAnimation {
            id: shakeAnimation
            loops: 1
            PropertyAnimation { target: card; property: "x"; to: card.x - 8; duration: 50 }
            PropertyAnimation { target: card; property: "x"; to: card.x + 8; duration: 50 }
            PropertyAnimation { target: card; property: "x"; to: card.x - 8; duration: 50 }
            PropertyAnimation { target: card; property: "x"; to: card.x + 8; duration: 50 }
            PropertyAnimation { target: card; property: "x"; to: card.x;     duration: 50 }
        }

        ColumnLayout {
            id: cardLayout
            anchors {
                top: parent.top; left: parent.left; right: parent.right
                topMargin: 32; leftMargin: 32; rightMargin: 32
            }
            spacing: 20

            // ── App Logo ─────────────────────────────────────────────
            Item {
                Layout.alignment: Qt.AlignHCenter
                width: 64; height: 64

                Rectangle {
                    anchors.fill: parent
                    radius: 14
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: AppStyle.primaryColor }
                        GradientStop { position: 1; color: AppStyle.primaryDark  }
                    }
                }
                Text {
                    anchors.centerIn: parent
                    text: "FM"
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: 22
                    font.weight:    Font.Bold
                    color:          "#FFFFFF"
                }
            }

            // ── Title ─────────────────────────────────────────────────
            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 4

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text:           "FileManager"
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeXXL
                    font.weight:    Font.Medium
                    color:          AppStyle.textPrimary
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text:           "Nhập Product Key để kích hoạt ứng dụng"
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    color:          AppStyle.textSecondary
                }
            }

            // ── Demo hint box ────────────────────────────────────────
            Rectangle {
                Layout.fillWidth: true
                height: hintLayout.implicitHeight + 16
                radius: AppStyle.radiusSM
                color:  AppStyle.primaryFaint

                RowLayout {
                    id: hintLayout
                    anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; margins: 12 }
                    spacing: 8

                    Text {
                        text:           "ℹ"
                        font.pixelSize: 14
                        color:          AppStyle.primaryColor
                    }
                    Text {
                        Layout.fillWidth: true
                        text: "Demo key: <b>FM-2024-DEMO-KEY</b>"
                        font.family:    AppStyle.fontFamily
                        font.pixelSize: AppStyle.fontSizeSM
                        color:          AppStyle.primaryDark
                        textFormat:     Text.RichText
                        wrapMode:       Text.WordWrap
                    }
                }
            }

            // ── Product Key field ─────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text:           "Product Key"
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    font.weight:    Font.Medium
                    color:          AppStyle.textPrimary
                }

                Rectangle {
                    id: fieldBg
                    Layout.fillWidth: true
                    height: 46
                    radius: AppStyle.radiusMD
                    color: AppStyle.backgroundColor
                    border.color: keyField.activeFocus
                        ? AppStyle.primaryColor
                        : (statusLabel.color === AppStyle.errorColor ? AppStyle.errorColor : AppStyle.borderColor)
                    border.width: keyField.activeFocus ? 2 : 1

                    Behavior on border.color { ColorAnimation { duration: AppStyle.durationFast } }

                    RowLayout {
                        anchors { fill: parent; leftMargin: 14; rightMargin: 8 }
                        spacing: 0

                        TextField {
                            id: keyField
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            placeholderText: "Nhập Product Key của bạn..."
                            echoMode: showKeyBtn.checked
                                      ? TextInput.Normal
                                      : TextInput.Password
                            font.family:    AppStyle.fontFamilyMono
                            font.pixelSize: AppStyle.fontSizeMD
                            color:          AppStyle.textPrimary
                            background: Item {}   // remove default background

                            Keys.onReturnPressed: activateBtn.activate()
                            Keys.onEnterPressed:  activateBtn.activate()

                            onTextChanged: {
                                statusLabel.text  = ""
                                statusLabel.color = AppStyle.textSecondary
                            }
                        }

                        // Show/hide toggle
                        RoundButton {
                            id: showKeyBtn
                            checkable: true
                            flat: true
                            icon.source: showKeyBtn.checked
                                         ? "qrc:/resources/icons/eye-off.svg"
                                         : "qrc:/resources/icons/eye.svg"
                            icon.color: AppStyle.textSecondary
                            icon.width: 18; icon.height: 18
                            width: 32; height: 32
                            padding: 0
                        }
                    }
                }

                // Status message
                Text {
                    id: statusLabel
                    Layout.fillWidth: true
                    text:           ""
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeSM
                    color:          AppStyle.textSecondary
                    wrapMode:       Text.WordWrap
                    visible:        text !== ""

                    Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }
                }
            }

            // ── Activate button ───────────────────────────────────────
            Rectangle {
                id: activateBtn
                Layout.fillWidth: true
                height: 46
                radius: AppStyle.radiusMD

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0; color: enabled ? AppStyle.primaryColor : "#BDBDBD" }
                    GradientStop { position: 1; color: enabled ? AppStyle.primaryDark  : "#9E9E9E" }
                }

                scale: activateMa.pressed ? 0.97 : 1.0
                Behavior on scale { NumberAnimation { duration: AppStyle.durationFast } }

                function activate() {
                    if (!enabled) return
                    licenseManager.activateLicense(keyField.text)
                }

                Text {
                    anchors.centerIn: parent
                    text:           "Kích hoạt"
                    font.family:    AppStyle.fontFamily
                    font.pixelSize: AppStyle.fontSizeMD
                    font.weight:    Font.Medium
                    color:          "#FFFFFF"
                }

                MouseArea {
                    id: activateMa
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: parent.activate()
                }

                Behavior on opacity { NumberAnimation { duration: AppStyle.durationFast } }
            }

            // ── Footer ────────────────────────────────────────────────
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 4
                text:           "FileManager v2.4.1 · © 2024 FM Corp. All rights reserved."
                font.family:    AppStyle.fontFamily
                font.pixelSize: AppStyle.fontSizeXS
                color:          AppStyle.textDisabled
            }
        }
    }
}
