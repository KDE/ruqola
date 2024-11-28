/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfowidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
using namespace Qt::Literals::StringLiterals;
AppsCountInfoWidget::AppsCountInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mProgressBar(new QProgressBar(this))
    , mApplicationInfo(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mainLayout->addStretch(1);

    mProgressBar->setObjectName("mProgressBar"_L1);
    mainLayout->addWidget(mProgressBar);
    mProgressBar->setTextVisible(false);
    mApplicationInfo->setObjectName("mApplicationInfo"_L1);
    mainLayout->addWidget(mApplicationInfo);
}

AppsCountInfoWidget::~AppsCountInfoWidget() = default;

void AppsCountInfoWidget::setAppCountInfo(const AppsCountInfo &info, InfoType type)
{
    setVisible(info.isValid());
    switch (type) {
    case AppsCountInfoWidget::InfoType::Unknown:
        break;
    case AppsCountInfoWidget::InfoType::Applications: {
        mProgressBar->setMaximum(info.maxMarketplaceApps());
        mProgressBar->setValue(info.totalMarketplaceEnabled());
        mApplicationInfo->setText(QStringLiteral("%1/%2").arg(QString::number(info.totalMarketplaceEnabled()), QString::number(info.maxMarketplaceApps())));
        break;
    }
    case AppsCountInfoWidget::InfoType::PrivateApps: {
        mProgressBar->setMaximum(info.maxPrivateApps());
        mProgressBar->setValue(info.totalPrivateEnabled());
        mApplicationInfo->setText(QStringLiteral("%1/%2").arg(QString::number(info.totalPrivateEnabled()), QString::number(info.maxPrivateApps())));
        break;
    }
    }
}

#include "moc_appscountinfowidget.cpp"
