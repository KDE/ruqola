/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serializemessagesgui.h"
using namespace Qt::Literals::StringLiterals;

#include "messages/message.h"
#include "serializemessagewidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStandardPaths>
#include <QVBoxLayout>

SerializeMessagesGui::SerializeMessagesGui(QWidget *parent)
    : QWidget(parent)
    , mOriginal(new SerializeMessageWidget(this))
    , mSerialize(new SerializeMessageWidget(this))
    , mDiffMessage(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);

    auto saveFromFile = new QPushButton(QStringLiteral("Load..."), this);
    mainLayout->addWidget(saveFromFile);
    connect(saveFromFile, &QPushButton::clicked, this, [this]() {
        const QString str = QFileDialog::getOpenFileName(this, QStringLiteral("Load File"));
        if (!str.isEmpty()) {
            QFile f(str);
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << " Impossible to open:" << str;
            } else {
                QTextStream in(&f);
                const QString fileContent = in.readAll();
                f.close();
                mOriginal->setText(fileContent);
            }
        }
    });

    auto splitter = new QSplitter(Qt::Orientation::Vertical, this);
    mainLayout->addWidget(splitter);

    auto mainWidget = new QWidget(this);
    splitter->addWidget(mainWidget);

    auto mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setContentsMargins({});

    mainWidgetLayout->addWidget(mOriginal);

    mSerialize->setReadOnly(true);
    auto generateJsonbutton = new QPushButton(QStringLiteral("Serialize Json"), this);
    mainWidgetLayout->addWidget(generateJsonbutton);
    mainWidgetLayout->addWidget(mSerialize);

    connect(generateJsonbutton, &QPushButton::clicked, this, [this]() {
        const QString json = mOriginal->text();
        if (!json.isEmpty()) {
            const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            if (doc.isObject()) {
                Message msg;
                msg.parseMessage(doc.object(), false, nullptr);

                mOriginal->addMessage(msg);

                const QByteArray ba = Message::serialize(msg, false);
                const QJsonDocument doc2 = QJsonDocument::fromJson(ba);
                Message newMsg = msg.deserialize(doc2.object(), nullptr);

                const QByteArray newBa = Message::serialize(msg, false);
                mSerialize->setText(QString::fromUtf8(newBa));
                mSerialize->addMessage(newMsg);

                const bool equal = (newMsg == msg);
                if (equal) {
                    mDiffMessage->setPlainText(QStringLiteral("Messages are equal"));
                } else {
                    QString diff = QStringLiteral("Original Message %1").arg(QDebug::toString(msg));
                    diff += QStringLiteral("\nSerialized Message %1").arg(QDebug::toString(newMsg));
                    mDiffMessage->setPlainText(QStringLiteral("Messages are NOT equal\n%1").arg(diff));
                }
            } else if (doc.isArray()) {
                // List of messages
                const QJsonArray array = doc.array();
                for (const auto &r : array) {
                    Message msg;
                    msg.parseMessage(r.toObject(), false, nullptr);

                    mOriginal->addMessage(msg);

                    const QByteArray ba = Message::serialize(msg, false);
                    const QJsonDocument doc2 = QJsonDocument::fromJson(ba);
                    Message newMsg = msg.deserialize(doc2.object(), nullptr);

                    const QByteArray newBa = Message::serialize(msg, false);
                    mSerialize->setText(mSerialize->text() + u'\n' + QString::fromUtf8(newBa));
                    mSerialize->addMessage(newMsg);

                    const bool equal = (newMsg == msg);
                    if (equal) {
                        mDiffMessage->setPlainText(mDiffMessage->toPlainText() + u'\n' + QStringLiteral("Messages are equal"));
                    } else {
                        QString diff = QStringLiteral("Original Message %1").arg(QDebug::toString(msg));
                        diff += QStringLiteral("\nSerialized Message %1").arg(QDebug::toString(newMsg));
                        mDiffMessage->setPlainText(mDiffMessage->toPlainText() + u'\n' + QStringLiteral("Messages are NOT equal\n%1").arg(diff));
                    }
                }
            } else {
                qDebug() << " Invalid json" << doc.isEmpty();
            }
        }
    });

    mDiffMessage->setReadOnly(true);
    splitter->addWidget(mDiffMessage);

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
