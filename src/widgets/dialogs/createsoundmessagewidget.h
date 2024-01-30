/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void stop();
    LIBRUQOLAWIDGETS_NO_EXPORT void record();
    LIBRUQOLAWIDGETS_NO_EXPORT void pause();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRecorderState(QMediaRecorder::RecorderState state);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRecordTime(qint64 duration);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeInput();
    LIBRUQOLAWIDGETS_NO_EXPORT void displayRecorderError();
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
