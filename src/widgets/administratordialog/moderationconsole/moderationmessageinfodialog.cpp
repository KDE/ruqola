/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfodialog.h"
#include "moderationmessageinfowidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myModerationMessageInfoDialogGroupName[] = "ModerationMessageInfoDialog";
}
ModerationMessageInfoDialog::ModerationMessageInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mModerationMessageInfoWidget(new ModerationMessageInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Report"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mModerationMessageInfoWidget->setObjectName(QStringLiteral("mModerationMessageInfoWidget"));
    mainLayout->addWidget(mModerationMessageInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ModerationMessageInfoDialog::reject);
    readConfig();
}

ModerationMessageInfoDialog::~ModerationMessageInfoDialog()
{
    writeConfig();
}

void ModerationMessageInfoDialog::setReportInfos(const ModerationReportInfos &info)
{
    mModerationMessageInfoWidget->setReportInfos(info);
}

void ModerationMessageInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myModerationMessageInfoDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ModerationMessageInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myModerationMessageInfoDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_moderationmessageinfodialog.cpp"
