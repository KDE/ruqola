/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pinmessagecheck.h"

#include "chat/getpinnedmessagesjob.h"
#include "chat/pinmessagejob.h"
#include "connection.h"
#include "model/messagesmodel.h"
#include "restapiabstractjob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>

QString PinMessageCheck::name() const
{
    return i18n("Pin message");
}

void PinMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker);
}

void PinMessageCheck::checkState()
{
    if (isFinished() || mPhase != Phase::WaitingForSend) {
        return;
    }
    const Message message = findConfirmedMessageByText(mMarker);
    if (message.messageId().isEmpty()) {
        return; // not arrived yet
    }
    mMessageId = message.messageId();
    mPhase = Phase::Working;
    startPin();
}

void PinMessageCheck::startPin()
{
    auto job = new RocketChatRestApi::PinMessageJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setMessageId(mMessageId);
    job->setPinMessage(true);
    connect(job, &RocketChatRestApi::PinMessageJob::pinMessageDone, this, [this]() {
        verifyPinned();
    });
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &serverError, const QString &description) {
        reportFailed(serverError.isEmpty() ? description : serverError);
    });
    if (!job->start()) {
        reportFailed(i18n("Could not start the pin-message request."));
    }
}

void PinMessageCheck::verifyPinned()
{
    if (isFinished()) {
        return;
    }
    // Ask the server for the room's pinned messages rather than relying on a model update.
    auto job = new RocketChatRestApi::GetPinnedMessagesJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(mRoomId);
    connect(job, &RocketChatRestApi::GetPinnedMessagesJob::getPinnedMessagesDone, this, [this](const QJsonObject &obj, const QByteArray &) {
        const QJsonArray messages = obj.value("messages"_L1).toArray();
        for (const auto &value : messages) {
            if (value.toObject().value("_id"_L1).toString().toLatin1() == mMessageId) {
                reportPassed();
                deleteTestMessage(mMessageId);
                return;
            }
        }
        reportFailed(i18n("The message was not found among the server's pinned messages."));
        deleteTestMessage(mMessageId);
    });
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &serverError, const QString &description) {
        reportFailed(serverError.isEmpty() ? description : serverError);
    });
    if (!job->start()) {
        reportFailed(i18n("Could not start the pinned-messages request."));
    }
}

#include "moc_pinmessagecheck.cpp"
