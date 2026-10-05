/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessagesdialog.h"

#include "moderationmessageswidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;

namespace
{
const char myModerationMessagesDialogGroupName[] = "ModerationMessagesDialog";
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

ModerationMessagesDialog::~ModerationMessagesDialog() = default;

void ModerationMessagesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myModerationMessagesDialogGroupName), QSize(800, 600));
}

void ModerationMessagesDialog::setModel(CommonMessageFilterProxyModel *model)
{
    mModerationMessagesWidget->setModel(model);
}

#include "moc_moderationmessagesdialog.cpp"
