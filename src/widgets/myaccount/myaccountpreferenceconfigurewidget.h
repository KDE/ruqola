/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QComboBox;
class QCheckBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPreferenceConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountPreferenceConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountPreferenceConfigureWidget() override;
    void save();
    void load();

private:
    void setWasChanged();
    void initComboboxValues();
    QLineEdit *const mHighlightWords;
    QComboBox *const mDesktopNotification;
    QComboBox *const mEmailNotification;
    QComboBox *const mPushNotification;
    QComboBox *const mViewMode;
    QCheckBox *const mUseEmoji;
    QCheckBox *const mConvertAsciiEmoji;
    QCheckBox *const mHideRoles;
    QCheckBox *const mDisplayAvatars;
    RocketChatAccount *const mRocketChatAccount;
    bool mChanged = false;
};
