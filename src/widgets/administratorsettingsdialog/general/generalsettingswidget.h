/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT GeneralSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~GeneralSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

    void loadSettings();

private:
    QCheckBox *const mEnableFavoriteRooms;
    QLineEdit *const mSiteUrl;
    QLineEdit *const mSiteName;
    QLineEdit *const mUTF8UsernamesValidation;
    QLineEdit *const mUTF8ChannelNamesValidation;
    QLineEdit *const mFirstChannelAfterLogin;
    QLineEdit *const mDeepLinkUrl;
    QLineEdit *const mCDNPrefix;
    QComboBox *const mUnreadCount;
    QComboBox *const mUnreadCountDirectMessages;
};
