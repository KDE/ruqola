/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
namespace Prison
{
class AbstractBarcode;
}
class RocketChatAccount;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaTotpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2FaTotpWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaTotpWidget() override;

private:
    void slotTotpResult(const QString &secret, const QString &url);
    void slotVerify();
    Prison::AbstractBarcode *const mQRCode;
    QLabel *const mTotpQrCode;
    QLabel *const mQrCoreLabel;
    QLineEdit *const mVerifyQrCode;
    RocketChatAccount *const mRocketChatAccount;
};
