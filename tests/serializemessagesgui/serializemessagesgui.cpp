/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serializemessagesgui.h"
#include "messages/message.h"
#include "serializemessagewidget.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

SerializeMessagesGui::SerializeMessagesGui(QWidget *parent)
    : QWidget(parent)
    , mOriginal(new SerializeMessageWidget(this))
    , mSerialize(new SerializeMessageWidget(this))
    , mDiffMessage(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(mOriginal);

    auto generateJsonbutton = new QPushButton(QStringLiteral("Serialize Json"), this);
    mainLayout->addWidget(generateJsonbutton);
    mainLayout->addWidget(mSerialize);

    connect(generateJsonbutton, &QPushButton::clicked, this, [this]() {
        const QString json = mOriginal->text();
        if (!json.isEmpty()) {
            const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            Message msg;
            msg.parseMessage(doc.object(), false, nullptr);

            mOriginal->addMessage(msg);

            const QByteArray ba = Message::serialize(msg, false);
            const QJsonDocument doc2 = QJsonDocument::fromJson(ba);
            Message newMsg = msg.deserialize(doc2.object(), nullptr);

            const QByteArray newBa = Message::serialize(msg, false);
            mSerialize->setText(QString::fromUtf8(newBa));
            mSerialize->addMessage(newMsg);

            qDebug() << " equal " << (newMsg == msg);
        }
    });

    mDiffMessage->setReadOnly(true);
    mainLayout->addWidget(mDiffMessage);

    resize(1200, 800);
}

SerializeMessagesGui::~SerializeMessagesGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    SerializeMessagesGui w;
    w.show();
    return app.exec();
}

#include "moc_serializemessagesgui.cpp"
