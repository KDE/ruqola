/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationusersdialog.h"

#include "moderationuserswidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;

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

ModerationUsersDialog::~ModerationUsersDialog() = default;

void ModerationUsersDialog::setModerationReportUserInfos(const ModerationReportUserInfos &infos)
{
    mModerationUsersWidget->setModerationReportUserInfos(infos);
}

void ModerationUsersDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myModerationUsersDialogGroupName), QSize(800, 600));
}

#include "moc_moderationusersdialog.cpp"
