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
import QtMultimedia 5.8
import KDE.Ruqola.RocketChatAccount 1.0
Dialog {
    id: takeVideoMessageDialog

    standardButtons: Dialog.Ok | Dialog.Cancel

    property QtObject rcAccount

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: 300
    height: 500

    modal: true
    property bool isCameraAvailable: QtMultimedia.availableCameras.length > 0

    Camera {
        id: camera
        videoRecorder.audioEncodingMode: CameraRecorder.ConstantBitrateEncoding;
        videoRecorder.audioBitRate: 128000
        videoRecorder.mediaContainer: "mp4"
        videoRecorder.outputLocation: rcAccount.recordingVideoPath()
    }

    //TODO align vertical center
    Label {
        visible: isCameraAvailable == false
        text: i18n("No camera found.");
        font.bold: true
        font.pointSize: 20
        anchors.fill: parent
        anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
    }

    VideoOutput {
        id: camareLiveOutput
        source: camera
        anchors.fill: parent
        autoOrientation: true
        focus: visible
        visible: isCameraAvailable && (camera.cameraStatus === Camera.ActiveStatus)
    }
    Button {
        text: i18n("Video");
        visible: isCameraAvailable
        onPressed: {
            if (camera.cameraStatus === camera.StartingStatus)
                camera.stop()
            else
                camera.start()
        }
    }

    Component.onCompleted: {
        camera.stop()
    }
    //TODO add info when we can't have camera ok
    onClosed: {
        camera.stop()
    }
}
