/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidget.h"
using namespace Qt::Literals::StringLiterals;

#include "moderationreportinfowidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

ModerationMessageInfoWidget::ModerationMessageInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationReportInfoWidget(new ModerationReportInfoWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mModerationReportInfoWidget->setObjectName(u"mModerationReportInfoWidget"_s);
    mainLayout->addWidget(mModerationReportInfoWidget);
}

ModerationMessageInfoWidget::~ModerationMessageInfoWidget() = default;

void ModerationMessageInfoWidget::setReportInfos(const ModerationReportInfos &infos)
{
    mModerationReportInfoWidget->setReportInfos(infos);
}

#include "moc_moderationmessageinfowidget.cpp"
