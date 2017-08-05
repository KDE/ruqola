/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.0
import org.kde.kirigami 2.1 
import QtQuick.Controls 2.2 as QQC

import QtQuick.Layouts 1.1

ScrollablePage {
    id: channelsList
    
    implicitHeight: 300
    header: Column {
        Heading {
            id: heading
            text: qsTr("Channels list")
            level: 1
        }
        QQC.TextField {
            id: searchField
            Behavior on opacity { NumberAnimation{} }
            visible: opacity ? true : false
            placeholderText: qsTr("Search...")
            width: parent.width
            onTextChanged: {
                if (!ignoreTextChange)
                    searchTextChanged(text)
            }
        }
    }
    
    mainItem: ListView {
        model:  ListModel {
            id: testModel
            ListElement { name: "channel1"; type: "c" }
            ListElement { name: "channel2"; type: "c" }
            ListElement { name: "channel3"; type: "c" }
            ListElement { name: "channel4"; type: "c" }
            ListElement { name: "channel5"; type: "c" }
            ListElement { name: "channel6"; type: "c" }
            ListElement { name: "channel7"; type: "c" }
            ListElement { name: "john"; type: "d" }
            ListElement { name: "mario"; type: "d" }
            ListElement { name: "pasquale"; type: "d" }
            ListElement { name: "valeria"; type: "d" }
        }
        delegate: BasicListItem {
            visible: RegExp(searchField.text,"i").test(name) // HACK -> use a proper filter model
            //             height: visible ? : 0
            label: name
            icon: type == "c" ? "irc-channel-active" : "user-avaliable"
            
        }
        
    }

}
