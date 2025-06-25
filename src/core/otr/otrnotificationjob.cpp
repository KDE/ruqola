/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrnotificationjob.h"
using namespace Qt::Literals::StringLiterals;

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
        auto notification = new KNotification(u"Otr-end"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("OTR"));
        notification->setIconName(u"network-connect"_s);
        notification->setText(generateText());
        notification->sendEvent();
        Q_EMIT endOtr(mOtr);
        deleteLater();
        break;
    }
    case Otr::OtrType::Handshake: {
        auto notification = new KNotification(u"Otr-handshake"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("OTR"));
        notification->setIconName(u"network-connect"_s);
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
        auto notification = new KNotification(u"Otr-deny"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("OTR"));
        notification->setIconName(u"network-connect"_s);
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
    // const QByteArray userId = mOtr.userId();

    switch (mOtr.type()) {
    case Otr::OtrType::Unknown:
        break;
    case Otr::OtrType::End: {
        str = mRocketChatAccount->accountName() + u'\n' + i18n("%1 ended the OTR session.", u"test"_s); // FIXME use correct name
        break;
    }
    case Otr::OtrType::Handshake: {
        str = mRocketChatAccount->accountName() + u'\n' + i18n("%1 wants to start OTR. Do you want to accept?", u"test"_s); // FIXME use correct name
        break;
    }
    case Otr::OtrType::Deny: {
        str = mRocketChatAccount->accountName() + u'\n' + i18n("%1 denied the OTR session.", u"test"_s); // FIXME use correct name
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
