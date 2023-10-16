/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidget.h"
#include <KLocalizedString>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>

ModerationMessageInfoWidget::ModerationMessageInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mReportInfoText(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mReportInfoText->setObjectName(QStringLiteral("mReportInfoText"));
    mainLayout->addWidget(mReportInfoText);
    mReportInfoText->setWordWrap(true);
    mainLayout->addStretch(1);
}

ModerationMessageInfoWidget::~ModerationMessageInfoWidget() = default;

void ModerationMessageInfoWidget::showReportInfo(const ModerationReportInfo &info)
{
    qDebug() << " info " << info;
    // TODO
    mReportInfoText->setText(info.description());
}

#include "moc_moderationmessageinfowidget.cpp"
