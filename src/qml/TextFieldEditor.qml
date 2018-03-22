/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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


import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import org.kde.kirigami 2.1 as Kirigami

RowLayout {
    id: textFieldEditor
    signal updateValue(string newVal)
    property alias textField: channelNameField.text

    function setReadOnly(ro)
    {
        channelNameField.readOnly = ro;
        channelIcon.visible = !ro;
    }

    function clear()
    {
        channelNameField.clear()
    }

    QQC2.TextField {
        id: channelNameField
        selectByMouse: true
        onAccepted: {
            if (enabled) {
                textFieldEditor.updateValue(text)
            }
        }
        onFocusChanged: {
            if (enabled) {
                textFieldEditor.updateValue(channelNameField.text)
            }
        }
    }

    Kirigami.Icon {
        id: channelIcon
        source: "document-edit"
        width: 24
        height: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {
                //TODO change edit mode
                //TODO change icon
                //TODO change setEnable
                //Add escape for cancel it.
            }
        }
    }
}
