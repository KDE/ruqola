/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
