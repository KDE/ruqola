/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "serverinfo.h"
#include <QJsonObject>
#include <QWidget>
class QTreeWidget;
class QTreeWidgetItem;
class KTreeWidgetSearchLineWidget;
class QPushButton;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorServerInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorServerInfoWidget() override;
    void initialize();

private:
    void parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj);
    void parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj);
    void parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj);
    void parseCommitInfo(QTreeWidgetItem *commitInfoItem);
    void parseBuildInfo(QTreeWidgetItem *buildInfoItem);
    void createItemFromIntValue(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void createItemFromStringValue(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void slotServerInfoDone(const QString &versionInfo, const QJsonObject &obj);
    void createItemDisplayFormat(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void slotLicensesListDone(const QJsonObject &obj);
    void slotStatisticDone(const QJsonObject &obj);
    void slotRefreshInfo();
    void loadStatisticInfo(bool refresh);
    void loadServerInfo();
    void loadLicensesInfo();
    void fillLicenses(const QJsonObject &obj);
    void slotContextMenu(const QPoint &pos);
    void createItemFromLicense(QTreeWidgetItem *licenseInfoItem, const QString &name, bool valid);
    QTreeWidget *const mTreeWidget;
    KTreeWidgetSearchLineWidget *const mSearchLineWidget;
    QPushButton *const mRefreshButton;
    ServerInfo mServerInfo;
    QTreeWidgetItem *mServerInfoItem = nullptr;
    QTreeWidgetItem *mUsageInfoItem = nullptr;
    QTreeWidgetItem *mRuntimeInfoItem = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
