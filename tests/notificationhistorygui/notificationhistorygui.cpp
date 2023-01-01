/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorygui.h"
#include <QApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QPushButton>
#include <QStandardPaths>
#include <QTextEdit>
#include <QVBoxLayout>
#include <notificationhistory/notificationhistorydialog.h>
#include <notificationhistorymanager.h>

NotificationHistoryGui::NotificationHistoryGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mTextEdit);
    auto pushButton = new QPushButton(QStringLiteral("Send Notification"), this);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &NotificationHistoryGui::slotSendNotification);

    auto dlg = new NotificationHistoryDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void NotificationHistoryGui::slotSendNotification()
{
    const QString plainText = mTextEdit->toPlainText();
    if (!plainText.isEmpty()) {
        NotificationInfo info;
        info.setAccountName(QStringLiteral("accountName")); // TODO
        info.setDateTime(QDateTime::currentDateTime().toString());

        const QByteArray content = plainText.toUtf8();
        QJsonParseError error;
        const auto doc = QJsonDocument::fromJson(content, &error);
        info.parseNotification(doc.array());
        qDebug() << " info " << info;
        NotificationHistoryManager::self()->addNotification(info);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    NotificationHistoryGui w;
    w.show();
    return app.exec();
}
