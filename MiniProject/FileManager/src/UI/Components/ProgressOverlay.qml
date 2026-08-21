import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// ProgressOverlay.qml — Overlay hiển thị tiến trình tác vụ dài
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root
    visible: false

    property string message:  "Đang xử lý..."
    property int    progress: 0

    signal cancelRequested()

    function show(msg, pct) {
        message  = msg
        progress = pct
        visible  = true
    }

    function update(pct) {
        progress = pct
        message  = overlayMessage + " (" + pct + "%)"
    }

    property string overlayMessage: "Đang xử lý..."

    function hide() {
        visible = false
    }

    // ── Dim background ────────────────────────────────────────────────
    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.3
    }

    // ── Blur backdrop ─────────────────────────────────────────────────
    // (Qt.labs.graphicaleffects FastBlur can be added here)

    // ── Center card ───────────────────────────────────────────────────
    Rectangle {
        id: card
        anchors.centerIn: parent
        width: 320; height: cardCol.implicitHeight + 48
        radius: AppStyle.radiusXL
        color: AppStyle.cardBackground
        border.color: AppStyle.borderColor; border.width: 1

        // Enter animation
        scale: 0.92; opacity: 0
        Behavior on scale   { NumberAnimation { duration: 240; easing.type: Easing.OutBack } }
        Behavior on opacity { NumberAnimation { duration: 200 } }

        Component.onCompleted: { scale = 1; opacity = 1 }

        ColumnLayout {
            id: cardCol
            anchors { left: parent.left; right: parent.right; top: parent.top; margins: 28 }
            spacing: 16

            // Circular progress indicator
            Item {
                Layout.alignment: Qt.AlignHCenter
                width: 72; height: 72

                // Track
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        const ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.strokeStyle = AppStyle.borderColor
                        ctx.lineWidth   = 5
                        ctx.beginPath()
                        ctx.arc(width/2, height/2, 30, 0, Math.PI * 2)
                        ctx.stroke()
                    }
                }

                // Progress arc (rotated)
                Canvas {
                    id: progressArc
                    anchors.fill: parent
                    property real progressRatio: root.progress / 100

                    onPaint: {
                        const ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.strokeStyle = AppStyle.primaryColor
                        ctx.lineWidth   = 5
                        ctx.lineCap     = "round"
                        ctx.beginPath()
                        const start = -Math.PI / 2
                        const end   = start + (Math.PI * 2 * progressRatio)
                        ctx.arc(width/2, height/2, 30, start, end)
                        ctx.stroke()
                    }

                    onProgressRatioChanged: requestPaint()
                    Behavior on progressRatio { NumberAnimation { duration: 300 } }
                }

                // Percentage label
                Text {
                    anchors.centerIn: parent
                    text: root.progress + "%"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    font.weight: Font.Medium; color: AppStyle.primaryColor
                }
            }

            // Message
            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 4

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: root.message
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    font.weight: Font.Medium; color: AppStyle.textPrimary
                    horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap
                    Layout.maximumWidth: 240
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Vui lòng không đóng ứng dụng..."
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeXS
                    color: AppStyle.textDisabled
                }
            }

            // Progress bar
            Rectangle {
                Layout.fillWidth: true; height: 6; radius: 3; color: AppStyle.borderColor

                Rectangle {
                    id: progressBar
                    width: parent.width * (root.progress / 100)
                    height: parent.height; radius: parent.radius
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: AppStyle.primaryColor }
                        GradientStop { position: 1; color: AppStyle.primaryDark }
                    }
                    Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.OutCubic } }
                }
            }

            // Cancel button
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 4
                height: 36; radius: AppStyle.radiusFull
                width: cancelLabel.implicitWidth + 28
                color: cancelMa.containsMouse ? AppStyle.errorFaint : "transparent"
                border.color: AppStyle.errorColor; border.width: 1

                Behavior on color { ColorAnimation { duration: AppStyle.durationFast } }

                Text {
                    id: cancelLabel; anchors.centerIn: parent
                    text: "Hủy"
                    font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
                    font.weight: Font.Medium; color: AppStyle.errorColor
                }
                MouseArea {
                    id: cancelMa; anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: root.cancelRequested()
                }
            }
        }
    }
}
