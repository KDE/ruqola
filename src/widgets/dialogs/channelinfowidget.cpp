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
#include "roomwrapper.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

ChannelInfoWidget::ChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    //Editable channel
    mEditableChannel = new QWidget(this);
    mEditableChannel->setObjectName(QStringLiteral("mEditableChannel"));
    mStackedWidget->addWidget(mEditableChannel);

    QFormLayout *layout = new QFormLayout(mEditableChannel);
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
    layout->addRow(i18n("Announcement:"), mAnnouncement);

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
    layout->addRow(QStringLiteral(" "), mDeleteChannel);

    //ReadOnly Channel
    mReadOnlyChannel = new QWidget(this);
    mReadOnlyChannel->setObjectName(QStringLiteral("mReadOnlyChannel"));
    mStackedWidget->addWidget(mReadOnlyChannel);

    QFormLayout *layoutReadOnly = new QFormLayout(mReadOnlyChannel);
    layoutReadOnly->setObjectName(QStringLiteral("layoutReadOnly"));
    layoutReadOnly->setContentsMargins(0, 0, 0, 0);

    mNameReadOnly = new QLabel(this);
    mNameReadOnly->setObjectName(QStringLiteral("mNameReadOnly"));
    layoutReadOnly->addRow(i18n("Name:"), mNameReadOnly);

    mCommentReadOnly = new QLabel(this);
    mCommentReadOnly->setObjectName(QStringLiteral("mCommentReadOnly"));
    layoutReadOnly->addRow(i18n("Comment:"), mCommentReadOnly);

    mAnnouncementReadOnly = new QLabel(this);
    mAnnouncementReadOnly->setObjectName(QStringLiteral("mAnnouncementReadOnly"));
    layoutReadOnly->addRow(i18n("Announcement:"), mAnnouncementReadOnly);

    mDescriptionReadOnly = new QLabel(this);
    mDescriptionReadOnly->setObjectName(QStringLiteral("mDescriptionReadOnly"));
    layoutReadOnly->addRow(i18n("Description:"), mDescriptionReadOnly);
}

ChannelInfoWidget::~ChannelInfoWidget()
{
}

void ChannelInfoWidget::setRoomWrapper(RoomWrapper *roomWrapper)
{
   mRoomWrapper = roomWrapper;
   if (mRoomWrapper->canBeModify()) {
       mStackedWidget->setCurrentWidget(mEditableChannel);
       //TODO connect signal/slot
   } else {
       mStackedWidget->setCurrentWidget(mReadOnlyChannel);
   }
}
