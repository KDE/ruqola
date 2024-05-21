/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ConfigureSoundComboBox;
class QToolButton;
class QAbstractItemModel;
class NotificationDesktopSoundPreferenceModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SoundConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SoundConfigureWidget() override;

    void setSoundModel(NotificationDesktopSoundPreferenceModel *model);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPlaySound();
    ConfigureSoundComboBox *const mConfigureSoundComboBox;
    QToolButton *const mPlaySoundToolButton;
    RocketChatAccount *const mRocketChatAccount;
    NotificationDesktopSoundPreferenceModel *mDesktopSoundModel = nullptr;
};
