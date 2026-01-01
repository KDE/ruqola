/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QJsonObject;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void
    createItemFromIntValue(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void
    createItemFromStringValue(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void
    createItemDisplayFormat(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLicensesListDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStatisticDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadStatisticInfo(bool refresh);
    LIBRUQOLAWIDGETS_NO_EXPORT void loadLicensesInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotContextMenu(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void createItemFromLicense(QTreeWidgetItem *licenseInfoItem, const QString &name, bool valid);
    QTreeWidget *const mTreeWidget;
    KTreeWidgetSearchLineWidget *const mSearchLineWidget;
    QPushButton *const mRefreshButton;
    QTreeWidgetItem *mServerInfoItem = nullptr;
    QTreeWidgetItem *mUsageInfoItem = nullptr;
    QTreeWidgetItem *mRuntimeInfoItem = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
