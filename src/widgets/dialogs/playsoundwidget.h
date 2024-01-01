/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMediaPlayer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QPushButton;
class QToolButton;
class QSlider;
class QLabel;
class QComboBox;
class KMessageWidget;
class RocketChatAccount;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QAudioOutput;
#endif
class LIBRUQOLAWIDGETS_TESTS_EXPORT PlaySoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySoundWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PlaySoundWidget() override;

    void setAudioPath(const QString &url);
    [[nodiscard]] QUrl audioUrl() const;

Q_SIGNALS:
    void updateTitle(const QUrl &url);

private:
    void play();
    void muteChanged(bool state);
    void setPosition(int position);
    void slotPositionChanged(qint64 progress);
    void slotDurationChanged(qint64 duration);
    void updateDurationInfo(qint64 currentInfo);
    void slotVolumeChanged(int position);
    void handleError();
    void initializeAudioOutput();
    void audioOutputChanged(int index);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void mediaStateChanged(QMediaPlayer::State state);
#else
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
#endif
    void slotAttachmentFileDownloadDone(const QString &url);
    qint64 mDuration;
    QMediaPlayer *const mMediaPlayer;
    QPushButton *const mPlayButton;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
    QSlider *const mPositionSlider;
    QLabel *const mLabelDuration;
    KMessageWidget *const mMessageWidget;
    QLabel *const mLabelPercentSound;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QAudioOutput *const mAudioOutput;
    QComboBox *const mDeviceComboBox;
#endif
    RocketChatAccount *const mRocketChatAccount;
};
