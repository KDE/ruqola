/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationusersdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "moderationuserswidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myModerationUsersDialogGroupName[] = "ModerationUsersDialog";
}
ModerationUsersDialog::ModerationUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mModerationUsersWidget(new ModerationUsersWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mModerationUsersWidget->setObjectName(u"mModerationUsersWidget"_s);
    mainLayout->addWidget(mModerationUsersWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ModerationUsersDialog::reject);
    readConfig();
}

ModerationUsersDialog::~ModerationUsersDialog()
{
    writeConfig();
}

void ModerationUsersDialog::setModerationReportUserInfos(const ModerationReportUserInfos &infos)
{
    mModerationUsersWidget->setModerationReportUserInfos(infos);
}

void ModerationUsersDialog::readConfig()
{
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myModerationUsersDialogGroupName), 800, 600);
}

void ModerationUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myModerationUsersDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_moderationusersdialog.cpp"
