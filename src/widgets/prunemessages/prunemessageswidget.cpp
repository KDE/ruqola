/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessageswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/adduserswidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PruneMessagesWidget::PruneMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mInclusive(new QCheckBox(i18nc("@option:check", "Inclusive"), this))
    , mDoNotPrunePinnedMessage(new QCheckBox(i18nc("@option:check", "Do not Prune Pinned Messages"), this))
    , mDoNotPruneDiscussionMessage(new QCheckBox(i18nc("@option:check", "Do not Prune Discussion Messages"), this))
    , mDoNotPruneThreads(new QCheckBox(i18nc("@option:check", "Do not Prune Threads"), this))
    , mOnlyRemoveAttachedFiles(new QCheckBox(i18nc("@option:check", "Only Remove Attached Files. Keep messages"), this))
    , mLastestDateTimeEdit(new QDateTimeEdit(this))
    , mOldestDateTimeEdit(new QDateTimeEdit(this))
    , mUsers(new AddUsersWidget(account, this))
    , mInfoLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto lastestLayout = new QHBoxLayout;
    lastestLayout->setObjectName(u"lastestLayout"_s);
    lastestLayout->setContentsMargins({});
    mainLayout->addLayout(lastestLayout);

    auto lastestLabel = new QLabel(i18nc("@label:textbox", "Newer than:"), this);
    lastestLabel->setObjectName(u"lastestLabel"_s);
    lastestLayout->addWidget(lastestLabel);

    mLastestDateTimeEdit->setObjectName(u"mLastestDateTimeEdit"_s);
    connect(mLastestDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &PruneMessagesWidget::slotCheckDateTime);
    lastestLayout->addWidget(mLastestDateTimeEdit);

    auto oldestLayout = new QHBoxLayout;
    oldestLayout->setObjectName(u"oldestLayout"_s);
    oldestLayout->setContentsMargins({});
    mainLayout->addLayout(oldestLayout);

    auto oldestLabel = new QLabel(i18nc("@label:textbox", "Older than:"), this);
    oldestLabel->setObjectName(u"oldestLabel"_s);
    oldestLayout->addWidget(oldestLabel);

    mOldestDateTimeEdit->setObjectName(u"mOldestDateTimeEdit"_s);
    connect(mOldestDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &PruneMessagesWidget::slotCheckDateTime);
    oldestLayout->addWidget(mOldestDateTimeEdit);

    auto usersLabel = new QLabel(i18nc("@label:textbox", "Only Prune content from these users (Keep empty to prune everyone's content)"), this);
    usersLabel->setObjectName(u"usersLabel"_s);
    usersLabel->setWordWrap(true);
    usersLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(usersLabel);

    mUsers->setObjectName(u"mUsers"_s);
    mUsers->setPlaceholderText(i18nc("@info:placeholder", "Select users…"));
    mainLayout->addWidget(mUsers);

    mInclusive->setObjectName(u"mInclusive"_s);
    mainLayout->addWidget(mInclusive);

    mDoNotPrunePinnedMessage->setObjectName(u"mDoNotPrunePinnedMessage"_s);
    mainLayout->addWidget(mDoNotPrunePinnedMessage);

    mDoNotPruneDiscussionMessage->setObjectName(u"mDoNotPruneDiscussionMessage"_s);
    mainLayout->addWidget(mDoNotPruneDiscussionMessage);

    mDoNotPruneThreads->setObjectName(u"mDoNotPruneThreads"_s);
    mainLayout->addWidget(mDoNotPruneThreads);

    mOnlyRemoveAttachedFiles->setObjectName(u"mOnlyRemoveAttachedFiles"_s);
    mainLayout->addWidget(mOnlyRemoveAttachedFiles);

    mInfoLabel->setObjectName(u"mInfoLabel"_s);
    mInfoLabel->setWordWrap(true);
    mainLayout->addWidget(mInfoLabel);

    mainLayout->addStretch(1);
}

PruneMessagesWidget::~PruneMessagesWidget() = default;

void PruneMessagesWidget::updateLabelInfo()
{
    const QString message = i18n("This will delete all messages in %3 between %1 and %2.",
                                 QLocale().toString(mLastestDateTimeEdit->dateTime()),
                                 QLocale().toString(mOldestDateTimeEdit->dateTime()),
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

#include "moc_prunemessageswidget.cpp"
