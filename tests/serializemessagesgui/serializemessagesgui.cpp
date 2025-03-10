/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serializemessagesgui.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

SerializeMessagesGui::SerializeMessagesGui(QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new QPlainTextEdit(this))
    , mSerializeTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mPlainTextEdit);

    auto generateJsonbutton = new QPushButton(QStringLiteral("Serialize Json"), this);
    mainLayout->addWidget(generateJsonbutton);

    connect(generateJsonbutton, &QPushButton::clicked, this, [this]() {
        const QString json = mPlainTextEdit->toPlainText();
        if (!json.isEmpty()) {
            const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            // TOOD
        }
    });

    mSerializeTextEdit->setReadOnly(true);
    mainLayout->addWidget(mSerializeTextEdit);
    resize(800, 600);
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
