import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "styles"
import "."

ApplicationWindow {
    id: root
    visible: true
    width: 1200
    height: 750
    minimumWidth: 900
    minimumHeight: 600
    title: "FileManager v2.4.1"

    Material.theme: Material.Light
    Material.primary: "#4285F4"
    Material.accent:  "#1A73E8"

    // ── StackView: điều hướng giữa LicenseView ↔ MainView ────────────
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: licenseViewComponent

        pushEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 280; easing.type: Easing.OutCubic }
        }
        pushExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200; easing.type: Easing.InCubic }
        }
        popEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
        }
        popExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 280 }
        }
    }

    // ── License Info Dialog (hiển thị đồng thời khi vào MainView) ────
    LicenseInfoDialog {
        id: licenseInfoDialog
    }

    // ── Components ────────────────────────────────────────────────────
    Component {
        id: licenseViewComponent
        LicenseView {
            onActivationSucceeded: {
                stackView.replace(mainViewComponent)
                licenseInfoDialog.open()
            }
        }
    }

    Component {
        id: mainViewComponent
        MainView {
            onLogoutRequested: {
                licenseManager.deactivateLicense()
                stackView.replace(licenseViewComponent)
            }
        }
    }

    // ── Global keyboard shortcuts ─────────────────────────────────────
    Shortcut {
        sequence: "Ctrl+Q"
        onActivated: Qt.quit()
    }
}
