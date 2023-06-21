/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchnamelineresultwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

ChannelSearchNameLineResultWidget::ChannelSearchNameLineResultWidget(QWidget *parent)
    : QWidget(parent)
    , mLabel(new QLabel(this))
    , mClearToolButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(QStringLiteral("mLabel"));
    mainLayout->addWidget(mLabel);

    mClearToolButton->setObjectName(QStringLiteral("mClearToolButton"));
    mClearToolButton->setIcon(QIcon::fromTheme(QStringLiteral("delete")));
    mClearToolButton->setToolTip(i18n("Clear"));
    mainLayout->addWidget(mClearToolButton);
    connect(mClearToolButton, &QToolButton::clicked, this, &ChannelSearchNameLineResultWidget::clearRoomName);
}

ChannelSearchNameLineResultWidget::~ChannelSearchNameLineResultWidget() = default;

void ChannelSearchNameLineResultWidget::setRoomName(const QString &name)
{
    mLabel->setText(name);
}

void ChannelSearchNameLineResultWidget::setReadOnly(bool readOnly)
{
    mClearToolButton->setVisible(!readOnly);
}

#include "moc_channelsearchnamelineresultwidget.cpp"
