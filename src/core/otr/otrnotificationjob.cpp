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

#include "otrnotificationjob.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

#include <KLocalizedString>
#include <KNotification>

OtrNotificationJob::OtrNotificationJob(QObject *parent)
    : QObject{parent}
{
}

OtrNotificationJob::~OtrNotificationJob()
{
}

void OtrNotificationJob::setRocketChatAccount(RocketChatAccount *account)
{
    mRocketChatAccount = account;
}

bool OtrNotificationJob::canStart() const
{
    return mOtr.isValid();
}

void OtrNotificationJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start OtrNotificationJob";
        deleteLater();
        return;
    }
    if (mOtr.isValid()) {
        switch (mOtr.type()) {
        case Otr::OtrType::Unknown:
            qCWarning(RUQOLA_LOG) << "It's a bug we can't have otrtype == Unknown";
            deleteLater();
            break;
        case Otr::OtrType::End: {
            auto notification = new KNotification(QStringLiteral("Otr-end"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setIconName(QStringLiteral("network-connect"));
            notification->setText(generateText());
            notification->sendEvent();
            deleteLater();
            break;
        }
        case Otr::OtrType::Handshake: {
            auto notification = new KNotification(QStringLiteral("Otr-handshake"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setIconName(QStringLiteral("network-connect"));
            notification->setText(generateText());
            const QStringList lstActions{i18n("Reject"), i18n("Ok")};
            notification->setActions(lstActions);

            connect(notification, qOverload<unsigned int>(&KNotification::activated), this, &OtrNotificationJob::slotActivateNotificationAction);
            connect(notification, &KNotification::closed, this, &OtrNotificationJob::deleteLater);
            notification->sendEvent();
            break;
        }
        case Otr::OtrType::Deny: {
            auto notification = new KNotification(QStringLiteral("Otr-deny"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setIconName(QStringLiteral("network-connect"));
            notification->setText(generateText());
            notification->sendEvent();
            deleteLater();
            break;
        }
        case Otr::OtrType::AcknowLedge:
            // TODO accept OTR => we need to inform ruqolaaccount
            deleteLater();
            break;
        }
    }
}

QString OtrNotificationJob::generateText()
{
    QString str;
    // TODO search real name.
    const QString userId = mOtr.userId();

    switch (mOtr.type()) {
    case Otr::OtrType::Unknown:
        break;
    case Otr::OtrType::End: {
        str = mRocketChatAccount->accountName() + QLatin1Char('\n') + i18n("%1 ended the OTR session.", QStringLiteral("test")); // FIXME use correct name
        break;
    }
    case Otr::OtrType::Handshake: {
        str = mRocketChatAccount->accountName() + QLatin1Char('\n')
            + i18n("%1  wants to start OTR. Do you want to accept?.", QStringLiteral("test")); // FIXME use correct name
        break;
    }
    case Otr::OtrType::Deny: {
        str = mRocketChatAccount->accountName() + QLatin1Char('\n') + i18n("%1 denied the OTR session.", QStringLiteral("test")); // FIXME use correct name
        break;
    }
    case Otr::OtrType::AcknowLedge:
        break;
    }
    return str;
}

void OtrNotificationJob::slotActivateNotificationAction(unsigned int val)
{
    // Index == 0 => is the default action. We don't have it.
    switch (val) {
    case 0:
        break;
    case 1:
        rejectOtr();
        break;
    case 2:
        acceptOtr();
        break;
    }
}

void OtrNotificationJob::acceptOtr()
{
    qDebug() << " void OtrNotificationJob::acceptOtr()";
    deleteLater();
}

void OtrNotificationJob::rejectOtr()
{
    qDebug() << " Reject ";
    deleteLater();
}

const Otr &OtrNotificationJob::otr() const
{
    return mOtr;
}

void OtrNotificationJob::setOtr(const Otr &newOtr)
{
    mOtr = newOtr;
}
