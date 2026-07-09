/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverchecklistmanager.h"

#include "addownerusercheck.h"
#include "adduserscheck.h"
#include "announcementchangecheck.h"
#include "deletemessagecheck.h"
#include "descriptionchangecheck.h"
#include "editmessagecheck.h"
#include "favoritemessagecheck.h"
#include "fileuploadcheck.h"
#include "imagedescriptioncheck.h"
#include "linkpreviewcheck.h"
#include "markasfavoriteroomcheck.h"
#include "pinmessagecheck.h"
#include "quotemessagecheck.h"
#include "reactioncheck.h"
#include "removeuserscheck.h"
#include "rocketchataccount.h"
#include "searchmessagecheck.h"
#include "sendmessagecheck.h"
#include "threadreplycheck.h"
#include "topicchangecheck.h"

ServerCheckListManager::ServerCheckListManager(RocketChatAccount *account, const QByteArray &roomId, QObject *parent)
    : QObject(parent)
    , mAccount(account)
    , mRoomId(roomId)
{
    // Add new checks here to grow the checklist.
    mChecks = {
        new SendMessageCheck(account, roomId, this),       new EditMessageCheck(account, roomId, this),
        new DeleteMessageCheck(account, roomId, this),     new ThreadReplyCheck(account, roomId, this),
        new ReactionCheck(account, roomId, this),          new QuoteMessageCheck(account, roomId, this),
        new FavoriteMessageCheck(account, roomId, this),   new PinMessageCheck(account, roomId, this),
        new LinkPreviewCheck(account, roomId, this),       new SearchMessageCheck(account, roomId, this),
        new ImageDescriptionCheck(account, roomId, this),  new FileUploadCheck(account, roomId, this),
        new TopicChangeCheck(account, roomId, this),       new AddUsersCheck(account, roomId, this),
        new RemoveUsersCheck(account, roomId, this),       new MarkAsFavoriteRoomCheck(account, roomId, this),
        new DescriptionChangeCheck(account, roomId, this), new AnnouncementChangeCheck(account, roomId, this),
        new AddOwnerUserCheck(account, roomId, this),
    };
}

ServerCheckListManager::~ServerCheckListManager() = default;

int ServerCheckListManager::checkCount() const
{
    return mChecks.count();
}

QString ServerCheckListManager::checkName(int index) const
{
    return mChecks.at(index)->name();
}

void ServerCheckListManager::start()
{
    mCurrentIndex = -1;
    runNext();
}

void ServerCheckListManager::runNext()
{
    ++mCurrentIndex;
    if (mCurrentIndex >= mChecks.count()) {
        Q_EMIT allFinished();
        return;
    }
    const int index = mCurrentIndex;
    auto check = mChecks.at(index);
    connect(
        check,
        &AbstractServerCheck::finished,
        this,
        [this, index](bool passed, const QString &details) {
            Q_EMIT checkFinished(index, passed, details);
            runNext();
        },
        Qt::SingleShotConnection);
    Q_EMIT checkStarted(index);
    check->start();
}

#include "moc_serverchecklistmanager.cpp"
