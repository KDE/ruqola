/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessagesdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "moderationmessageswidget.h"
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
static const char myModerationMessagesDialogGroupName[] = "ModerationMessagesDialog";
}
ModerationMessagesDialog::ModerationMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mModerationMessagesWidget(new ModerationMessagesWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mModerationMessagesWidget->setObjectName(u"mModerationMessagesWidget"_s);
    mainLayout->addWidget(mModerationMessagesWidget);
    connect(mModerationMessagesWidget, &ModerationMessagesWidget::goToMessageRequested, this, &ModerationMessagesDialog::goToMessageRequested);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ModerationMessagesDialog::reject);
    readConfig();
}

ModerationMessagesDialog::~ModerationMessagesDialog()
{
    writeConfig();
}

void ModerationMessagesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myModerationMessagesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ModerationMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myModerationMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ModerationMessagesDialog::setModel(CommonMessageFilterProxyModel *model)
{
    mModerationMessagesWidget->setModel(model);
}

#include "moc_moderationmessagesdialog.cpp"
