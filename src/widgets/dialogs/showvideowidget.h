/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMediaPlayer>
#include <QWidget>
class QPushButton;
class QSlider;
class QLabel;
class QToolButton;
class KMessageWidget;
class QComboBox;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QAudioOutput;
#endif
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowVideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowVideoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowVideoWidget() override;
    void setVideoUrl(const QString &videoPath);
    [[nodiscard]] QUrl videoUrl() const;

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void mediaStateChanged(QMediaPlayer::State state);
#else
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
#endif
    void slotPositionChanged(qint64 position);
    void slotDurationChanged(qint64 duration);
    void updateDurationInfo(qint64 currentInfo);
    void slotMuteChanged(bool state);
    void slotVolumeChanged(int position);
    void setPosition(int position);
    void handleError();
    void play();
    void initializeAudioOutput();
    void audioOutputChanged(int index);
    void slotAttachmentFileDownloadDone(const QString &url);

    qint64 mDuration;
    QMediaPlayer *const mMediaPlayer;
    QPushButton *const mPlayButton;
    QSlider *const mPositionSlider;
    KMessageWidget *const mMessageWidget;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
    QLabel *const mLabelDuration;
    QLabel *const mLabelPercentSound;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QAudioOutput *const mAudioOutput;
    QComboBox *const mSoundDeviceComboBox;
#endif
    RocketChatAccount *const mRocketChatAccount;
};
