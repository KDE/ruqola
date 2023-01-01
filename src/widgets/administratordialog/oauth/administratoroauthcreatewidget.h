/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthCreateWidget : public QWidget
{
    Q_OBJECT
public:
    struct OauthCreateInfo {
        QString applicationName;
        QString redirectUrl;
        bool active = false;
    };

    explicit AdministratorOauthCreateWidget(QWidget *parent = nullptr);
    ~AdministratorOauthCreateWidget() override;

    Q_REQUIRED_RESULT OauthCreateInfo oauthInfo() const;
    void setOauthInfo(const OauthCreateInfo &info);
Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    void slotTextChanged();
    QCheckBox *const mActiveCheckBox;
    QLineEdit *const mApplicationName;
    QLineEdit *const mRedirectUrl;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorOauthCreateWidget::OauthCreateInfo &t);
