import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../styles"

// ─────────────────────────────────────────────────────────────────────────────
// ToastNotification.qml — Thông báo nhỏ xuất hiện ở dưới màn hình
// ─────────────────────────────────────────────────────────────────────────────
Item {
    id: root
    width: toastRow.implicitWidth + 28
    height: 44
    visible: false
    opacity: 0

    function show(message, type) {
        toastMessage.text = message
        switch (type) {
        case "success": iconText.text = "✓"; toastBg.border.color = AppStyle.successColor; iconBg.color = AppStyle.successFaint; iconText.color = AppStyle.successColor; break
        case "error":   iconText.text = "✕"; toastBg.border.color = AppStyle.errorColor;   iconBg.color = AppStyle.errorFaint;   iconText.color = AppStyle.errorColor;   break
        default:        iconText.text = "ℹ"; toastBg.border.color = AppStyle.primaryColor;  iconBg.color = AppStyle.primaryFaint; iconText.color = AppStyle.primaryColor;  break
        }
        visible = true
        showAnim.start()
        hideTimer.restart()
    }

    Timer {
        id: hideTimer
        interval: 3000
        onTriggered: hideAnim.start()
    }

    SequentialAnimation {
        id: showAnim
        NumberAnimation { target: root; property: "opacity"; from: 0; to: 1; duration: 200; easing.type: Easing.OutCubic }
        NumberAnimation { target: root; property: "y"; from: root.y + 12; to: root.y; duration: 200; easing.type: Easing.OutCubic }
    }

    SequentialAnimation {
        id: hideAnim
        NumberAnimation { target: root; property: "opacity"; from: 1; to: 0; duration: 200 }
        ScriptAction { script: root.visible = false }
    }

    Rectangle {
        id: toastBg
        anchors.fill: parent
        radius: AppStyle.radiusFull
        color: AppStyle.cardBackground
        border.color: AppStyle.primaryColor; border.width: 1

        layer.enabled: true
    }

    RowLayout {
        id: toastRow
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; leftMargin: 12; rightMargin: 16 }
        spacing: 10

        Rectangle {
            id: iconBg
            width: 24; height: 24; radius: 12
            color: AppStyle.primaryFaint
            Text {
                id: iconText; anchors.centerIn: parent
                font.pixelSize: 12; font.weight: Font.Bold; color: AppStyle.primaryColor
            }
        }

        Text {
            id: toastMessage
            font.family: AppStyle.fontFamily; font.pixelSize: AppStyle.fontSizeSM
            color: AppStyle.textPrimary
        }
    }
}
