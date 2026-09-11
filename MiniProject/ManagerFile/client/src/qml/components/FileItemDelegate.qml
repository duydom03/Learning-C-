import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: delegateRoot
    radius: 12
    color: mouseArea.containsPress ? "#d2e3fc" : (isItemSelected ? "#e8f0fe" : (mouseArea.containsMouse ? "#f1f3f4" : "#ffffff"))
    border.color: isItemSelected ? window.primaryColor : (mouseArea.containsMouse ? "#dadce0" : "#e0e0e0")
    border.width: isItemSelected ? 2 : 1

    property string viewMode: "grid" // "grid" or "list"
    property bool isItemSelected: contentAreaRoot.selectedFileName === model.fileName

    signal itemClicked(string path, string name)
    signal itemDoubleClicked(string path, bool isDir)
    signal itemRightClicked(string path, string name)

    ToolTip.visible: mouseArea.containsMouse
    ToolTip.text: model.fileName + "\nKích thước: " + model.fileSize + "\nSửa đổi: " + model.fileModified

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: function(mouse) {
            if (mouse.button === Qt.RightButton) {
                delegateRoot.itemRightClicked(model.filePath, model.fileName)
            } else {
                delegateRoot.itemClicked(model.filePath, model.fileName)
            }
        }

        onDoubleClicked: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                delegateRoot.itemDoubleClicked(model.filePath, model.isFolder)
            }
        }
    }

    // Grid View Layout
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6
        visible: delegateRoot.viewMode === "grid"

        Image {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 54
            Layout.preferredHeight: 54
            source: "image://fileicon/" + (model.fileIcon !== undefined ? model.fileIcon : "file-generic")
            fillMode: Image.PreserveAspectFit
            cache: false
        }

        Text {
            text: model.fileName !== undefined ? model.fileName : ""
            color: window.textLight
            font.pixelSize: 12
            font.bold: model.isFolder
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
            elide: Text.ElideMiddle
            maximumLineCount: 2
            wrapMode: Text.WrapAnywhere
        }

        Text {
            text: model.isFolder ? "Folder" : model.fileSize
            color: window.textMuted
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }

    // List View Layout
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 12
        visible: delegateRoot.viewMode === "list"

        Image {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            source: "image://fileicon/" + (model.fileIcon !== undefined ? model.fileIcon : "file-generic")
            fillMode: Image.PreserveAspectFit
            cache: false
        }

        Text {
            text: model.fileName !== undefined ? model.fileName : ""
            color: window.textLight
            font.pixelSize: 13
            font.bold: model.isFolder
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        Text {
            text: model.fileSize !== undefined ? model.fileSize : "--"
            color: window.textMuted
            font.pixelSize: 11
            Layout.preferredWidth: 80
            horizontalAlignment: Text.AlignRight
        }

        Text {
            text: model.fileModified !== undefined ? model.fileModified : ""
            color: window.textMuted
            font.pixelSize: 11
            Layout.preferredWidth: 120
            horizontalAlignment: Text.AlignRight
        }
    }
}
