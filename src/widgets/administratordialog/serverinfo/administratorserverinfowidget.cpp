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
    const QJsonValue activateUsers = obj.value(QLatin1String("activeUsers"));
    if (!activateUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Activated Users"));
        item->setText(1, QString::number(activateUsers.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue activateGuests = obj.value(QLatin1String("activeGuests"));
    if (!activateGuests.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Activated Guests"));
        item->setText(1, QString::number(activateGuests.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue uploadsTotal = obj.value(QLatin1String("uploadsTotal"));
    if (!uploadsTotal.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Uploads"));
        item->setText(1, QString::number(uploadsTotal.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalChannels = obj.value(QLatin1String("totalChannels"));
    if (!totalChannels.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Channels"));
        item->setText(1, QString::number(totalChannels.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalRooms = obj.value(QLatin1String("totalRooms"));
    if (!totalRooms.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Rooms"));
        item->setText(1, QString::number(totalRooms.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalPrivateGroups = obj.value(QLatin1String("totalPrivateGroups"));
    if (!totalPrivateGroups.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Private Groups"));
        item->setText(1, QString::number(totalPrivateGroups.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalThreads = obj.value(QLatin1String("totalThreads"));
    if (!totalThreads.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Threads"));
        item->setText(1, QString::number(totalPrivateGroups.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalUsers = obj.value(QLatin1String("totalUsers"));
    if (!totalUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Users"));
        item->setText(1, QString::number(totalPrivateGroups.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue appUsers = obj.value(QLatin1String("appUsers"));
    if (!appUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Rocket.Chat App Users"));
        item->setText(1, QString::number(totalPrivateGroups.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue awayUsers = obj.value(QLatin1String("awayUsers"));
    if (!awayUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Away Users"));
        item->setText(1, QString::number(awayUsers.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue uploadsTotalSize = obj.value(QLatin1String("uploadsTotalSize"));
    if (!uploadsTotalSize.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Uploads Size"));
        item->setText(1, QString::number(uploadsTotalSize.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue offlineUsers = obj.value(QLatin1String("offlineUsers"));
    if (!offlineUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Offline Users"));
        item->setText(1, QString::number(offlineUsers.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue onlineUsers = obj.value(QLatin1String("onlineUsers"));
    if (!onlineUsers.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Online Users"));
        item->setText(1, QString::number(onlineUsers.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalMessages = obj.value(QLatin1String("totalMessages"));
    if (!totalMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Messages"));
        item->setText(1, QString::number(totalMessages.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalChannelMessages = obj.value(QLatin1String("totalChannelMessages"));
    if (!totalChannelMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Messages in Channels"));
        item->setText(1, QString::number(totalChannelMessages.toInt()));
        usageInfoItem->addChild(item);
    }
    const QJsonValue totalPrivateGroupMessages = obj.value(QLatin1String("totalPrivateGroupMessages"));
    if (!totalPrivateGroupMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, i18n("Total Messages in Private Groups"));
        item->setText(1, QString::number(totalPrivateGroupMessages.toInt()));
        usageInfoItem->addChild(item);
    }
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
