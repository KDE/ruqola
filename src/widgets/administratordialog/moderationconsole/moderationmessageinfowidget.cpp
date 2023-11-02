/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidget.h"
#include "moderationreportinfowidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

ModerationMessageInfoWidget::ModerationMessageInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationReportInfoWidget(new ModerationReportInfoWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mModerationReportInfoWidget->setObjectName(QStringLiteral("mModerationReportInfoWidget"));
    mainLayout->addWidget(mModerationReportInfoWidget);
}

ModerationMessageInfoWidget::~ModerationMessageInfoWidget() = default;

void ModerationMessageInfoWidget::setReportInfos(const ModerationReportInfos &infos)
{
    mModerationReportInfoWidget->setReportInfos(infos);
}

#include "moc_moderationmessageinfowidget.cpp"
