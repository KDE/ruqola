/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class QPlainTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AnalyticsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit AnalyticsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AnalyticsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mMessages;
    QCheckBox *const mRooms;
    QCheckBox *const mUsers;
    QCheckBox *const mGoogleEnabled;
    QLineEdit *const mGoogleTrackingId;
    QCheckBox *const mPiwikEnabled;
    QLineEdit *const mPiwikUrl;
    QLineEdit *const mPiwikClientID;
    QPlainTextEdit *const mAdditionalPiwikSites;
    QCheckBox *const mPrependDomain;
    QCheckBox *const mAllSubdomains;
    QPlainTextEdit *const mHideOutgoingLinks;
};
