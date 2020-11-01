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
#include "misc/lineeditcatchreturnkey.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QClipboard>
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <KCollapsibleGroupBox>

InviteUsersWidget::InviteUsersWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto *hlayout = new QHBoxLayout;
    hlayout->setObjectName(QStringLiteral("hlayout"));
    hlayout->setContentsMargins({});
    mainLayout->addLayout(hlayout);

    QLabel *label = new QLabel(i18n("Invite Link:"), this);
    label->setObjectName(QStringLiteral("label"));
    hlayout->addWidget(label);

    mInviteUserLineEdit = new QLineEdit(this);
    mInviteUserLineEdit->setObjectName(QStringLiteral("mInviteUserLineEdit"));
    mInviteUserLineEdit->setReadOnly(true);
    new LineEditCatchReturnKey(mInviteUserLineEdit, this);
    hlayout->addWidget(mInviteUserLineEdit);

    auto *copyLinkButton = new QToolButton(this);
    copyLinkButton->setObjectName(QStringLiteral("copyLinkButton"));
    copyLinkButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    copyLinkButton->setToolTip(i18n("Copy link"));
    hlayout->addWidget(copyLinkButton);
    connect(copyLinkButton, &QToolButton::clicked, this, &InviteUsersWidget::slotCopyLink);

    auto *collapsibleGroupBox = new KCollapsibleGroupBox(this);
    collapsibleGroupBox->setObjectName(QStringLiteral("collapsibleGroupBox"));
    collapsibleGroupBox->setTitle(i18n("Options"));
    mainLayout->addWidget(collapsibleGroupBox);

    connect(Ruqola::self()->rocketChatAccount()->restApi(), &RocketChatRestApi::RestApiRequest::findOrCreateInviteDone, this, &InviteUsersWidget::slotFindOrCreateInvite);

    auto *formLayout = new QFormLayout(collapsibleGroupBox);
    formLayout->setObjectName(QStringLiteral("formLayout"));
    formLayout->setContentsMargins({});

    mExpirationDays = new QComboBox(this);
    mExpirationDays->setObjectName(QStringLiteral("mExpirationDays"));
    formLayout->addRow(i18n("Expiration (Days)"), mExpirationDays);

    mMaxUses = new QComboBox(this);
    mMaxUses->setObjectName(QStringLiteral("mMaxUses"));
    formLayout->addRow(i18n("Max number of uses"), mMaxUses);

    QPushButton *generateNewLink = new QPushButton(i18n("Generate New Link"), this);
    generateNewLink->setObjectName(QStringLiteral("generateNewLink"));
    connect(generateNewLink, &QPushButton::clicked, this, &InviteUsersWidget::slotGenerateNewLink);
    mainLayout->addWidget(generateNewLink);

    mExpireDateLabel = new QLabel(this);
    mExpireDateLabel->setObjectName(QStringLiteral("mExpireDateLabel"));
    mExpireDateLabel->setWordWrap(true);
    mainLayout->addWidget(mExpireDateLabel);

    mainLayout->addStretch(1);
    fillComboBox();
}

InviteUsersWidget::~InviteUsersWidget()
{
}

void InviteUsersWidget::slotGenerateNewLink()
{
    generateLink(mMaxUses->currentData().toInt(), mExpirationDays->currentData().toInt());
}

void InviteUsersWidget::slotCopyLink()
{
    const QString link = mInviteUserLineEdit->text();
    if (!link.isEmpty()) {
        QClipboard *clip = QApplication::clipboard();
        clip->setText(link, QClipboard::Clipboard);
        clip->setText(link, QClipboard::Selection);
    }
}

void InviteUsersWidget::slotFindOrCreateInvite(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info)
{
    mInviteUserLineEdit->setText(info.url.toString());
    if (info.maxUses > 0) {
        mExpireDateLabel->setText(i18np("Your invite link will expire on %2 or after %1 use.", "Your invite link will expire on %2 or after %1 uses.", info.maxUses, info.expireDateTime));
    } else {
        mExpireDateLabel->setText(i18n("Your invite link will expire on %1.", info.expireDateTime));
    }
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

void InviteUsersWidget::fillComboBox()
{
    mExpirationDays->addItem(QString::number(1), 1);
    mExpirationDays->addItem(QString::number(7), 7);
    mExpirationDays->addItem(QString::number(15), 15);
    mExpirationDays->addItem(QString::number(30), 30);
    mExpirationDays->addItem(i18n("Never"), 0);

    mMaxUses->addItem(QString::number(1), 1);
    mMaxUses->addItem(QString::number(5), 5);
    mMaxUses->addItem(QString::number(10), 10);
    mMaxUses->addItem(QString::number(25), 25);
    mMaxUses->addItem(QString::number(50), 50);
    mMaxUses->addItem(QString::number(100), 100);
    mMaxUses->addItem(i18n("No Limit"), 0);
    mMaxUses->setCurrentIndex(3);
}
