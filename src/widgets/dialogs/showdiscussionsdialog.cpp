/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "showdiscussionsdialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "showdiscussionswidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace
{
static const char myShowDiscussionsDialogGroupName[] = "ShowDiscussionsDialog";
}
ShowDiscussionsDialog::ShowDiscussionsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowDiscussionsWidget(new ShowDiscussionsWidget(this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Show Discussions"));
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
}

ShowDiscussionsDialog::~ShowDiscussionsDialog()
{
    writeConfig();
}

void ShowDiscussionsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowDiscussionsDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowDiscussionsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowDiscussionsDialogGroupName);
    group.writeEntry("Size", size());
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
