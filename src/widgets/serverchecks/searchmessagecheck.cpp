/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagecheck.h"

#include "chat/searchmessagejob.h"
#include "connection.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;

QString SearchMessageCheck::name() const
{
    return i18n("Message search");
}

int SearchMessageCheck::watchdogTimeoutMs() const
{
    // Server-side search indexing can lag, so retry for a while.
    return 60000;
}

void SearchMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    // A single searchable token (no spaces/brackets) to avoid full-text tokenization surprises.
    mToken = u"ruqolachecksearch%1"_s.arg(QDateTime::currentMSecsSinceEpoch());
    connectModel();
    mAccount->sendMessage(mRoomId, mToken);
}

void SearchMessageCheck::checkState()
{
    if (isFinished() || mPhase != Phase::WaitingForSend) {
        return;
    }
    const Message message = findConfirmedMessageByText(mToken);
    if (message.messageId().isEmpty()) {
        return; // not arrived yet
    }
    mMessageId = message.messageId();
    mPhase = Phase::Searching;
    startSearch();
}

void SearchMessageCheck::startSearch()
{
    if (isFinished()) {
        return;
    }
    auto job = new RocketChatRestApi::SearchMessageJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoomId));
    job->setSearchText(mToken);
    connect(job, &RocketChatRestApi::SearchMessageJob::searchMessageDone, this, [this](const QJsonObject &obj) {
        if (isFinished()) {
            return;
        }
        const QJsonArray messages = obj.value("messages"_L1).toArray();
        for (const auto &value : messages) {
            if (value.toObject().value("msg"_L1).toString().contains(mToken)) {
                reportPassed();
                deleteTestMessage(mMessageId);
                return;
            }
        }
        // Not indexed yet: retry until the watchdog gives up.
        QTimer::singleShot(3000, this, [this]() {
            startSearch();
        });
    });
    if (!job->start()) {
        reportFailed(i18n("Could not start the search request."));
    }
}

#include "moc_searchmessagecheck.cpp"
