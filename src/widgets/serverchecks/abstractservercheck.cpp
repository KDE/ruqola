/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstractservercheck.h"

#include "chat/deletemessagejob.h"
#include "connection.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QDateTime>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;

AbstractServerCheck::AbstractServerCheck(RocketChatAccount *account, const QByteArray &roomId, QObject *parent)
    : QObject(parent)
    , mAccount(account)
    , mRoomId(roomId)
    , mWatchdog(new QTimer(this))
{
    mWatchdog->setSingleShot(true);
    connect(mWatchdog, &QTimer::timeout, this, &AbstractServerCheck::slotTimeout);
}

AbstractServerCheck::~AbstractServerCheck() = default;

int AbstractServerCheck::watchdogTimeoutMs() const
{
    // Long enough for an upload round-trip on a slow server.
    return 30000;
}

void AbstractServerCheck::start()
{
    mWatchdog->start(watchdogTimeoutMs());
    doStart();
}

void AbstractServerCheck::slotTimeout()
{
    reportFailed(i18n("Timed out waiting for the server to answer."));
}

bool AbstractServerCheck::isFinished() const
{
    return mFinished;
}

void AbstractServerCheck::reportPassed()
{
    if (mFinished) {
        return;
    }
    mFinished = true;
    mWatchdog->stop();
    Q_EMIT finished(true, QString());
}

void AbstractServerCheck::reportFailed(const QString &details)
{
    if (mFinished) {
        return;
    }
    mFinished = true;
    mWatchdog->stop();
    Q_EMIT finished(false, details);
}

MessagesModel *AbstractServerCheck::messageModel() const
{
    if (!mAccount) {
        return nullptr;
    }
    return mAccount->messageModelForRoom(mRoomId);
}

void AbstractServerCheck::connectModel()
{
    auto model = messageModel();
    if (!model) {
        return;
    }
    connect(model, &QAbstractItemModel::rowsInserted, this, [this]() {
        checkState();
    });
    connect(model, &QAbstractItemModel::rowsRemoved, this, [this]() {
        checkState();
    });
    connect(model, &QAbstractItemModel::dataChanged, this, [this]() {
        checkState();
    });
}

Message AbstractServerCheck::findConfirmedMessageByText(const QString &text) const
{
    auto model = messageModel();
    if (!model) {
        return {};
    }
    const int rows = model->rowCount();
    for (int row = 0; row < rows; ++row) {
        const QByteArray messageId = model->messageIdFromIndex(row);
        const Message message = model->findMessageById(messageId);
        if (!message.pendingMessage() && message.text() == text) {
            return message;
        }
    }
    return {};
}

void AbstractServerCheck::deleteTestMessage(const QByteArray &messageId)
{
    if (!mAccount || messageId.isEmpty()) {
        return;
    }
    auto job = new RocketChatRestApi::DeleteMessageJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(mRoomId);
    job->setMessageId(messageId);
    if (!job->start()) {
        job->deleteLater();
    }
}

QString AbstractServerCheck::generateMarker()
{
    return u"[ruqola-server-check %1]"_s.arg(QDateTime::currentMSecsSinceEpoch());
}

#include "moc_abstractservercheck.cpp"
