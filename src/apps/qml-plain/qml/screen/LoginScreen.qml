import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

import Ruqola 1.0

Rectangle {
    id: "root"

    color: "#2f343d"

    // ColumnLayout {
    //     anchors.fill: parent

    StackLayout {
        currentIndex: _loginScreenController.loginMode
        width: 350
        height: 200
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            // Layout.preferredWidth: 350
            // Layout.preferredHeight: 200
            // Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            color: "white"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 40

                TextInput {
                    id: serverUrlText
                    color: "#000000"
                    font.pointSize: 16
                    Layout.fillWidth: true
                    property string placeholderText: "https://open.rocket.chat"

                    Text {
                        text: serverUrlText.placeholderText
                        color: "#757575"
                        font.pointSize: serverUrlText.font.pointSize
                        visible: !serverUrlText.text
                    }
                }

                Button {
                    text: "Connect"
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#1d74f5"
                    }
                    Layout.fillWidth: true
                    onClicked: _loginScreenController.addServer(serverUrlText.text)
                }
            }
        }

        Rectangle {
            // Layout.preferredWidth: 350
            // Layout.preferredHeight: 200
            // Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            color: "white"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 40

                TextInput {
                    id: usernameText
                    color: "#000000"
                    Layout.fillWidth: true
                    font.pointSize: 14
                    property string placeholderText: "Email or username"

                    Text {
                        text: usernameText.placeholderText
                        color: "#ced1d4"
                        font.pointSize: usernameText.font.pointSize
                        visible: !usernameText.text
                    }
                }

                TextInput {
                    id: passwordText
                    text: ""
                    color: "#000000"

                    Layout.fillWidth: true
                    font.pointSize: 14
                    // TODO: unsafe!
                    echoMode: text !== "" ?  TextInput.Password : TextInput.Normal

                    property string placeholderText: "Password"

                    Text {
                        text: passwordText.placeholderText
                        color: "#ced1d4"
                        font.pointSize: passwordText.font.pointSize
                        visible: !passwordText.text
                    }
                }

                Button {
                    text: "Login"
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#1d74f5"
                    }
                    Layout.fillWidth: true
                    onClicked: _loginScreenController.authenticate(usernameText.text, passwordText.text)
                }
            }
        }
    }
}
