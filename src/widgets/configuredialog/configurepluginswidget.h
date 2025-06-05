/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room/plugins/pluginutils.h"
#include <QTreeWidgetItem>
#include <QWidget>
class KTreeWidgetSearchLineWidget;
class QTreeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigurePluginsWidget : public QWidget
{
    Q_OBJECT
public:
    enum PluginItemData {
        Description = Qt::UserRole + 1,
    };
    explicit ConfigurePluginsWidget(QWidget *parent = nullptr);
    ~ConfigurePluginsWidget() override;

    void save();
    void load();

Q_SIGNALS:
    void changed();

private:
    void initializeDone();
    void slotItemChanged(QTreeWidgetItem *item, int column);

    class PluginItem : public QTreeWidgetItem
    {
    public:
        explicit PluginItem(QTreeWidgetItem *parent)
            : QTreeWidgetItem(parent)
        {
        }

        QString mIdentifier;
        bool mEnableByDefault = false;
        bool mHasConfigureSupport = false;
        bool mEnableFromUserSettings = false;
    };
    void savePlugins(const QString &groupName, const QString &prefixSettingKey, const QList<PluginItem *> &listItems);

    KTreeWidgetSearchLineWidget *mSearchLineEdit = nullptr;
    QTreeWidget *const mTreePluginWidget;
    bool mInitializeDone = false;
    void fillTopItems(const QList<PluginUtils::PluginUtilData> &lst,
                      const QString &topLevelItemName,
                      const QString &groupName,
                      const QString &prefixKey,
                      QList<PluginItem *> &itemsList,
                      const QString &configureGroupName,
                      bool checkable);
};
