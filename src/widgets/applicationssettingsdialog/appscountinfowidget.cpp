/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
    , mEnableAppsLabel(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mainLayout->addStretch(1);

    mEnableAppsLabel->setObjectName("mEnableAppsLabel"_L1);
    mainLayout->addWidget(mEnableAppsLabel);

    mProgressBar->setObjectName("mProgressBar"_L1);
    mainLayout->addWidget(mProgressBar);
    mProgressBar->setTextVisible(false);
    mApplicationInfo->setObjectName("mApplicationInfo"_L1);
    mainLayout->addWidget(mApplicationInfo);
}

AppsCountInfoWidget::~AppsCountInfoWidget() = default;

void AppsCountInfoWidget::setAppCountInfo(const AppsCountInfo &info)
{
    setVisible(info.isValid());
    switch (mInfotype) {
    case AppsCountInfoWidget::InfoType::Unknown:
        break;
    case AppsCountInfoWidget::InfoType::Applications: {
        mProgressBar->setMaximum(info.maxMarketplaceApps());
        mProgressBar->setValue(info.totalMarketplaceEnabled());
        mApplicationInfo->setText(u"%1/%2"_s.arg(QString::number(info.totalMarketplaceEnabled()), QString::number(info.maxMarketplaceApps())));
        mEnableAppsLabel->setText(i18np("%1 app enabled", "%1 apps enabled", info.totalMarketplaceEnabled()));
        break;
    }
    case AppsCountInfoWidget::InfoType::PrivateApps: {
        mProgressBar->setMaximum(info.maxPrivateApps());
        mProgressBar->setValue(info.totalPrivateEnabled());
        mApplicationInfo->setText(u"%1/%2"_s.arg(QString::number(info.totalPrivateEnabled()), QString::number(info.maxPrivateApps())));
        mEnableAppsLabel->setText(i18np("%1 private app enabled", "%1 private apps enabled", info.totalPrivateEnabled()));
        break;
    }
    }
}

AppsCountInfoWidget::InfoType AppsCountInfoWidget::infotype() const
{
    return mInfotype;
}

void AppsCountInfoWidget::setInfotype(InfoType newInfotype)
{
    mInfotype = newInfotype;
}

#include "moc_appscountinfowidget.cpp"
