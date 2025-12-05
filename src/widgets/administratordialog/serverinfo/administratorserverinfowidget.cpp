/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorserverinfowidget.h"

#include "connection.h"
#include "license/licensesinfojob.h"
#include "misc/statisticsjob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLineEditEventHandler>

#include <KIO/Global>
#include <KLocalizedString>
#include <KTreeWidgetSearchLine>
#include <KTreeWidgetSearchLineWidget>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QJsonObject>
#include <QMenu>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

AdministratorServerInfoWidget::AdministratorServerInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTreeWidget(new QTreeWidget(this))
    , mSearchLineWidget(new KTreeWidgetSearchLineWidget(this, mTreeWidget))
    , mRefreshButton(new QPushButton(QIcon::fromTheme(u"view-refresh"_s), {}, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);
    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    KLineEditEventHandler::catchReturnKey(mSearchLineWidget->searchLine());
    hboxLayout->addWidget(mSearchLineWidget);
    mRefreshButton->setObjectName(u"mRefreshButton"_s);
    mRefreshButton->setToolTip(i18nc("@info:tooltip", "Refresh"));
    hboxLayout->addWidget(mRefreshButton);
    connect(mRefreshButton, &QPushButton::clicked, this, &AdministratorServerInfoWidget::slotRefreshInfo);

    mTreeWidget->setObjectName(u"mTreeWidget"_s);
    mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTreeWidget, &QTreeWidget::customContextMenuRequested, this, &AdministratorServerInfoWidget::slotContextMenu);

    mainLayout->addWidget(mTreeWidget);
    mTreeWidget->header()->hide();
    mTreeWidget->setColumnCount(2);
}

AdministratorServerInfoWidget::~AdministratorServerInfoWidget() = default;

void AdministratorServerInfoWidget::slotContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeWidget->indexAt(pos);
    menu.addAction(QIcon::fromTheme(u"edit-copy"_s), i18nc("@action", "Copy"), this, [index]() {
        const QString currentValue = index.data().toString();
        QClipboard *clip = QApplication::clipboard();
        clip->setText(currentValue, QClipboard::Clipboard);
        clip->setText(currentValue, QClipboard::Selection);
    });
    menu.exec(mTreeWidget->viewport()->mapToGlobal(pos));
}

void AdministratorServerInfoWidget::slotRefreshInfo()
{
    loadStatisticInfo(true);
}

void AdministratorServerInfoWidget::initialize()
{
    loadLicensesInfo();
    loadStatisticInfo(false);
}

void AdministratorServerInfoWidget::loadLicensesInfo()
{
    auto job = new RocketChatRestApi::LicensesInfoJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::LicensesInfoJob::licensesInfoDone, this, [this](const QJsonObject &obj) {
        slotLicensesListDone(obj);
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start LicensesInfoJob job";
    }
}

void AdministratorServerInfoWidget::slotLicensesListDone(const QJsonObject &obj)
{
    LicensesManager m;
    m.parseLicenses(obj);
    const QStringList listLicences = m.licenses();
    auto licenseItem = new QTreeWidgetItem(mTreeWidget);
    licenseItem->setText(0, i18n("Licenses"));
    QFont f = licenseItem->font(0);
    f.setBold(true);
    licenseItem->setFont(0, f);
    createItemFromLicense(licenseItem, i18n("Omnichannel"), listLicences.contains("livechat-enterprise"_L1));
    createItemFromLicense(licenseItem, i18n("Auditing"), listLicences.contains(u"auditing"_s));
    createItemFromLicense(licenseItem, i18n("Canned Responses"), listLicences.contains(u"canned-responses"_s));
    createItemFromLicense(licenseItem, i18n("Engagement Dashboard"), listLicences.contains(u"engagement-dashboard"_s));
    createItemFromLicense(licenseItem, i18n("Read Receipts"), listLicences.contains(u"message-read-receipt"_s));
    // Load Statistic after loading licenses info
    loadStatisticInfo(false);
}

void AdministratorServerInfoWidget::createItemFromLicense(QTreeWidgetItem *licenseInfoItem, const QString &name, bool valid)
{
    auto item = new QTreeWidgetItem(licenseInfoItem);
    item->setText(0, name);
    item->setIcon(0, valid ? QIcon::fromTheme(u"dialog-ok-apply"_s) : QIcon::fromTheme(u"dialog-cancel"_s));
    licenseInfoItem->addChild(item);
}

void AdministratorServerInfoWidget::loadStatisticInfo(bool refresh)
{
    auto statisticJob = new RocketChatRestApi::StatisticsJob(this);
    statisticJob->setRefresh(refresh);
    mRocketChatAccount->restApi()->initializeRestApiJob(statisticJob);
    connect(statisticJob, &RocketChatRestApi::StatisticsJob::statisticDone, this, &AdministratorServerInfoWidget::slotStatisticDone);
    if (!statisticJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start StatisticsJob";
    }
}

void AdministratorServerInfoWidget::parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj)
{
    createItemFromStringValue(serverInfoItem, obj, i18n("Version"), u"version"_s);
    createItemFromStringValue(serverInfoItem, obj, i18n("Deployment ID"), u"uniqueId"_s);
}

void AdministratorServerInfoWidget::createItemFromStringValue(QTreeWidgetItem *parentItem,
                                                              const QJsonObject &obj,
                                                              const QString &label,
                                                              const QString &identifier)
{
    const QJsonValue objValue = obj.value(identifier);
    if (!objValue.isUndefined()) {
        auto item = new QTreeWidgetItem(parentItem);
        item->setText(0, label);
        item->setText(1, objValue.toString());
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    createItemFromIntValue(usageInfoItem, obj, i18n("Rocket.Chat App Users"), u"appUsers"_s);
    createItemDisplayFormat(usageInfoItem, obj, i18n("Total Uploads Size"), u"uploadsTotalSize"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Uploads"), u"uploadsTotal"_s);

    createItemFromIntValue(usageInfoItem, obj, i18n("Online Users"), u"onlineUsers"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Away Users"), u"awayUsers"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Offline Users"), u"offlineUsers"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Busy Users"), u"busyUsers"_s);

    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Users"), u"activeUsers"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Activated Guests"), u"activeGuests"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Deactivated Users"), u"nonActiveUsers"_s);

    createItemFromIntValue(usageInfoItem, obj, i18n("Total Channels"), u"totalChannels"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Rooms"), u"totalRooms"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Private Groups"), u"totalPrivateGroups"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Direct Message Rooms"), u"totalDirect"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Threads"), u"totalThreads"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Users"), u"totalUsers"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Discussions"), u"totalDiscussions"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Omnichannel Rooms"), u"totalLivechat"_s);

    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages"), u"totalMessages"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Channels"), u"totalChannelMessages"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Private Groups"), u"totalPrivateGroupMessages"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Messages in Direct Messages"), u"totalDirectMessages"_s);
    createItemFromIntValue(usageInfoItem, obj, i18n("Total Connected Users"), u"totalConnectedUsers"_s);
}

void AdministratorServerInfoWidget::createItemFromIntValue(QTreeWidgetItem *usageInfoItem,
                                                           const QJsonObject &obj,
                                                           const QString &label,
                                                           const QString &identifier)
{
    const QJsonValue totalDirectMessages = obj.value(identifier);
    if (!totalDirectMessages.isUndefined()) {
        auto item = new QTreeWidgetItem(usageInfoItem);
        item->setText(0, label);
        item->setText(1, QString::number(totalDirectMessages.toInt()));
        usageInfoItem->addChild(item);
    }
}

void AdministratorServerInfoWidget::createItemDisplayFormat(QTreeWidgetItem *parentItem,
                                                            const QJsonObject &obj,
                                                            const QString &label,
                                                            const QString &identifier)
{
    const QJsonValue objValue = obj.value(identifier);
    if (!objValue.isUndefined()) {
        auto item = new QTreeWidgetItem(parentItem);
        item->setText(0, label);
        item->setText(1, KIO::convertSize(objValue.toDouble()));
        item->addChild(item);
    }
}

void AdministratorServerInfoWidget::parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj)
{
    const QJsonObject runtimeObj = obj.value("os"_L1).toObject();
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Release"), u"release"_s);
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Type"), u"type"_s);
    createItemFromStringValue(runtimeInfoItem, runtimeObj, i18n("OS Platform"), u"platform"_s);
    createItemDisplayFormat(runtimeInfoItem, runtimeObj, i18n("OS Total Memory"), u"totalmem"_s);
    createItemDisplayFormat(runtimeInfoItem, runtimeObj, i18n("OS Free Memory"), u"freemem"_s);
}

void AdministratorServerInfoWidget::slotStatisticDone(const QJsonObject &obj)
{
    delete mServerInfoItem;
    mServerInfoItem = new QTreeWidgetItem(mTreeWidget);
    QFont f = mServerInfoItem->font(0);
    f.setBold(true);
    mServerInfoItem->setFont(0, f);
    mServerInfoItem->setText(0, i18n("Server Info"));
    parseServerInfo(mServerInfoItem, obj);

    delete mUsageInfoItem;
    mUsageInfoItem = new QTreeWidgetItem(mTreeWidget);
    mUsageInfoItem->setText(0, i18n("Usage"));
    mUsageInfoItem->setFont(0, f);
    parseUsageInfo(mUsageInfoItem, obj);

    delete mRuntimeInfoItem;
    mRuntimeInfoItem = new QTreeWidgetItem(mTreeWidget);
    mRuntimeInfoItem->setText(0, i18n("Runtime Environment"));
    mRuntimeInfoItem->setFont(0, f);
    parseRuntimeInfo(mRuntimeInfoItem, obj);
    mTreeWidget->expandAll();
    mTreeWidget->resizeColumnToContents(1);
    mTreeWidget->resizeColumnToContents(0);
}

#include "moc_administratorserverinfowidget.cpp"
