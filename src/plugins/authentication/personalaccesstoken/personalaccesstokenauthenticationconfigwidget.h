/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "personalaccesstokenpluginutil.h"
#include <QWidget>
class QLineEdit;
class PersonalAccessTokenAuthenticationConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigWidget() override;

    [[nodiscard]] PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo info() const;

Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    void slotEnableOkButton();
    QLineEdit *const mServerNameLineEdit;
    QLineEdit *const mAccountNameLineEdit;
    QLineEdit *const mUserLineEdit;
    QLineEdit *const mPersonalAccessTokenLineEdit;
};
