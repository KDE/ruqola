/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loaddatabasegui.h"
#include "messages/message.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStandardPaths>
#include <QVBoxLayout>

// Same as in localmessagedatabase.cpp
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table
LoadDataBaseGui::LoadDataBaseGui(QWidget *parent)
    : QWidget{parent}
    , mMessageListView(new MessageListView(new RocketChatAccount(QStringLiteral("test"), this), MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessageDatabase())
    , mAccountName(new QLineEdit(this))
    , mRoomName(new QLineEdit(this))
    , mNumberElement(new QSpinBox(this))
    , mMessageModel(new MessagesModel()) // TODO allow to delete it
{
    auto mainLayout = new QVBoxLayout(this);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(QStringLiteral("Account name:"), this);
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mAccountName);
    mAccountName->setPlaceholderText(QStringLiteral("<account name>"));
    label = new QLabel(QStringLiteral("Room name:"), this);
    hboxLayout->addWidget(label);
    mRoomName->setPlaceholderText(QStringLiteral("all"));
    hboxLayout->addWidget(mRoomName);

    hboxLayout->addWidget(mNumberElement);
    mNumberElement->setRange(-1, 9999);
    mNumberElement->setValue(-1);

    auto pushButton = new QPushButton(QStringLiteral("Load"), this);
    hboxLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &LoadDataBaseGui::slotLoad);

    mMessageListView->setModel(mMessageModel);
    mainLayout->addWidget(mMessageListView);
}

LoadDataBaseGui::~LoadDataBaseGui()
{
    delete mMessageModel;
}

void LoadDataBaseGui::slotLoad()
{
    if (!mRoomName->text().trimmed().isEmpty() && !mAccountName->text().trimmed().isEmpty()) {
        const auto listMessages = mLocalMessageDatabase->loadMessages(mAccountName->text(), mRoomName->text(), -1, -1, mNumberElement->value());
        // qDebug() << " listMessages " << listMessages.count();
        mMessageModel->clear();
        mMessageModel->addMessages(listMessages);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Use specific ruqola name for database path
    app.setApplicationName(QStringLiteral("ruqola"));
    // QStandardPaths::setTestModeEnabled(true);

    LoadDataBaseGui w;
    w.show();
    return app.exec();
}

#include "moc_loaddatabasegui.cpp"
