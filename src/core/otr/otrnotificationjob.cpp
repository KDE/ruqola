/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

OtrNotificationJob::~OtrNotificationJob() = default;

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
        Q_EMIT endOtr(mOtr);
        deleteLater();
        break;
    }
    case Otr::OtrType::Handshake: {
        auto notification = new KNotification(QStringLiteral("Otr-handshake"), KNotification::CloseOnTimeout);
        notification->setTitle(i18n("OTR"));
        notification->setIconName(QStringLiteral("network-connect"));
        notification->setText(generateText());

        auto rejectAction = notification->addAction(i18n("Reject"));
        connect(rejectAction, &KNotificationAction::activated, this, &OtrNotificationJob::slotRejectOtr);

        auto okAction = notification->addAction(i18n("Ok"));
        connect(okAction, &KNotificationAction::activated, this, &OtrNotificationJob::slotAcceptOtr);
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
        Q_EMIT acknowLedgeOtr(mOtr);
        deleteLater();
        break;
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

void OtrNotificationJob::slotAcceptOtr()
{
    Q_EMIT acceptOtr(mOtr);
    qDebug() << " void OtrNotificationJob::acceptOtr()";
    deleteLater();
}

void OtrNotificationJob::slotRejectOtr()
{
    Q_EMIT rejectOtr(mOtr);
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

#include "moc_otrnotificationjob.cpp"
