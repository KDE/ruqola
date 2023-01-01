/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlinewidget.h"
#include "rocketchataccount.h"

#include <KColorScheme>
#include <KLocalizedString>
#include <KStatefulBrush>
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
    mChannelNameLabel->setTextFormat(Qt::PlainText);

    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QColor color = bgBrush.brush(palette()).color();

    QPalette pal = mChannelNameLabel->palette();
    pal.setColor(QPalette::WindowText, color);
    mChannelNameLabel->setPalette(pal);
    mChannelNameLabel->setAutoFillBackground(true);

    mChannelNameLabel->hide();
    mainLayout->addWidget(mChannelNameLabel);
    connect(mChannelNameValidLineEdit, &ChannelNameValidLineEdit::channelIsValid, this, &ChannelNameValidLineWidget::slotChannelIsValid);
}

ChannelNameValidLineWidget::~ChannelNameValidLineWidget() = default;

void ChannelNameValidLineWidget::slotChannelIsValid(ChannelNameValidLineEdit::ChannelNameStatus status)
{
    bool isValid = false;
    switch (status) {
    case ChannelNameValidLineEdit::ChannelNameStatus::Unknown:
        isValid = false;
        mChannelNameLabel->setHidden(true);
        break;
    case ChannelNameValidLineEdit::ChannelNameStatus::Valid:
        isValid = true;
        mChannelNameLabel->setHidden(true);
        break;
    case ChannelNameValidLineEdit::ChannelNameStatus::AlreadyExistingName:
        mChannelNameLabel->setText(i18n("%1 name is already used.", mChannelNameValidLineEdit->text()));
        mChannelNameLabel->setHidden(false);
        isValid = false;
        break;
    case ChannelNameValidLineEdit::ChannelNameStatus::InvalidCharacters:
        isValid = false;
        mChannelNameLabel->setText(i18n("Invalid characters found."));
        mChannelNameLabel->setHidden(false);
        break;
    }
    Q_EMIT channelIsValid(isValid);
}

QString ChannelNameValidLineWidget::text() const
{
    return mChannelNameValidLineEdit->text();
}
