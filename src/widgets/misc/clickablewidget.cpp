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
    , mUserName(userName)
    , mUserLabel(new QLabel(mUserName, this))
    , mClickableLabel(new ClickableLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUserLabel->setObjectName(QStringLiteral("mUserLabel"));
    mainLayout->addWidget(mUserLabel);

    mClickableLabel->setObjectName(QStringLiteral("mClickableLabel"));
    mainLayout->addWidget(mClickableLabel);
    connect(mClickableLabel, &ClickableLabel::clicked, this, &ClickableWidget::slotRemoveUser);
}

ClickableWidget::~ClickableWidget()
{
}

void ClickableWidget::slotRemoveUser()
{
    Q_EMIT removeUser(mUserName);
}

QString ClickableWidget::userName() const
{
    return mUserName;
}

void ClickableWidget::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString ClickableWidget::userId() const
{
    return mUserId;
}

void ClickableWidget::setUserId(const QString &userId)
{
    mUserId = userId;
}

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{
    setPixmap(QIcon::fromTheme(QStringLiteral("delete")).pixmap(18, 18));
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(event);
}
