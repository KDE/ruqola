/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidget.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QVBoxLayout>
// Same as in localmessagedatabase.cpp
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table
ExploreDatabaseWidget::ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMessageListView(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessageDatabase())
    , mAccountName(new QLineEdit(this))
    , mRoomName(new QLineEdit(this))
    , mMessageModel(new MessageModel()) // TODO allow to delete it
{
    auto mainLayout = new QVBoxLayout(this);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(QStringLiteral("Account name:"), this);
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mAccountName);
    label = new QLabel(QStringLiteral("Room name:"), this);
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mRoomName);

    auto pushButton = new QPushButton(QStringLiteral("Load"), this);
    hboxLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &ExploreDatabaseWidget::slotLoad);

    mMessageListView->setModel(mMessageModel);
    mainLayout->addWidget(mMessageListView);
}

ExploreDatabaseWidget::~ExploreDatabaseWidget() = default;

void ExploreDatabaseWidget::slotLoad()
{
    if (!mRoomName->text().trimmed().isEmpty() && !mAccountName->text().trimmed().isEmpty()) {
        auto tableModel = mLocalMessageDatabase->createMessageModel(mAccountName->text(), mRoomName->text());
        qDebug() << " tableModel " << tableModel.get();
        QVector<Message> listMessages;
        if (tableModel) {
            int rows = tableModel->rowCount();
            for (int row = 0; row < rows; ++row) {
                const QSqlRecord record = tableModel->record(row);
                // const QDateTime timeStamp = QDateTime::fromMSecsSinceEpoch(record.value(int(Fields::TimeStamp)).toULongLong());
                const QString json = record.value(int(Fields::Json)).toString();
                qDebug() << " json111 " << json.toUtf8();
                QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
                qDebug() << " doc " << doc;
                // qDebug() << " json " << QCborValue::fromCbor(json.toUtf8()).toMap();
                const Message msg = Message::deserialize(doc.object());
                qDebug() << " msg " << msg;
                listMessages.append(std::move(msg));
                if (row == rows - 1 && tableModel->canFetchMore()) {
                    tableModel->fetchMore();
                    rows = tableModel->rowCount();
                }
            }
            qDebug() << " listMessages " << listMessages.count();
            mMessageModel->clear();
            mMessageModel->addMessages(listMessages);
        }
    }
}
