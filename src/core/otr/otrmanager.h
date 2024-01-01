/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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
    LIBRUQOLACORE_NO_EXPORT void slotAcceptOtr(const Otr &t);
    LIBRUQOLACORE_NO_EXPORT void slotRejectOtr(const Otr &t);
    LIBRUQOLACORE_NO_EXPORT void slotAcknowLedgeOtr(const Otr &t);
    LIBRUQOLACORE_NO_EXPORT void slotEndOtr(const Otr &t);
    LIBRUQOLACORE_NO_EXPORT void slotActivateNotificationAction();
    QList<Otr> mOtrList;
    RocketChatAccount *const mRocketChatAccount;
};
