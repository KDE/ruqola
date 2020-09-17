/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "administratorroomswidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "restapirequest.h"
#include "rooms/adminroomsjob.h"
#include "ruqolawidgets_debug.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QJsonObject>

AdministratorRoomsWidget::AdministratorRoomsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    initialize();
}

AdministratorRoomsWidget::~AdministratorRoomsWidget()
{
}

void AdministratorRoomsWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    RocketChatRestApi::AdminRoomsJob *adminRoomsJob = new RocketChatRestApi::AdminRoomsJob(this);
    rcAccount->restApi()->initializeRestApiJob(adminRoomsJob);
    connect(adminRoomsJob, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone,
            this, &AdministratorRoomsWidget::slotAdminRoomDone);
    if (!adminRoomsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob";
    }
}

void AdministratorRoomsWidget::slotAdminRoomDone(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
}
