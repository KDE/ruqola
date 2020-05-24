import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

import "../components/chatmessage"

Item {
    id: root

    // see MessageModel
    property var _model: null

    property bool showDate: model.dateDiffersFromPrevious

    property real contentPadding: 0
    property bool compact: _settings.compactMode

    implicitHeight: layout.height

    signal reactionDialogRequested(real x, real y)
    signal reactionButtonClicked(string emojiId)
    signal menuButtonClicked(real x, real y)

    ColumnLayout {
        id: layout
        width: parent.width
        height: daySeparatorLoader.height + messageDelegateLoader.height
        spacing: 0

        Loader {
            id: daySeparatorLoader
            active: root.showDate

            Layout.fillWidth: true
            Layout.preferredHeight: root.showDate ? 25 : 0

            sourceComponent: DaySeparator {
                id: daySeparator

                text: root._model.date
                padding: root.contentPadding
            }
        }

        Loader {
            id: messageDelegateLoader

            Layout.fillWidth: true

            Component {
                id: normalMessage

                NormalMessage {
                    _model: root._model
                    padding: root.contentPadding
                }
            }

            Component {
                id: systemMessage

                SystemMessage {
                    _model: root._model
                    padding: root.contentPadding
                }
            }

            Component {
                id: normalMessageCompact

                NormalMessageCompact{
                    _model: root._model
                    padding: root.contentPadding
                }
            }

            Component {
                id: systemMessageCompact

                SystemMessageCompact{
                    _model: root._model
                    padding: root.contentPadding
                }
            }

            sourceComponent: {
                if (_settings.compactMode) {
                    return (root._model.messagetype === 0) ? systemMessageCompact : normalMessageCompact;
                }
                else {
                    return (root._model.messagetype === 0) ? systemMessage : normalMessage;
                }
            }
        }
    }
}