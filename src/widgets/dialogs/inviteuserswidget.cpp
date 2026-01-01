/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteuserswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "connection.h"
#include "rocketchataccount.h"

#include "ruqolawidgets_debug.h"

#include <KLineEditEventHandler>

#include <KCollapsibleGroupBox>
#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>

InviteUsersWidget::InviteUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mInviteUserLineEdit(new QLineEdit(this))
    , mExpireDateLabel(new QLabel(this))
    , mExpirationDays(new QComboBox(this))
    , mMaxUses(new QComboBox(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto hlayout = new QHBoxLayout;
    hlayout->setObjectName(u"hlayout"_s);
    hlayout->setContentsMargins({});
    mainLayout->addLayout(hlayout);

    auto label = new QLabel(i18nc("@label:textbox", "Invite Link:"), this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::PlainText);
    hlayout->addWidget(label);

    mInviteUserLineEdit->setObjectName(u"mInviteUserLineEdit"_s);
    mInviteUserLineEdit->setReadOnly(true);
    KLineEditEventHandler::catchReturnKey(mInviteUserLineEdit);
    hlayout->addWidget(mInviteUserLineEdit);

    auto copyLinkButton = new QToolButton(this);
    copyLinkButton->setObjectName(u"copyLinkButton"_s);
    copyLinkButton->setIcon(QIcon::fromTheme(u"edit-copy"_s));
    copyLinkButton->setToolTip(i18nc("@info:tooltip", "Copy link"));
    copyLinkButton->setAutoRaise(true);
    hlayout->addWidget(copyLinkButton);
    connect(copyLinkButton, &QToolButton::clicked, this, &InviteUsersWidget::slotCopyLink);

    auto collapsibleGroupBox = new KCollapsibleGroupBox(this);
    collapsibleGroupBox->setObjectName(u"collapsibleGroupBox"_s);
    collapsibleGroupBox->setTitle(i18n("Options"));
    mainLayout->addWidget(collapsibleGroupBox);

    auto formLayout = new QFormLayout(collapsibleGroupBox);
    formLayout->setObjectName(u"formLayout"_s);
    formLayout->setContentsMargins({});

    mExpirationDays->setObjectName(u"mExpirationDays"_s);
    formLayout->addRow(i18n("Expiration (Days)"), mExpirationDays);

    mMaxUses->setObjectName(u"mMaxUses"_s);
    formLayout->addRow(i18n("Max number of uses"), mMaxUses);

    auto generateNewLink = new QPushButton(i18nc("@action:button", "Generate New Link"), this);
    generateNewLink->setObjectName(u"generateNewLink"_s);
    connect(generateNewLink, &QPushButton::clicked, this, &InviteUsersWidget::slotGenerateNewLink);
    mainLayout->addWidget(generateNewLink);

    mExpireDateLabel->setObjectName(u"mExpireDateLabel"_s);
    mExpireDateLabel->setTextFormat(Qt::PlainText);
    mExpireDateLabel->setWordWrap(true);
    mainLayout->addWidget(mExpireDateLabel);

    mainLayout->addStretch(1);
    fillComboBox();
}

InviteUsersWidget::~InviteUsersWidget() = default;

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
        mExpireDateLabel->setText(i18np("Your invite link will expire on %2 or after %1 use.",
                                        "Your invite link will expire on %2 or after %1 uses.",
                                        info.maxUses,
                                        info.expireDateTime));
    } else {
        mExpireDateLabel->setText(i18n("Your invite link will expire on %1.", info.expireDateTime));
    }
}

QByteArray InviteUsersWidget::roomId() const
{
    return mRoomId;
}

void InviteUsersWidget::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void InviteUsersWidget::generateLink(int maxUses, int numberOfDays)
{
    auto job = new RocketChatRestApi::FindOrCreateInviteJob(this);
    job->setRoomId(mRoomId);
    job->setMaxUses(maxUses);
    job->setNumberOfDays(numberOfDays);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FindOrCreateInviteJob::findOrCreateInviteDone, this, &InviteUsersWidget::slotFindOrCreateInvite);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start findOrCreateInviteJob";
    }
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

#include "moc_inviteuserswidget.cpp"
