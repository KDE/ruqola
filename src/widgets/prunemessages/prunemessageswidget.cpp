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

#include "prunemessageswidget.h"
#include "misc/adduserswidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>

PruneMessagesWidget::PruneMessagesWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *lastestLayout = new QHBoxLayout;
    lastestLayout->setObjectName(QStringLiteral("lastestLayout"));
    lastestLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(lastestLayout);

    QLabel *lastestLabel = new QLabel(i18n("Newer than:"), this);
    lastestLabel->setObjectName(QStringLiteral("lastestLabel"));
    lastestLayout->addWidget(lastestLabel);

    mLastestDateTimeEdit = new QDateTimeEdit(this);
    mLastestDateTimeEdit->setObjectName(QStringLiteral("mLastestDateTimeEdit"));
    lastestLayout->addWidget(mLastestDateTimeEdit);

    QHBoxLayout *oldestLayout = new QHBoxLayout;
    oldestLayout->setObjectName(QStringLiteral("oldestLayout"));
    oldestLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(oldestLayout);

    QLabel *oldestLabel = new QLabel(i18n("Older than:"), this);
    oldestLabel->setObjectName(QStringLiteral("oldestLabel"));
    oldestLayout->addWidget(oldestLabel);

    mOldestDateTimeEdit = new QDateTimeEdit(this);
    mOldestDateTimeEdit->setObjectName(QStringLiteral("mOldestDateTimeEdit"));
    oldestLayout->addWidget(mOldestDateTimeEdit);

    QLabel *usersLabel = new QLabel(i18n("Only Prune content from these users (Keep empty to prune everyone's content)"), this);
    usersLabel->setObjectName(QStringLiteral("usersLabel"));
    usersLabel->setWordWrap(true);
    usersLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(usersLabel);

    mUsers = new AddUsersWidget(this);
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Invite Users..."));
    mainLayout->addWidget(mUsers);

    mInclusive = new QCheckBox(i18n("Inclusive"), this);
    mInclusive->setObjectName(QStringLiteral("mInclusive"));
    mainLayout->addWidget(mInclusive);

    mDoNotPrunePinnedMessage = new QCheckBox(i18n("Do not Prune Pinned Messages"), this);
    mDoNotPrunePinnedMessage->setObjectName(QStringLiteral("mDoNotPrunePinnedMessage"));
    mainLayout->addWidget(mDoNotPrunePinnedMessage);

    mDoNotPruneDiscussionMessage = new QCheckBox(i18n("Do not Prune Discussion Messages"), this);
    mDoNotPruneDiscussionMessage->setObjectName(QStringLiteral("mDoNotPruneDiscussionMessage"));
    mainLayout->addWidget(mDoNotPruneDiscussionMessage);

    mDoNotPruneThreads = new QCheckBox(i18n("Do not Prune Threads"), this);
    mDoNotPruneThreads->setObjectName(QStringLiteral("mDoNotPruneThreads"));
    mainLayout->addWidget(mDoNotPruneThreads);

    mOnlyRemoveAttachedFiles = new QCheckBox(i18n("Only Remove Attached Files. Keep messages"), this);
    mOnlyRemoveAttachedFiles->setObjectName(QStringLiteral("mOnlyRemoveAttachedFiles"));
    mainLayout->addWidget(mOnlyRemoveAttachedFiles);
    mainLayout->addStretch(1);
}

PruneMessagesWidget::~PruneMessagesWidget()
{

}

RocketChatRestApi::ChannelCleanHistoryJob::CleanHistoryInfo PruneMessagesWidget::cleanHistoryInfo() const
{
    RocketChatRestApi::ChannelCleanHistoryJob::CleanHistoryInfo info;
    info.lastest = mLastestDateTimeEdit->dateTime();
    info.oldest = mOldestDateTimeEdit->dateTime();
    info.inclusive = mInclusive->isChecked();
    //TODO verify ? user id or username ?
    info.users = mUsers->usersId();
//    QStringList users;
    //info.excludePinned = false;
    info.filesOnly = mOnlyRemoveAttachedFiles->isChecked();
    return info;
}
