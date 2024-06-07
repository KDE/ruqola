/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QVBoxLayout>

#include "apps/appcategoriesjob.h"
#include "apps/appcountjob.h"
#include "apps/appfeaturedappsjob.h"
#include "apps/appmarketplacejob.h"

ApplicationsSettingsWidget::ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mCurrentRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

#if 0
    auto job = new RocketChatRestApi::AppCountJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppCountJob::appCountDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppCountJob";
    }
#endif
#if 0
    auto job = new RocketChatRestApi::AppFeaturedAppsJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppFeaturedAppsJob::appFeaturedAppsDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppCountJob";
    }
#endif
#if 0
    auto job = new RocketChatRestApi::AppCategoriesJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppCategoriesJob::appCategoriesDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appCategories";
    }
#endif
#if 1
    auto job = new RocketChatRestApi::AppMarketPlaceJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppMarketPlaceJob::appMarketPlaceDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppMarketPlaceJob";
    }
#endif
}

ApplicationsSettingsWidget::~ApplicationsSettingsWidget() = default;

#include "moc_applicationssettingswidget.cpp"
