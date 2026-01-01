/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CasSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit CasSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CasSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QLineEdit *const mSSOBaseURL;
    QLineEdit *const mSSOLoginURL;
    QComboBox *const mCASVersion;
    QCheckBox *const mTrustCASUsername;
    QCheckBox *const mAllowUserCreation;
    QCheckBox *const mAlwaysSyncUserData;
    QLineEdit *const mAttributeMap;
};
