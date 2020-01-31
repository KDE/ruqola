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

#include "channelinfowidget.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>

ChannelInfoWidget::ChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mName = new KLineEdit(this);
    mName->setObjectName(QStringLiteral("mName"));
    mName->setTrapReturnKey(true);
    mName->setClearButtonEnabled(true);
    layout->addRow(i18n("Name:"), mName);

    mComment = new KLineEdit(this);
    mComment->setObjectName(QStringLiteral("mComment"));
    mComment->setTrapReturnKey(true);
    mComment->setClearButtonEnabled(true);
    layout->addRow(i18n("Comment:"), mComment);

    mAnnouncement = new KLineEdit(this);
    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    mAnnouncement->setTrapReturnKey(true);
    mAnnouncement->setClearButtonEnabled(true);
    layout->addRow(i18n("Annoucement:"), mAnnouncement);

    mDescription = new KLineEdit(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setTrapReturnKey(true);
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);

    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    layout->addRow(i18n("Password:"), mPasswordLineEdit);

    mReadOnly = new QCheckBox(this);
    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    layout->addRow(i18n("ReadOnly:"), mReadOnly);

    mArchive = new QCheckBox(this);
    mArchive->setObjectName(QStringLiteral("mArchive"));
    layout->addRow(i18n("Archive:"), mArchive);

    mPrivate = new QCheckBox(this);
    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);

    mDeleteChannel = new QPushButton(i18n("Delete"), this); //TODO add icons!
    mDeleteChannel->setObjectName(QStringLiteral("mDeleteChannel"));
    layout->addRow(QStringLiteral(" "), mPrivate);

}

ChannelInfoWidget::~ChannelInfoWidget()
{
}
