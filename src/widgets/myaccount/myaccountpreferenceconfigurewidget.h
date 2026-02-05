/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QComboBox;
class QCheckBox;
class RocketChatAccount;
class QSpinBox;
class QLabel;
class NotificationDesktopSoundPreferenceModel;
class SoundConfigureWidget;
class QVBoxLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPreferenceConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountPreferenceConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountPreferenceConfigureWidget() override;
    void save();
    void load();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserRequestDataDownloadDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void setWasChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void initComboboxValues();
    LIBRUQOLAWIDGETS_NO_EXPORT void downloadData(bool fullData);
    LIBRUQOLAWIDGETS_NO_EXPORT void createLayout(QLabel *label, QWidget *widget, QVBoxLayout *layout);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString desktopPushNotificationI18n(const QString &value) const;
    QLineEdit *const mHighlightWords;
    QComboBox *const mDesktopNotification;
    QComboBox *const mEmailNotification;
    QComboBox *const mPushNotification;
    QCheckBox *const mReceiveLoginDetectionEmails;
    QCheckBox *const mUseEmojis;
    QCheckBox *const mConvertAsciiEmoji;
    QCheckBox *const mHideRoles;
    QCheckBox *const mDisplayAvatars;
    QCheckBox *const mAutomaticAway;
    QSpinBox *const mIdleTimeLimit;

    QLabel *const mEmailNotificationLabel;
    SoundConfigureWidget *const mSoundNewRoomNotification;
    SoundConfigureWidget *const mSoundNewMessageNotification;
    NotificationDesktopSoundPreferenceModel *const mSoundModel;
    QCheckBox *const mMuteFocusedConversations;
    QSpinBox *const mNotificationsSoundVolume;
    QSpinBox *const mCallRingerVolume;
    QSpinBox *const mMasterVolume;
    RocketChatAccount *const mRocketChatAccount;
    bool mChanged = false;
};
