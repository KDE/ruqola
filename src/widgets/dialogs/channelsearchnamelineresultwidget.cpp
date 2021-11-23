/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
