/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT VideoConferenceWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit VideoConferenceWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~VideoConferenceWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mBigBlueEnabled;
    QLineEdit *const mServer;
    QLineEdit *const mSharedSecret;
    QCheckBox *const mAlwaysOpenNewWindow;

    QCheckBox *const mJitsiEnabled;
    QLineEdit *const mJitsiDomain;
    QLineEdit *const mJitsiRoomPrefix;
    QLineEdit *const mJitsiRoomSuffix;
    QCheckBox *const mJitsiEnableRoomNameHash;
    QCheckBox *const mJitsiUseSSL;
    QCheckBox *const mJitsiAlwaysOpenNewWindow;
    QCheckBox *const mJitsiEnableInChannels;
    QCheckBox *const mJitsiEnableTeams;
    QLineEdit *const mJitsiChromeExtensionId;
    QCheckBox *const mJitsiEnableJWTAuth;
    QLineEdit *const mJitsiApplicationID;
    QLineEdit *const mJitsiApplicationSecret;
    QCheckBox *const mJitsiLimitTokenJitsiRoom;
};
