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
class QAudioOutput;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void play();
    LIBRUQOLAWIDGETS_NO_EXPORT void muteChanged(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void setPosition(int position);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPositionChanged(qint64 progress);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDurationChanged(qint64 duration);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateDurationInfo(qint64 currentInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVolumeChanged(int position);
    LIBRUQOLAWIDGETS_NO_EXPORT void handleError();
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeAudioOutput();
    LIBRUQOLAWIDGETS_NO_EXPORT void audioOutputChanged(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void mediaStateChanged(QMediaPlayer::PlaybackState state);
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
    QAudioOutput *const mAudioOutput;
    QComboBox *const mDeviceComboBox;
    RocketChatAccount *const mRocketChatAccount;
};
