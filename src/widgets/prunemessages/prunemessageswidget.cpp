/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessageswidget.h"
#include "misc/adduserswidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PruneMessagesWidget::PruneMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mInclusive(new QCheckBox(i18n("Inclusive"), this))
    , mDoNotPrunePinnedMessage(new QCheckBox(i18n("Do not Prune Pinned Messages"), this))
    , mDoNotPruneDiscussionMessage(new QCheckBox(i18n("Do not Prune Discussion Messages"), this))
    , mDoNotPruneThreads(new QCheckBox(i18n("Do not Prune Threads"), this))
    , mOnlyRemoveAttachedFiles(new QCheckBox(i18n("Only Remove Attached Files. Keep messages"), this))
    , mLastestDateTimeEdit(new QDateTimeEdit(this))
    , mOldestDateTimeEdit(new QDateTimeEdit(this))
    , mUsers(new AddUsersWidget(account, this))
    , mInfoLabel(new QLabel(this))
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
    mUsers->setPlaceholderText(i18n("Select users..."));
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

    mInfoLabel->setObjectName(QStringLiteral("mInfoLabel"));
    mInfoLabel->setWordWrap(true);
    mainLayout->addWidget(mInfoLabel);

    mainLayout->addStretch(1);
}

PruneMessagesWidget::~PruneMessagesWidget() = default;

void PruneMessagesWidget::updateLabelInfo()
{
    const QString message = i18n("This will delete all messages in %3 between %1 and %2.",
                                 mLastestDateTimeEdit->dateTime().toString(),
                                 mOldestDateTimeEdit->dateTime().toString(),
                                 mRoomName);
    mInfoLabel->setText(message);
}

void PruneMessagesWidget::slotCheckDateTime()
{
    const bool valid =
        (mLastestDateTimeEdit->dateTime() != mOldestDateTimeEdit->dateTime()) && (mLastestDateTimeEdit->dateTime() < mOldestDateTimeEdit->dateTime());
    updateLabelInfo();
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
    info.ignoreDiscussion = mDoNotPruneDiscussionMessage->isChecked();
    // qDebug() << " info " << info;
    return info;
}

void PruneMessagesWidget::setRoomName(const QString &roomName)
{
    mRoomName = roomName;
}
