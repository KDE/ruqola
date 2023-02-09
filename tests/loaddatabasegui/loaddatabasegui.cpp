/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loaddatabasegui.h"
#include "room/messagelistview.h"
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlTableModel>
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
    , mMessageListView(new MessageListView(nullptr, MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessageDatabase())
    , mAccountName(new QLineEdit(this))
    , mRoomName(new QLineEdit(this))
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
    connect(pushButton, &QPushButton::clicked, this, &LoadDataBaseGui::slotLoad);

    mainLayout->addWidget(mMessageListView);
}

void LoadDataBaseGui::slotLoad()
{
    if (!mRoomName->text().trimmed().isEmpty() && !mAccountName->text().trimmed().isEmpty()) {
        auto tableModel = mLocalMessageDatabase->createMessageModel(mAccountName->text(), mRoomName->text());
        qDebug() << " tableModel " << tableModel.get();
        if (tableModel) {
            int rows = tableModel->rowCount();
            for (int row = 0; row < rows; ++row) {
                const QSqlRecord record = tableModel->record(row);
                // const QDateTime timeStamp = QDateTime::fromMSecsSinceEpoch(record.value(int(Fields::TimeStamp)).toULongLong());
                // const QString json = record.value(int(Fields::Json)).toString();
                if (row == rows - 1 && tableModel->canFetchMore()) {
                    tableModel->fetchMore();
                    rows = tableModel->rowCount();
                }
            }
            // TODO create messages!

            mMessageListView->setModel(tableModel.get());
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Use specific ruqola name for databae path
    app.setApplicationName(QStringLiteral("ruqola"));
    // QStandardPaths::setTestModeEnabled(true);

    LoadDataBaseGui w;
    w.show();
    return app.exec();
}
