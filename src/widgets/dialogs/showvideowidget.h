/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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

    void setVideoPath(const QString &videoPath);

    [[nodiscard]] QUrl videoUrl() const;

Q_SIGNALS:
    void updateTitle(const QUrl &url);

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    LIBRUQOLAWIDGETS_NO_EXPORT void mediaStateChanged(QMediaPlayer::State state);
#else
    LIBRUQOLAWIDGETS_NO_EXPORT void mediaStateChanged(QMediaPlayer::PlaybackState state);
#endif
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPositionChanged(qint64 position);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDurationChanged(qint64 duration);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateDurationInfo(qint64 currentInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMuteChanged(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVolumeChanged(int position);
    LIBRUQOLAWIDGETS_NO_EXPORT void setPosition(int position);
    LIBRUQOLAWIDGETS_NO_EXPORT void handleError();
    LIBRUQOLAWIDGETS_NO_EXPORT void play();
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeAudioOutput();
    LIBRUQOLAWIDGETS_NO_EXPORT void audioOutputChanged(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAttachmentFileDownloadDone(const QString &url);

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
