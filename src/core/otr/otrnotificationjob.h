/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "libruqola_private_export.h"
#include "otr.h"
#include <QObject>
#include <QPointer>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT OtrNotificationJob : public QObject
{
    Q_OBJECT
public:
    explicit OtrNotificationJob(QObject *parent = nullptr);
    ~OtrNotificationJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void start();
    Q_REQUIRED_RESULT const Otr &otr() const;
    void setOtr(const Otr &newOtr);

    void setRocketChatAccount(RocketChatAccount *account);

Q_SIGNALS:
    void acceptOtr(const Otr &t);
    void rejectOtr(const Otr &t);
    void acknowLedgeOtr(const Otr &t);
    void endOtr(const Otr &t);

private:
    void slotActivateNotificationAction(unsigned int val);
    void slotAcceptOtr();
    void slotRejectOtr();
    Q_REQUIRED_RESULT QString generateText();
    Otr mOtr;
    QPointer<RocketChatAccount> mRocketChatAccount;
};
