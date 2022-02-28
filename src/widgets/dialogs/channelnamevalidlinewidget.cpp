/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlinewidget.h"
#include "channelnamevalidlineedit.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ChannelNameValidLineWidget::ChannelNameValidLineWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mChannelNameValidLineEdit(new ChannelNameValidLineEdit(account, this))
    , mChannelNameLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(QMargins{});

    mChannelNameValidLineEdit->setObjectName(QStringLiteral("mChannelNameValidLineEdit"));
    mainLayout->addWidget(mChannelNameValidLineEdit);

    mChannelNameLabel->setObjectName(QStringLiteral("mChannelNameLabel"));
    mChannelNameLabel->hide();
    mainLayout->addWidget(mChannelNameLabel);
    connect(mChannelNameValidLineEdit, &ChannelNameValidLineEdit::channelIsValid, this, &ChannelNameValidLineWidget::slotChannelIsValid);
}

ChannelNameValidLineWidget::~ChannelNameValidLineWidget() = default;

void ChannelNameValidLineWidget::slotChannelIsValid(bool isValid)
{
    mChannelNameLabel->setVisible(!isValid);
    mChannelNameLabel->setText(i18n("%1 is already used", mChannelNameValidLineEdit->text()));
    Q_EMIT channelIsValid(isValid);
}

QString ChannelNameValidLineWidget::text() const
{
    return mChannelNameValidLineEdit->text();
}
