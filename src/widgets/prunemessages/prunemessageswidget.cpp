/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PruneMessagesWidget::PruneMessagesWidget(QWidget *parent)
    : QWidget(parent)
    , mInclusive(new QCheckBox(i18n("Inclusive"), this))
    , mDoNotPrunePinnedMessage(new QCheckBox(i18n("Do not Prune Pinned Messages"), this))
    , mDoNotPruneDiscussionMessage(new QCheckBox(i18n("Do not Prune Discussion Messages"), this))
    , mDoNotPruneThreads(new QCheckBox(i18n("Do not Prune Threads"), this))
    , mOnlyRemoveAttachedFiles(new QCheckBox(i18n("Only Remove Attached Files. Keep messages"), this))
    , mLastestDateTimeEdit(new QDateTimeEdit(this))
    , mOldestDateTimeEdit(new QDateTimeEdit(this))
    , mUsers(new AddUsersWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto lastestLayout = new QHBoxLayout;
    lastestLayout->setObjectName(QStringLiteral("lastestLayout"));
    lastestLayout->setContentsMargins({});
    mainLayout->addLayout(lastestLayout);

    auto lastestLabel = new QLabel(i18n("Newer than:"), this);
    lastestLabel->setObjectName(QStringLiteral("lastestLabel"));
    lastestLayout->addWidget(lastestLabel);

    mLastestDateTimeEdit->setObjectName(QStringLiteral("mLastestDateTimeEdit"));
    connect(mLastestDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &PruneMessagesWidget::slotCheckDateTime);
    lastestLayout->addWidget(mLastestDateTimeEdit);

    auto oldestLayout = new QHBoxLayout;
    oldestLayout->setObjectName(QStringLiteral("oldestLayout"));
    oldestLayout->setContentsMargins({});
    mainLayout->addLayout(oldestLayout);

    auto oldestLabel = new QLabel(i18n("Older than:"), this);
    oldestLabel->setObjectName(QStringLiteral("oldestLabel"));
    oldestLayout->addWidget(oldestLabel);

    mOldestDateTimeEdit->setObjectName(QStringLiteral("mOldestDateTimeEdit"));
    connect(mOldestDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &PruneMessagesWidget::slotCheckDateTime);
    oldestLayout->addWidget(mOldestDateTimeEdit);

    auto usersLabel = new QLabel(i18n("Only Prune content from these users (Keep empty to prune everyone's content)"), this);
    usersLabel->setObjectName(QStringLiteral("usersLabel"));
    usersLabel->setWordWrap(true);
    usersLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(usersLabel);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Select Users..."));
    mainLayout->addWidget(mUsers);

    mInclusive->setObjectName(QStringLiteral("mInclusive"));
    mainLayout->addWidget(mInclusive);

    mDoNotPrunePinnedMessage->setObjectName(QStringLiteral("mDoNotPrunePinnedMessage"));
    mainLayout->addWidget(mDoNotPrunePinnedMessage);

    mDoNotPruneDiscussionMessage->setObjectName(QStringLiteral("mDoNotPruneDiscussionMessage"));
    mainLayout->addWidget(mDoNotPruneDiscussionMessage);

    mDoNotPruneThreads->setObjectName(QStringLiteral("mDoNotPruneThreads"));
    mainLayout->addWidget(mDoNotPruneThreads);

    mOnlyRemoveAttachedFiles->setObjectName(QStringLiteral("mOnlyRemoveAttachedFiles"));
    mainLayout->addWidget(mOnlyRemoveAttachedFiles);
    mainLayout->addStretch(1);
}

PruneMessagesWidget::~PruneMessagesWidget()
{
}

void PruneMessagesWidget::slotCheckDateTime()
{
    const bool valid =
        (mLastestDateTimeEdit->dateTime() != mOldestDateTimeEdit->dateTime()) && (mLastestDateTimeEdit->dateTime() > mOldestDateTimeEdit->dateTime());
    Q_EMIT updateOkButton(valid);
}

RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo PruneMessagesWidget::cleanHistoryInfo() const
{
    RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo info;
    info.latest = mLastestDateTimeEdit->dateTime().toUTC();
    info.oldest = mOldestDateTimeEdit->dateTime().toUTC();
    info.inclusive = mInclusive->isChecked();
    info.users = mUsers->userNames();
    info.ignoreThreads = mDoNotPruneThreads->isChecked();
    info.filesOnly = mOnlyRemoveAttachedFiles->isChecked();
    info.excludePinned = mDoNotPrunePinnedMessage->isChecked();
    // qDebug() << " info " << info;
    return info;
}
