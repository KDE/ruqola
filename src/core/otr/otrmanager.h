/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "otr.h"
#include <QJsonArray>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT OtrManager : public QObject
{
    Q_OBJECT
public:
    explicit OtrManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~OtrManager() override;
    void parseOtr(const QJsonArray &contents);

private:
    Q_DISABLE_COPY(OtrManager)
    void slotAcceptOtr(const Otr &t);
    void slotRejectOtr(const Otr &t);
    void slotAcknowLedgeOtr(const Otr &t);
    void slotEndOtr(const Otr &t);
    void slotActivateNotificationAction();
    QList<Otr> mOtrList;
    RocketChatAccount *const mRocketChatAccount;
};
