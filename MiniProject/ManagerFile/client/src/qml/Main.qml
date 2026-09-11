import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1100
    height: 700
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: qsTr("FileManager - Google Drive Style Manager")

    // Theme palette - Google Drive Light Theme
    property color bgDark: "#f8f9fa"
    property color cardBg: "#ffffff"
    property color primaryColor: "#1a73e8"
    property color textLight: "#202124"
    property color textMuted: "#5f6368"
    property color accentGreen: "#188038"
    property color accentRed: "#d93025"
    property color borderLight: "#dadce0"

    background: Rectangle {
        color: window.bgDark
    }

    // Toast Notification Banner
    Rectangle {
        id: toastBanner
        z: 999
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 16
        width: Math.min(450, parent.width - 32)
        height: 44
        radius: 8
        color: isError ? "#d32f2f" : "#2e7d32"
        visible: opacity > 0
        opacity: 0

        property bool isError: false
        property alias text: toastText.text

        Behavior on opacity { NumberAnimation { duration: 250 } }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            Text {
                id: toastText
                color: "#ffffff"
                font.pixelSize: 13
                font.bold: true
                Layout.fillWidth: true
                elide: Text.ElideRight
            }

            Text {
                text: "✕"
                color: "#ffffff"
                font.pixelSize: 14
                MouseArea {
                    anchors.fill: parent
                    onClicked: toastBanner.opacity = 0
                }
            }
        }

        Timer {
            id: toastTimer
            interval: 3500
            onTriggered: toastBanner.opacity = 0
        }
    }

    Connections {
        target: appCore
        function onShowToast(message, isError) {
            toastBanner.text = message
            toastBanner.isError = isError
            toastBanner.opacity = 1
            toastTimer.restart()
        }
    }

    // StackView for switching between LicenseView and MainView
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: licenseViewComponent

        pushEnter: Transition { NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } }
        pushExit:  Transition { NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } }
        popEnter:  Transition { NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } }
        popExit:   Transition { NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } }
    }

    Component {
        id: licenseViewComponent
        LicenseView {}
    }

    Component {
        id: mainViewComponent
        MainView {}
    }

    // Automatically transition to MainView when license is verified
    Connections {
        target: licenseInfo
        function onChanged() {
            if (licenseInfo.isValid) {
                if (stackView.currentItem !== null && stackView.depth === 1) {
                    stackView.push(mainViewComponent)
                }
            }
        }
    }
}
