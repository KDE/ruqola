/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString desktopPushNotificationI18n(const QString &value) const;
    QLineEdit *const mHighlightWords;
    QComboBox *const mDesktopNotification;
    QComboBox *const mEmailNotification;
    QComboBox *const mPushNotification;
    QCheckBox *const mUseEmojis;
    QCheckBox *const mConvertAsciiEmoji;
    QCheckBox *const mHideRoles;
    QCheckBox *const mDisplayAvatars;
    QCheckBox *const mReceiveLoginDetectionEmails;
    QSpinBox *const mIdleTimeLimit;

    QCheckBox *const mAutomaticAway;
    QLabel *const mEmailNotificationLabel;

    RocketChatAccount *const mRocketChatAccount;
    bool mChanged = false;
};
