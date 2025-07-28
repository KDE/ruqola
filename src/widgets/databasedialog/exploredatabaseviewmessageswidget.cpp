/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseviewmessageswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "exploredatabaselineedit.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <KLineEditEventHandler>
#include <KMessageBox>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// debug dialogbox => don't translate it
ExploreDatabaseViewMessagesWidget::ExploreDatabaseViewMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMessageListView(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessagesDatabase())
    , mRoomName(new ExploreDatabaseLineEdit(account, this))
    , mNumberOfMessages(new QSpinBox(this))
    , mUseStartDateTime(new QCheckBox(u"Start"_s, this))
    , mStartDateTime(new QDateTimeEdit(this))
    , mUseEndDateTime(new QCheckBox(u"End"_s, this))
    , mEndDateTime(new QDateTimeEdit(this))
    , mMessageModel(new MessagesModel("no_room"_ba, account, nullptr, this))
{
    mNumberOfMessages->setObjectName(u"mNumberOfMessages"_s);
    mNumberOfMessages->setRange(-1, 9999);
    mNumberOfMessages->setValue(-1);
    mNumberOfMessages->setSpecialValueText(u"All messages"_s);

    mUseStartDateTime->setObjectName(u"mUseStartDateTime"_s);
    mStartDateTime->setObjectName(u"mStartDateTime"_s);
    mUseEndDateTime->setObjectName(u"mUseEndDateTime"_s);
    mEndDateTime->setObjectName(u"mEndDateTime"_s);

    mMessageListView->setObjectName(u"mMessageListView"_s);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    hboxLayout->setObjectName(u"hboxLayout"_s);
    mainLayout->addLayout(hboxLayout);

    mRoomName->setObjectName(u"mRoomName"_s);
    mRoomName->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mRoomName);
    auto label = new QLabel(u"Room name:"_s, this);
    label->setObjectName(u"label"_s);
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mRoomName);

    hboxLayout->addWidget(mUseStartDateTime);
    hboxLayout->addWidget(mStartDateTime);
    hboxLayout->addWidget(mUseEndDateTime);
    hboxLayout->addWidget(mEndDateTime);

    mStartDateTime->setEnabled(false);
    mEndDateTime->setEnabled(false);

    connect(mUseStartDateTime, &QCheckBox::clicked, mStartDateTime, &QDateTimeEdit::setEnabled);
    connect(mUseEndDateTime, &QCheckBox::clicked, mEndDateTime, &QDateTimeEdit::setEnabled);

    hboxLayout->addWidget(new QLabel(u"Number Of Elements"_s, this));
    hboxLayout->addWidget(mNumberOfMessages);

    auto pushButton = new QPushButton(u"Load"_s, this);
    pushButton->setObjectName(u"pushButton"_s);
    hboxLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &ExploreDatabaseViewMessagesWidget::slotLoad);
    connect(mRoomName, &QLineEdit::returnPressed, this, &ExploreDatabaseViewMessagesWidget::slotLoad);

    mMessageListView->setModel(mMessageModel);
    mainLayout->addWidget(mMessageListView);
}

ExploreDatabaseViewMessagesWidget::~ExploreDatabaseViewMessagesWidget() = default;

void ExploreDatabaseViewMessagesWidget::slotLoad()
{
    const QString roomName = mRoomName->text().trimmed();
    if (!roomName.isEmpty()) {
        qint64 startId = -1;
        qint64 endId = -1;
        if (mUseStartDateTime->isChecked()) {
            startId = mStartDateTime->dateTime().toMSecsSinceEpoch();
        }
        if (mUseEndDateTime->isChecked()) {
            endId = mEndDateTime->dateTime().toMSecsSinceEpoch();
        }
        const QList<Message> listMessages = mLocalMessageDatabase->loadMessages(mRocketChatAccount, roomName, startId, endId, mNumberOfMessages->value());
        mMessageModel->clear();
        mMessageModel->addMessages(listMessages);
        Q_EMIT messagesLoaded(listMessages);
        if (listMessages.isEmpty()) {
            KMessageBox::error(this, u"Room '%1' does not have database"_s.arg(roomName), u"Database empty"_s);
        }
    } else {
        Q_EMIT messagesLoaded({});
        KMessageBox::error(this, u"Room '%1' does not have database"_s.arg(roomName), u"Database empty"_s);
    }
}

#include "moc_exploredatabaseviewmessageswidget.cpp"
