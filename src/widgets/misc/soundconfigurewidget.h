/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ConfigureSoundComboBox;
class QToolButton;
class NotificationDesktopSoundPreferenceModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SoundConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SoundConfigureWidget() override;

    void setSoundModel(NotificationDesktopSoundPreferenceModel *model, bool excludeDefaultNone = false);

    void setCurrentSound(const QByteArray &identifier);

    void updateButtonState();

    [[nodiscard]] QByteArray identifier() const;

Q_SIGNALS:
    void soundChanged(const QByteArray &identifier);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPlaySound();
    ConfigureSoundComboBox *const mConfigureSoundComboBox;
    QToolButton *const mPlaySoundToolButton;
    RocketChatAccount *const mRocketChatAccount;
    NotificationDesktopSoundPreferenceModel *mDesktopSoundModel = nullptr;
    bool mExcludeDefaultNone = false;
};
