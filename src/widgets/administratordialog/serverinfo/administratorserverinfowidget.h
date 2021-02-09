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

#ifndef ADMINISTRATORSERVERINFOWIDGET_H
#define ADMINISTRATORSERVERINFOWIDGET_H

#include "libruqolawidgets_private_export.h"
#include "serverinfo.h"
#include <QJsonObject>
#include <QWidget>
class QTreeWidget;
class QTreeWidgetItem;
class KTreeWidgetSearchLineWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorServerInfoWidget(QWidget *parent = nullptr);
    ~AdministratorServerInfoWidget() override;

private:
    void parseServerInfo(QTreeWidgetItem *serverInfoItem, const QJsonObject &obj);
    void parseUsageInfo(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj);
    void parseRuntimeInfo(QTreeWidgetItem *runtimeInfoItem, const QJsonObject &obj);
    void parseCommitInfo(QTreeWidgetItem *commitInfoItem);
    void parseBuildInfo(QTreeWidgetItem *buildInfoItem);
    void createItemFromIntValue(QTreeWidgetItem *usageInfoItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void createItemFromStringValue(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void slotServerInfoDone(const QString &versionInfo, const QJsonObject &obj);
    void createItemFromLongValue(QTreeWidgetItem *parentItem, const QJsonObject &obj, const QString &label, const QString &identifier);
    void initialize();
    void slotStatisticDone(const QJsonObject &obj);
    void slotRefreshInfo();
    void loadStatisticInfo(bool refresh);
    void loadServerInfo();
    void slotContextMenu(const QPoint &pos);
    QTreeWidget *const mTreeWidget;
    KTreeWidgetSearchLineWidget *const mSearchLineWidget;
    QPushButton *const mRefreshButton;
    ServerInfo mServerInfo;
    QTreeWidgetItem *mServerInfoItem = nullptr;
    QTreeWidgetItem *mUsageInfoItem = nullptr;
    QTreeWidgetItem *mRuntimeInfoItem = nullptr;
};

#endif // ADMINISTRATORSERVERINFOWIDGET_H
