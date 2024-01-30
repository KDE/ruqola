/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QTemporaryFile>
#include <QWidget>
class QLabel;
class QCamera;
class QComboBox;
class QVideoWidget;
class QCameraDevice;
class QToolButton;
class KMessageWidget;
class QAudioInput;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateVideoMessageWidget(QWidget *parent = nullptr);
    ~CreateVideoMessageWidget() override;

    [[nodiscard]] QUrl temporaryFilePath() const;

    void loadSettings();
    void saveSettings();

Q_SIGNALS:
    void recordDone();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateCameras();
    LIBRUQOLAWIDGETS_NO_EXPORT void setCamera(const QCameraDevice &cameraDevice);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRecorderState(QMediaRecorder::RecorderState state);
    LIBRUQOLAWIDGETS_NO_EXPORT void record();
    LIBRUQOLAWIDGETS_NO_EXPORT void pause();
    LIBRUQOLAWIDGETS_NO_EXPORT void stop();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRecordTime();
    LIBRUQOLAWIDGETS_NO_EXPORT void displayRecorderError();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateCameraActive(bool active);
    LIBRUQOLAWIDGETS_NO_EXPORT void displayCameraError();

    QTemporaryFile *mTemporaryFile = nullptr;
    QVideoWidget *const mVideoWidget;
    QScopedPointer<QMediaRecorder> mMediaRecorder;
    QScopedPointer<QAudioInput> mAudioInput;
    QMediaCaptureSession mCaptureSession;
    QScopedPointer<QCamera> mCamera;
    QComboBox *const mListCamera;
    QToolButton *const mRecordButton;
    QToolButton *const mPauseButton;
    QToolButton *const mStopButton;
    QLabel *const mDurationLabel;
    KMessageWidget *const mMessageWidget;
};
