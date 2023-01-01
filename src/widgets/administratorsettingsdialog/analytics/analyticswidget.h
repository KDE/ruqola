/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mMessages;
    QCheckBox *const mRooms;
    QCheckBox *const mUsers;
    QCheckBox *const mGoogleEnabled;
    QCheckBox *const mPiwikEnabled;
    QLineEdit *const mGoogleTrackingId;
    QLineEdit *const mPiwikUrl;
    QLineEdit *const mPiwikClientID;
    QCheckBox *const mPrependDomain;
    QCheckBox *const mAllSubdomains;
    QPlainTextEdit *const mAdditionalPiwikSites;
    QPlainTextEdit *const mHideOutgoingLinks;
};
