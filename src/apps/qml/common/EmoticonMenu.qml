/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.0
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Layouts 1.12
QQC2.Popup {
    id: emojiPopup
    property var emojiPopupModel
    property string categoryName
    signal insertEmoticon(string emoti)

    ColumnLayout {
        anchors.fill: parent

        GridView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellWidth: Kirigami.Units.gridUnit * 2.5
            cellHeight: Kirigami.Units.gridUnit * 2.5

            boundsBehavior: Flickable.DragOverBounds

            clip: true

            model: emojiPopupModel

            delegate: QQC2.ItemDelegate {
                width: Kirigami.Units.gridUnit * 2
                height: Kirigami.Units.gridUnit * 2

                contentItem: Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    font.pointSize: 20
                    font.family: "NotoColorEmoji"
                    text: unicodeEmoji
                }

                hoverEnabled: true
                QQC2.ToolTip.text: identifier
                QQC2.ToolTip.visible: hovered

                onClicked: {
                    emojiPopup.insertEmoticon(identifier)
                }
            }
            QQC2.ScrollBar.vertical: QQC2.ScrollBar {}
        }
        //Row {
            GridView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                cellWidth: Kirigami.Units.gridUnit * 2.5
                cellHeight: Kirigami.Units.gridUnit * 2.5

                boundsBehavior: Flickable.DragOverBounds

                clip: true
                model: emojiPopupModel.emoticonCategoriesModel()

                delegate: QQC2.ItemDelegate {
                    width: Kirigami.Units.gridUnit * 2
                    height: Kirigami.Units.gridUnit * 2

                    contentItem: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        font.pointSize: 20
                        font.family: "NotoColorEmoji"
                        text: name
                    }

                    //hoverEnabled: true
                    //QQC2.ToolTip.text: category
                    //QQC2.ToolTip.visible: hovered

                    onClicked: {
                        emojiPopupModel.setCurrentCategory(category)
                    }
                }
            }
        }
   // }
}
