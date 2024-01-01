/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthEditWidget : public QWidget
{
    Q_OBJECT
public:
    struct OauthEditInfo {
        QString applicationName;
        QString redirectUrl;
        QString clientId;
        QString clientSecret;
        QString authorizationUrl;
        QString accessTokenUrl;
        bool active = false;
        [[nodiscard]] bool isValid() const;
    };
    explicit AdministratorOauthEditWidget(QWidget *parent = nullptr);
    ~AdministratorOauthEditWidget() override;

    [[nodiscard]] OauthEditInfo oauthInfo() const;
    void setOauthInfo(const OauthEditInfo &info);
Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    void slotTextChanged();
    QCheckBox *const mActiveCheckBox;
    QLineEdit *const mApplicationName;
    QLineEdit *const mRedirectUrl;
    QLineEdit *const mClientId;
    QLineEdit *const mClientSecret;
    QLineEdit *const mAuthorizationUrl;
    QLineEdit *const mAccessTokenUrl;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorOauthEditWidget::OauthEditInfo &t);
