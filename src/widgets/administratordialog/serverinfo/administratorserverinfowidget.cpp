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
    mTreeWidget->header()->hide();
    mTreeWidget->setColumnCount(2);
    //TODO fix column size

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
    const QJsonValue version = obj.value(QLatin1String("version"));
    if (!version.isUndefined()) {
        auto item = new QTreeWidgetItem(serverInfoItem);
        item->setText(0, i18n("Version"));
        item->setText(1, version.toString());
        serverInfoItem->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Users"), QStringLiteral("activeUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Guests"), QStringLiteral("activeGuests"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Uploads"), QStringLiteral("uploadsTotal"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Channels"), QStringLiteral("totalChannels"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Rooms"), QStringLiteral("totalRooms"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Private Groups"), QStringLiteral("totalPrivateGroups"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Threads"), QStringLiteral("totalThreads"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Users"), QStringLiteral("totalUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Rocket.Chat App Users"), QStringLiteral("appUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Away Users"), QStringLiteral("awayUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Uploads Size"), QStringLiteral("uploadsTotalSize"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Offline Users"), QStringLiteral("offlineUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Online Users"), QStringLiteral("onlineUsers"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages"), QStringLiteral("totalMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Channels"), QStringLiteral("totalChannelMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Private Groups"), QStringLiteral("totalPrivateGroupMessages"));
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Direct Messages"), QStringLiteral("totalDirectMessages"));
}

void AdministratorServerInfoWidget::createItemFromIntValue(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj, const QString &label, const QString &identifier)
{
    const QJsonValue totalDirectMessages = obj.value(identifier);
    if (!totalDirectMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, label);
        item->setText(1, QString::number(totalDirectMessages.toInt()));
        usageInfoItem->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj)
{
    const QJsonValue release = obj.value(QLatin1String("release"));
    if (!release.isUndefined()) {
        auto item = new QTreeWidgetItem(runtimeInfoItem);
        item->setText(0, i18n("OS Release"));
        item->setText(1, release.toString());
        runtimeInfoItem->addChild(item);
    }
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
    mTreeWidget->resizeColumnToContents(1);
    mTreeWidget->resizeColumnToContents(0);
    mTreeWidget->expandAll();
}
