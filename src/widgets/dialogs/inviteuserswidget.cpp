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

#include "inviteuserswidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "restapirequest.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>
#include <KLineEdit>
#include <QToolButton>
#include <QClipboard>
#include <QApplication>

InviteUsersWidget::InviteUsersWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setObjectName(QStringLiteral("hlayout"));
    hlayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(hlayout);

    QLabel *label = new QLabel(i18n("Invite Link:"), this);
    label->setObjectName(QStringLiteral("label"));
    hlayout->addWidget(label);

    mInviteUserLineEdit = new KLineEdit(this);
    mInviteUserLineEdit->setObjectName(QStringLiteral("mInviteUserLineEdit"));
    mInviteUserLineEdit->setReadOnly(true);
    mInviteUserLineEdit->setTrapReturnKey(true);
    hlayout->addWidget(mInviteUserLineEdit);

    QToolButton *copyLinkButton = new QToolButton(this);
    copyLinkButton->setObjectName(QStringLiteral("copyLinkButton"));
    copyLinkButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    copyLinkButton->setToolTip(i18n("Copy link"));
    hlayout->addWidget(copyLinkButton);
    connect(copyLinkButton, &QToolButton::clicked, this, &InviteUsersWidget::slotCopyLink);

    mLink = new QLabel(this);
    mLink->setObjectName(QStringLiteral("mLink"));
    mainLayout->addWidget(mLink);

    connect(Ruqola::self()->rocketChatAccount()->restApi(), &RocketChatRestApi::RestApiRequest::findOrCreateInviteDone, this, &InviteUsersWidget::slotFindOrCreateInvite);
}

InviteUsersWidget::~InviteUsersWidget()
{
}

void InviteUsersWidget::slotCopyLink()
{
    const QString link = mInviteUserLineEdit->text();
    QClipboard *clip = QApplication::clipboard();
    clip->setText(link, QClipboard::Clipboard);
    clip->setText(link, QClipboard::Selection);
}

void InviteUsersWidget::slotFindOrCreateInvite(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info)
{
    mInviteUserLineEdit->setText(info.url.toString());
    //TODO
}

QString InviteUsersWidget::roomId() const
{
    return mRoomId;
}

void InviteUsersWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void InviteUsersWidget::generateLink(int maxUses, int numberOfDays)
{
    Ruqola::self()->rocketChatAccount()->restApi()->findOrCreateInvite(mRoomId, maxUses, numberOfDays);
}
