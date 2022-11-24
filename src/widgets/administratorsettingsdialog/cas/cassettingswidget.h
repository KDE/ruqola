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
class LIBRUQOLAWIDGETS_TESTS_EXPORT CasSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit CasSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CasSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QCheckBox *const mTrustCASUsername;
    QCheckBox *const mAllowUserCreation;
    QLineEdit *const mSSOBaseURL;
    QLineEdit *const mSSOLoginURL;
    QComboBox *const mCASVersion;
    QCheckBox *const mAlwaysSyncUserData;
};
