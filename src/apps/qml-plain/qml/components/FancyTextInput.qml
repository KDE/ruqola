import QtQuick 2.9
import QtQuick.Controls 2.9

Flickable {
    id: root
    property alias font: textEdit.font
    property alias placeholderText: textEdit.placeholderText
    property alias color: textEdit.color
    property alias placeholderColor: placeholder.color
    property alias text: textEdit.text
    property alias _textEdit: textEdit

    contentWidth: width
    contentHeight: textEdit.height

    implicitHeight: textEdit.height

    clip: true
    function ensureVisible(r)
    {
        if (contentX >= r.x)
            contentX = r.x;
        else if (contentX+width <= r.x+r.width)
            contentX = r.x+r.width-width;
        if (contentY >= r.y)
            contentY = r.y;
        else if (contentY+height <= r.y+r.height)
            contentY = r.y+r.height-height;
    }

    Item {
        width: parent.width
        height: textEdit.height
        TextEdit {
            id: textEdit
            width: parent.width - 4
            // Add some left margin so we don't clip the text
            anchors.leftMargin: 2
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            property string placeholderText: ""
            font.pointSize: 10
            onCursorRectangleChanged: root.ensureVisible(cursorRectangle)
            selectionColor: "#3390FF"
            selectedTextColor: "#FFFFFF"
            selectByMouse: true

            Text {
                id: placeholder
                text: root.placeholderText
                font.pointSize: parent.font.pointSize
                visible: !parent.text
                z: -10
            }
        }
    }
}