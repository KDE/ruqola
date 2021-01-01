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

#include "myaccountprofileconfigureavatarwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMenu>
#include <QVBoxLayout>
#include <QDebug>
#include <QInputDialog>

MyAccountProfileConfigureAvatarWidget::MyAccountProfileConfigureAvatarWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mAvatarImage = new AvatarImage(this);
    mAvatarImage->setObjectName(QStringLiteral("mAvatarImage"));
    mainLayout->addWidget(mAvatarImage, 0, Qt::AlignLeft);
}

MyAccountProfileConfigureAvatarWidget::~MyAccountProfileConfigureAvatarWidget()
{
}

AvatarImage::AvatarImage(QWidget *parent)
    : QPushButton(parent)
{
    setIconSize(QSize(100, 100));
    setFixedSize(QSize(120, 120));

    connect(this, &AvatarImage::clicked, this, &AvatarImage::changeImage);
}

AvatarImage::~AvatarImage()
{
}

void AvatarImage::changeImage()
{
    qDebug() << " Not implemented yet";
    //TODO use QFileDialog
}

void AvatarImage::changeUrl()
{
    const QString url = QInputDialog::getText(this, i18n("Change Url"), i18n("Define Avatar Url:"));
    if (!url.isEmpty()) {
        Ruqola::self()->rocketChatAccount()->setAvatarUrl(url);
    }
}

void AvatarImage::resetAvatar()
{
    Ruqola::self()->rocketChatAccount()->resetAvatar();
}

void AvatarImage::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(i18n("Change Picture..."), this, &AvatarImage::changeImage);
    menu.addAction(i18n("Change URL for avatar..."), this, &AvatarImage::changeUrl);
    menu.addSeparator();
    menu.addAction(i18n("Reset Avatar"), this, &AvatarImage::resetAvatar);
    menu.exec(event->globalPos());
}
