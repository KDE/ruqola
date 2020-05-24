import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: root

    property var _model: null
    property int padding: 0

    signal reactionDialogRequested(real x, real y)
    signal reactionButtonClicked(string emojiId)
    signal menuButtonClicked(real x, real y)
}
