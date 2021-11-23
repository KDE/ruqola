/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "clickablewidget.h"
#include <QHBoxLayout>
#include <QIcon>

ClickableWidget::ClickableWidget(const QString &userName, QWidget *parent)
    : QWidget(parent)
    , mName(userName)
    , mLabel(new QLabel(mName, this))
    , mClickableLabel(new ClickableLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(QStringLiteral("mUserLabel"));
    mainLayout->addWidget(mLabel);

    mClickableLabel->setObjectName(QStringLiteral("mClickableLabel"));
    mainLayout->addWidget(mClickableLabel);
    connect(mClickableLabel, &ClickableLabel::clicked, this, &ClickableWidget::slotRemove);
}

ClickableWidget::~ClickableWidget() = default;

void ClickableWidget::slotRemove()
{
    Q_EMIT removeClickableWidget(mName);
}

QString ClickableWidget::name() const
{
    return mName;
}

void ClickableWidget::setName(const QString &userName)
{
    mName = userName;
}

QString ClickableWidget::identifier() const
{
    return mIdentifier;
}

void ClickableWidget::setIdentifier(const QString &userId)
{
    mIdentifier = userId;
}

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{
    setPixmap(QIcon::fromTheme(QStringLiteral("delete")).pixmap(18, 18));
}

ClickableLabel::~ClickableLabel() = default;

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(event);
}
