/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationusersdialog.h"
#include "moderationuserswidget.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myModerationUsersDialogGroupName[] = "ModerationUsersDialog";
}
ModerationUsersDialog::ModerationUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mModerationUsersWidget(new ModerationUsersWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mModerationUsersWidget->setObjectName(QStringLiteral("mModerationUsersWidget"));
    mainLayout->addWidget(mModerationUsersWidget);
    connect(mModerationUsersWidget, &ModerationUsersWidget::goToMessageRequested, this, &ModerationUsersDialog::goToMessageRequested);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ModerationUsersDialog::reject);
    readConfig();
}

ModerationUsersDialog::~ModerationUsersDialog()
{
    writeConfig();
}

void ModerationUsersDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myModerationUsersDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ModerationUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myModerationUsersDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ModerationUsersDialog::setModel(CommonMessageFilterProxyModel *model)
{
    mModerationUsersWidget->setModel(model);
}

#include "moc_moderationusersdialog.cpp"
