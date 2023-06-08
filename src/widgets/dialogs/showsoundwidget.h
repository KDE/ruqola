/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QPushButton;
class QSlider;
class QToolButton;
class QLabel;
class QComboBox;
class QMediaDevices;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowSoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowSoundWidget(QWidget *parent = nullptr);
    ~ShowSoundWidget() override;

    void setSoundUrl(const QUrl &videoPath);
    Q_REQUIRED_RESULT QUrl soundUrl() const;

private:
    void play();
    void slotVolumeChanged(int position);
    void slotMuteChanged(bool state);
    void initializeAudioComboBox();
    void updateAudioDevices();
    void deviceChanged(int index);
    QPushButton *const mPlayButton;
    QSlider *const mPositionSlider;
    QLabel *const mErrorLabel;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
    QLabel *const mLabelDuration;
    QLabel *const mLabelPercentSound;
    QComboBox *const mDeviceComboBox;
    QMediaDevices *const mDevices;
};
