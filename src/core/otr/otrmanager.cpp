/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrmanager.h"
#include "ddpapi/ddpclient.h"
#include "otrnotificationjob.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

OtrManager::OtrManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

OtrManager::~OtrManager() = default;

void OtrManager::slotActivateNotificationAction()
{
}

void OtrManager::parseOtr(const QJsonArray &contents)
{
    Otr t;
    t.parseOtr(contents);
    auto job = new OtrNotificationJob(this);
    connect(job, &OtrNotificationJob::acceptOtr, this, &OtrManager::slotAcceptOtr);
    connect(job, &OtrNotificationJob::rejectOtr, this, &OtrManager::slotRejectOtr);
    connect(job, &OtrNotificationJob::acknowLedgeOtr, this, &OtrManager::slotAcknowLedgeOtr);
    connect(job, &OtrNotificationJob::endOtr, this, &OtrManager::slotEndOtr);

    job->setRocketChatAccount(mRocketChatAccount);
    job->setOtr(t);
    job->start();
}

void OtrManager::slotAcceptOtr(const Otr &t)
{
    // We need to answer with own userId
    mRocketChatAccount->ddp()->streamNotifyUserOtrAcknowledge(t.roomId(), mRocketChatAccount->userId(), t.crypto().mCrypt);
}

void OtrManager::slotRejectOtr(const Otr &t)
{
    // TODO add reject method.
    qCDebug(RUQOLA_LOG) << " Otr rejected!";
}

void OtrManager::slotAcknowLedgeOtr(const Otr &t)
{
    // TODO add slotAcknowLedgeOtr method.
    mOtrList.append(t);
    qCDebug(RUQOLA_LOG) << "AcknowLedge Otr!";
}

void OtrManager::slotEndOtr(const Otr &t)
{
    mOtrList.removeAll(t);
}

#include "moc_otrmanager.cpp"
