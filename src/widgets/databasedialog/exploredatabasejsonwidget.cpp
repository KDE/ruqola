/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasejsonwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseJsonWidget::ExploreDatabaseJsonWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mainLayout->addWidget(mPlainTextEdit);
}

ExploreDatabaseJsonWidget::~ExploreDatabaseJsonWidget() = default;

void ExploreDatabaseJsonWidget::slotLoadedMessages(QList<Message> listMessages)
{
    QString str;
    std::sort(listMessages.begin(), listMessages.end(), [](const Message &lhs, const Message &rhs) {
        return lhs.timeStamp() < rhs.timeStamp();
    });
    for (const auto &m : listMessages) {
        str.append(QString::fromUtf8(m.serialize(m, false)));
    }
    mPlainTextEdit->setPlainText(str);
}

#include "moc_exploredatabasejsonwidget.cpp"
