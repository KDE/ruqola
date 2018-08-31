import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2

import org.kde.kirigami 2.4 as Kirigami

QQC2.ScrollView {
    property QtObject personsModel : undefined

    contentHeight: authorsColumn.height
    contentWidth: 300

    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.topMargin: Kirigami.Units.smallSpacing
        spacing: Kirigami.Units.largeSpacing

        Column {
            id: authorsColumn
            spacing: Kirigami.Units.largeSpacing
            height: childrenRect.height

            Repeater {
                id: authorList
                model: personsModel

                Column {
                    id: authorItem
                    spacing: Kirigami.Units.smallSpacing
                    QQC2.Label {
                        text: "<b>" + model.username + "</b>"
                        wrapMode: QQC2.Label.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                        font.bold: true
                    }
                    QQC2.Label {
                        text: "<i>" + model.task + "<i>"
                        wrapMode: QQC2.Label.Wrap
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                    }
                    QQC2.Label {
                        text: model.email
                        wrapMode: QQC2.Label.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                        onLinkActivated: {
                            aboutDialog.openurl(link);
                        }
                    }
                }
            }
        }
    }
}
