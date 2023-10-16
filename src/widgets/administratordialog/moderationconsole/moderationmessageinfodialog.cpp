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
ModerationMessageInfoDialog::ModerationMessageInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mModerationMessageInfoWidget(new ModerationMessageInfoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Messages"));
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

void ModerationMessageInfoDialog::showReportInfo(const ModerationReportInfo &info)
{
    mModerationMessageInfoWidget->showReportInfo(info);
}

void ModerationMessageInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myModerationMessageInfoDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ModerationMessageInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myModerationMessageInfoDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_moderationmessageinfodialog.cpp"
