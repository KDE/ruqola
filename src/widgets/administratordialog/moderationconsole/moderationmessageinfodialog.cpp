/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfodialog.h"

#include "moderationmessageinfowidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myModerationMessageInfoDialogGroupName[] = "ModerationMessageInfoDialog";
}
using namespace Qt::Literals::StringLiterals;
ModerationMessageInfoDialog::ModerationMessageInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mModerationMessageInfoWidget(new ModerationMessageInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Moderation Report"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mModerationMessageInfoWidget->setObjectName(u"mModerationMessageInfoWidget"_s);
    mainLayout->addWidget(mModerationMessageInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ModerationMessageInfoDialog::reject);
    readConfig();
}

ModerationMessageInfoDialog::~ModerationMessageInfoDialog() = default;

void ModerationMessageInfoDialog::setReportInfos(const ModerationReportInfos &info)
{
    mModerationMessageInfoWidget->setReportInfos(info);
}

void ModerationMessageInfoDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myModerationMessageInfoDialogGroupName), QSize(800, 600));
}

#include "moc_moderationmessageinfodialog.cpp"
