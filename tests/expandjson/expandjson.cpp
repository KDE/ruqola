/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "expandjson.h"
#include <QApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

ExpandJson::ExpandJson(QWidget *parent)
    : QWidget(parent)
    , mInput(new QTextEdit(this))
    , mOutput(new QTextEdit(this))
    , mExpandButton(new QPushButton(QStringLiteral("Expand Json String"), this))
{
    auto layout = new QVBoxLayout(this);
    mInput->setAcceptRichText(false);
    layout->addWidget(mInput);

    mOutput->setReadOnly(true);
    layout->addWidget(mOutput);

    layout->addWidget(mExpandButton);
    connect(mExpandButton, &QPushButton::clicked, this, &ExpandJson::slotExpandJson);
}

void ExpandJson::slotExpandJson()
{
    const QString inputStr = mInput->toPlainText().trimmed();
    if (!inputStr.isEmpty()) {
        QJsonDocument json = QJsonDocument::fromJson(inputStr.toUtf8());
        qDebug() << " json " << json << inputStr.toUtf8();
        mOutput->setText(QString::fromUtf8(json.toJson(QJsonDocument::Indented)));
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ExpandJson w;
    w.show();
    return app.exec();
}
