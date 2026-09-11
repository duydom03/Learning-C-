import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: licenseRoot
    color: "#f8f9fa"

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: Math.min(540, parent.width - 40)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20

            // Google Drive Style Card Container
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: cardColumn.height + 48
                radius: 16
                color: "#ffffff"
                border.color: "#dadce0"
                border.width: 1

                ColumnLayout {
                    id: cardColumn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 24
                    spacing: 16

                    // App Logo Symbol
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 72
                        height: 72
                        radius: 36
                        color: "#e8f0fe"

                        Text {
                            anchors.centerIn: parent
                            text: "☁"
                            color: window.primaryColor
                            font.pixelSize: 36
                        }
                    }

                    // Title
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "FileManager Drive"
                        color: window.textLight
                        font.pixelSize: 24
                        font.bold: true
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Nhập Mã Product Key để đăng nhập hệ thống"
                        color: window.textMuted
                        font.pixelSize: 13
                    }

                    // Machine ID Box (Fixed: Auto wrap & Copy button)
                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        radius: 8
                        color: "#f1f3f4"
                        border.color: "#dadce0"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 8

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                Text {
                                    text: "💻 Machine ID (Mã Máy):"
                                    color: window.textMuted
                                    font.pixelSize: 11
                                    font.bold: true
                                }

                                Text {
                                    id: machineIdText
                                    text: licenseInfo ? licenseInfo.machineId : ""
                                    color: window.primaryColor
                                    font.pixelSize: 11
                                    font.family: "Consolas"
                                    font.bold: true
                                    Layout.fillWidth: true
                                    wrapMode: Text.WrapAnywhere
                                    maximumLineCount: 2
                                    elide: Text.ElideMiddle
                                }
                            }

                            Button {
                                text: "📋 Copy"
                                implicitWidth: 64
                                implicitHeight: 28
                                onClicked: appCore.copyToClipboard(machineIdText.text)
                            }
                        }
                    }

                    // Product Key Input Field
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        Text {
                            text: "Product Key License:"
                            color: window.textLight
                            font.pixelSize: 12
                            font.bold: true
                        }

                        TextField {
                            id: keyInput
                            Layout.fillWidth: true
                            height: 44
                            placeholderText: "Ví dụ: ADFE-3JCS-AKS1-HN24"
                            text: "ADFE-3JCS-AKS1-HN24"
                            color: window.textLight
                            placeholderTextColor: window.textMuted
                            font.pixelSize: 14
                            font.family: "Consolas"

                            background: Rectangle {
                                color: "#f1f3f4"
                                radius: 8
                                border.color: keyInput.activeFocus ? window.primaryColor : "#dadce0"
                                border.width: keyInput.activeFocus ? 2 : 1
                            }
                        }
                    }

                    // Activate Button
                    Button {
                        id: activateBtn
                        Layout.fillWidth: true
                        height: 44
                        text: "KÍCH HOẠT VÀ ĐĂNG NHẬP"
                        enabled: keyInput.text.length > 0

                        contentItem: Text {
                            text: activateBtn.text
                            color: "#ffffff"
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: activateBtn.pressed ? "#1557b0" : (activateBtn.hovered ? "#185abc" : window.primaryColor)
                            radius: 8
                            opacity: activateBtn.enabled ? 1.0 : 0.4
                        }

                        onClicked: {
                            appCore.activateLicense(keyInput.text.trim())
                        }
                    }

                    // Status Message Label
                    Text {
                        id: statusLabel
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        text: licenseInfo ? licenseInfo.statusMessage : ""
                        color: (licenseInfo && licenseInfo.isValid) ? window.accentGreen : window.accentRed
                        font.pixelSize: 13
                        font.bold: true
                        visible: text.length > 0
                    }

                    // Quick test keys helper box
                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 110
                        radius: 8
                        color: "#f8f9fa"
                        border.color: "#e0e0e0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 6

                            Text {
                                text: "🔑 Mẫu Product Key có sẵn trên CSDL Server:"
                                color: "#e37400"
                                font.pixelSize: 11
                                font.bold: true
                            }

                            RowLayout {
                                spacing: 8

                                Button {
                                    text: "ADFE-3JCS-AKS1-HN24"
                                    implicitHeight: 28
                                    onClicked: keyInput.text = "ADFE-3JCS-AKS1-HN24"
                                }
                                Button {
                                    text: "PRO-USER-2026-KEY1"
                                    implicitHeight: 28
                                    onClicked: keyInput.text = "PRO-USER-2026-KEY1"
                                }
                                Button {
                                    text: "TRIAL-8888-9999-7DAY"
                                    implicitHeight: 28
                                    onClicked: keyInput.text = "TRIAL-8888-9999-7DAY"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
