/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "administratorserverinfowidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "restapirequest.h"
#include "misc/statisticsjob.h"
#include "ruqolawidgets_debug.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QTreeWidget>
#include <QHeaderView>

AdministratorServerInfoWidget::AdministratorServerInfoWidget(QWidget *parent)
    : QWidget(parent)
    , mTreeWidget(new QTreeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTreeWidget->setObjectName(QStringLiteral("mTreeWidget"));
    mainLayout->addWidget(mTreeWidget);
    mTreeWidget->setRootIsDecorated(false);
    mTreeWidget->header()->hide();

    initialize();
}

AdministratorServerInfoWidget::~AdministratorServerInfoWidget()
{
}

void AdministratorServerInfoWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto statisticJob = new RocketChatRestApi::StatisticsJob(this);
    rcAccount->restApi()->initializeRestApiJob(statisticJob);
    connect(statisticJob, &RocketChatRestApi::StatisticsJob::statisticDone,
            this, &AdministratorServerInfoWidget::slotStatisticDone);
    if (!statisticJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start StatisticsJob";
    }
}

void AdministratorServerInfoWidget::parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj)
{
    //TODO
}

void AdministratorServerInfoWidget::parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    //TODO
}

void AdministratorServerInfoWidget::parseRuntimeInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    //TODO
}

void AdministratorServerInfoWidget::parseCommitInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    //TODO
}

void AdministratorServerInfoWidget::parseBuildInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    //TODO
}

void AdministratorServerInfoWidget::slotStatisticDone(const QJsonObject &obj)
{
    qDebug() << "AdministratorServerInfoWidget::slotStatisticDone " << obj;
    QTreeWidgetItem *serverInfoItem = new QTreeWidgetItem(mTreeWidget);
    serverInfoItem->setText(0, i18n("Server Info"));
    parseServerInfo(serverInfoItem, obj);

    QTreeWidgetItem *usageInfoItem = new QTreeWidgetItem(mTreeWidget);
    usageInfoItem->setText(0, i18n("Usage"));
    parseUsageInfo(usageInfoItem, obj);

    QTreeWidgetItem *runtimeInfoItem = new QTreeWidgetItem(mTreeWidget);
    runtimeInfoItem->setText(0, i18n("Runtime Environment"));
    parseRuntimeInfo(runtimeInfoItem, obj);

    QTreeWidgetItem *commitItem = new QTreeWidgetItem(mTreeWidget);
    commitItem->setText(0, i18n("Commit"));
    parseCommitInfo(commitItem, obj);

    QTreeWidgetItem *buildItem = new QTreeWidgetItem(mTreeWidget);
    buildItem->setText(0, i18n("Build Environment"));
    parseBuildInfo(buildItem, obj);
}
