/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "libruqola_private_export.h"
#include "otr.h"
#include <QObject>

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

private:
    void slotActivateNotificationAction(unsigned int val);
    void acceptOtr();
    void rejectOtr();
    Otr mOtr;
};
