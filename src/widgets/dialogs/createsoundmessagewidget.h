/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QWidget>
class QToolButton;
class QLabel;
class QTemporaryFile;
class QComboBox;
class KMessageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateSoundMessageWidget(QWidget *parent = nullptr);
    ~CreateSoundMessageWidget() override;

    [[nodiscard]] QUrl temporaryFilePath() const;

    void loadSettings();
    void saveSettings();

Q_SIGNALS:
    void recordDone();

private:
    void stop();
    void record();
    void pause();
    void updateRecorderState(QMediaRecorder::RecorderState state);
    void updateRecordTime(qint64 duration);
    void initializeInput();
    void displayRecorderError();
    QToolButton *const mRecordButton;
    QToolButton *const mPauseButton;
    QToolButton *const mStopButton;
    QLabel *const mLabelDuration;
    QMediaCaptureSession mCaptureSession;
    QMediaRecorder *mAudioRecorder = nullptr;
    QComboBox *const mDeviceComboBox;
    QTemporaryFile *mTemporaryFile = nullptr;
    KMessageWidget *const mMessageWidget;
};
