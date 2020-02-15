/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "usersinroomlabel.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>

UsersInRoomLabel::UsersInRoomLabel(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mIconLabel = new QLabel(this);
    mIconLabel->setObjectName(QStringLiteral("mIconLabel"));
    mainLayout->addWidget(mIconLabel);

    mUserNameLabel = new UserLabel(this);
    mUserNameLabel->setObjectName(QStringLiteral("mUserNameLabel"));
    mainLayout->addWidget(mUserNameLabel);
}

UsersInRoomLabel::~UsersInRoomLabel()
{
}

void UsersInRoomLabel::setUserName(const QString &userName)
{
    mUserNameLabel->setText(userName);
}

void UsersInRoomLabel::setIconStatus(const QString &iconStatus)
{
    mIconLabel->setPixmap(QIcon::fromTheme(iconStatus).pixmap(18, 18));
}

UserLabel::UserLabel(QWidget *parent)
    : QLabel(parent)
{
    setTextFormat(Qt::RichText);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UserLabel::customContextMenuRequested, this, &UserLabel::slotCustomContextMenuRequested);
}

UserLabel::~UserLabel()
{
}

void UserLabel::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(new QAction(i18n("Ignore"), &menu));
    menu.exec(mapToGlobal(pos));
}
