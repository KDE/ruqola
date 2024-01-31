/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <Prison/Barcode>
#include <QWidget>
class QLabel;
namespace Prison
{
class Barcode;
}
class RocketChatAccount;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaTotpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2FaTotpWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaTotpWidget() override;

Q_SIGNALS:
    void show2FaEnabledWidget();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTotpResult(const QString &secret, const QString &url);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVerify();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTotpInvalid();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTotpValid(const QStringList &listCodes);
    std::optional<Prison::Barcode> mQRCode;
    QLabel *const mTotpQrCode;
    QLabel *const mQrCoreLabel;
    QLineEdit *const mVerifyQrCode;
    RocketChatAccount *const mRocketChatAccount;
};
