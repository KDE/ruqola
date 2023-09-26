/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <KMessageBox>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// debug dialogbox => don't translate it
ExploreDatabaseWidget::ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMessageListView(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessageDatabase())
    , mRoomName(new QLineEdit(this))
    , mNumberOfMessages(new QSpinBox(this))
    , mUseStartDateTime(new QCheckBox(QStringLiteral("Start"), this))
    , mStartDateTime(new QDateTimeEdit(this))
    , mUseEndDateTime(new QCheckBox(QStringLiteral("End"), this))
    , mEndDateTime(new QDateTimeEdit(this))
    , mMessageModel(new MessagesModel(QStringLiteral("no_room"), account, nullptr, this))
{
    mNumberOfMessages->setObjectName(QStringLiteral("mNumberOfMessages"));
    mNumberOfMessages->setRange(-1, 9999);
    mNumberOfMessages->setValue(-1);
    mNumberOfMessages->setSpecialValueText(QStringLiteral("All messages"));

    mUseStartDateTime->setObjectName(QStringLiteral("mUseStartDateTime"));
    mStartDateTime->setObjectName(QStringLiteral("mStartDateTime"));
    mUseEndDateTime->setObjectName(QStringLiteral("mUseEndDateTime"));
    mEndDateTime->setObjectName(QStringLiteral("mEndDateTime"));

    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    mainLayout->addLayout(hboxLayout);

    mRoomName->setObjectName(QStringLiteral("mRoomName"));
    mRoomName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mRoomName, this);
    auto label = new QLabel(QStringLiteral("Room name:"), this);
    label->setObjectName(QStringLiteral("label"));
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

    hboxLayout->addWidget(new QLabel(QStringLiteral("Number Of Elements"), this));
    hboxLayout->addWidget(mNumberOfMessages);

    auto pushButton = new QPushButton(QStringLiteral("Load"), this);
    pushButton->setObjectName(QStringLiteral("pushButton"));
    hboxLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &ExploreDatabaseWidget::slotLoad);
    connect(mRoomName, &QLineEdit::returnPressed, this, &ExploreDatabaseWidget::slotLoad);

    mMessageListView->setModel(mMessageModel);
    mainLayout->addWidget(mMessageListView);
}

ExploreDatabaseWidget::~ExploreDatabaseWidget() = default;

void ExploreDatabaseWidget::slotLoad()
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
        const QVector<Message> listMessages = mLocalMessageDatabase->loadMessages(mRocketChatAccount, roomName, startId, endId, mNumberOfMessages->value());
        mMessageModel->clear();
        mMessageModel->addMessages(listMessages);
    } else {
        KMessageBox::error(this, QStringLiteral("Room '%1' does not have database").arg(roomName), QStringLiteral("Database empty"));
    }
}

#include "moc_exploredatabasewidget.cpp"
