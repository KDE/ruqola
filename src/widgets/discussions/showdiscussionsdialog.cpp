/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionsdialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "showdiscussionswidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

namespace
{
static const char myShowDiscussionsDialogGroupName[] = "ShowDiscussionsDialog";
}
ShowDiscussionsDialog::ShowDiscussionsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowDiscussionsWidget(new ShowDiscussionsWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Show Discussions - %1", account ? account->accountName() : QStringLiteral("account")));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowDiscussionsWidget->setObjectName(QStringLiteral("mShowDiscussionsWidget"));
    mainLayout->addWidget(mShowDiscussionsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowDiscussionsDialog::reject);
    connect(mShowDiscussionsWidget, &ShowDiscussionsWidget::loadMoreDiscussion, this, &ShowDiscussionsDialog::slotLoadMoreDiscussions);
    readConfig();
    setAttribute(Qt::WA_DeleteOnClose);
}

ShowDiscussionsDialog::~ShowDiscussionsDialog()
{
    writeConfig();
}

void ShowDiscussionsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowDiscussionsDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowDiscussionsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowDiscussionsDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ShowDiscussionsDialog::slotLoadMoreDiscussions()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    mRocketChatAccount->loadMoreDiscussions(mRoomId);
}

void ShowDiscussionsDialog::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void ShowDiscussionsDialog::setModel(DiscussionsFilterProxyModel *model)
{
    mShowDiscussionsWidget->setModel(model);
}

#include "moc_showdiscussionsdialog.cpp"
