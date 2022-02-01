/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthEditWidget : public QWidget
{
    Q_OBJECT
public:
    struct OauthEditInfo {
        QString applicationName;
        QString redirectUrl;
        bool active = false;
    };
    explicit AdministratorOauthEditWidget(QWidget *parent = nullptr);
    ~AdministratorOauthEditWidget() override;

    Q_REQUIRED_RESULT OauthEditInfo oauthInfo() const;
    void setOauthInfo(const OauthEditInfo &info);
Q_SIGNALS:
    void enableOkButton(bool enabled);
};
