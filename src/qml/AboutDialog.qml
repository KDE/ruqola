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
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import KDE.Ruqola.RuqolaAboutDataAuthorModel 1.0
import KDE.Ruqola.RuqolaAboutData 1.0

import org.kde.kirigami 2.1 as Kirigami
Dialog {
    id: aboutDialog

    title: i18n("About Ruqola")

    standardButtons: Dialog.Close

    signal openurl(string link)

    property QtObject applicationData

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: 400
    height: 600
    modal: true
    TabBar {
        id: bar
        width: parent.width

        TabButton {
            text: i18n("About")
        }
        TabButton {
            text: i18n("Libraries")
        }
        TabButton {
            text: i18n("Author")
        }
        TabButton {
            text: i18n("Thanks To")
            visible: applicationData.creditsModel.rowCount() > 0
        }
        TabButton {
            text: i18n("Translation")
            visible: applicationData.translatorModel.rowCount() > 0
        }
    }

    StackLayout {
        anchors.top: bar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        currentIndex: bar.currentIndex

        Rectangle {
            id: aboutTab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    text: applicationData.title
                    wrapMode: Text.WordWrap
                    textFormat: Qt.RichText
                }
                Text {
                    text: applicationData.about
                    wrapMode: Text.WordWrap
                    textFormat: Qt.RichText
                }
                Text {
                    text: applicationData.licenses
                    wrapMode: Text.WordWrap
                    textFormat: Qt.RichText
                    onLinkActivated: {
                        licenseDialog.licenseText = applicationData.licenseText(link);
                        licenseDialog.open();
                    }
                }
            }
        }
        Item {
            id: librariesTab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing

                Text {
                    text: applicationData.libraries
                    wrapMode: Text.WordWrap
                    textFormat: Qt.RichText
                }
            }
        }
        Item {
            id: authorTab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing

                Text {
                    visible: applicationData.reportBugs !== ""
                    text: applicationData.reportBugs
                    wrapMode: Text.WordWrap
                    textFormat: Qt.RichText
                    onLinkActivated: {
                        aboutDialog.openurl(link);
                    }
                }
                ScrollView {
                    id: view
                    width: authorTab.width
                    height: authorTab.height

                    Column {
                        Repeater {
                            id: authorList

                            model: applicationData.authorModel
                            Column {
                                spacing: 5
                                Text {
                                    text: username
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                    font.bold: true
                                }
                                Text {
                                    text: task
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                }
                                Text {
                                    text: email
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
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
        }
        Item {
            id: thanksToTab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing

                ScrollView {
                    id: thanksToTabview
                    width: thanksToTab.width
                    height: thanksToTab.height

                    Column {
                        Repeater {
                            id: creditList

                            model: applicationData.creditsModel
                            Column {
                                Text {
                                    text: username
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                    font.bold: true
                                }
                                Text {
                                    text: task
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                }
                                Text {
                                    text: email
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
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
        }
        Item {
            id: translatorToTab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing

                ScrollView {
                    id: translatorToTabView
                    width: translatorToTab.width
                    height: translatorToTab.height

                    Column {
                        Repeater {
                            id: translatorList

                            model: applicationData.translatorModel
                            Column {
                                Text {
                                    text: username
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                    font.bold: true
                                }
                                Text {
                                    text: task
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                }
                                Text {
                                    text: email
                                    wrapMode: Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
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
        }
    }

    LicenseDialog {
        id: licenseDialog
    }
}
