/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaDisableTotpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2FaDisableTotpWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaDisableTotpWidget() override;

Q_SIGNALS:
    void hide2FaDisableTotpWidget();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVerify();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegenerateCode();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTotpInvalid(bool check);
    RocketChatAccount *const mRocketChatAccount;
    QLineEdit *const mDisableCodeLineEdit;
};
