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
class KMessageWidget;
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
    void restoreToDefaults();

Q_SIGNALS:
    void changed();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotItemChanged(QTreeWidgetItem *item, int column);

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
    LIBRUQOLAWIDGETS_NO_EXPORT void savePlugins(const QString &groupName, const QString &prefixSettingKey, const QList<PluginItem *> &listItems);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureClicked(QAction *act);

    KTreeWidgetSearchLineWidget *mSearchLineEdit = nullptr;
    QTreeWidget *const mTreePluginWidget;
    KMessageWidget *const mMessageWidget;
    bool mInitializeDone = false;
    LIBRUQOLAWIDGETS_NO_EXPORT void fillTopItems(const QList<PluginUtils::PluginUtilData> &lst,
                                                 const QString &topLevelItemName,
                                                 const QString &groupName,
                                                 const QString &prefixKey,
                                                 QList<PluginItem *> &itemsList,
                                                 const QString &configureGroupName,
                                                 bool checkable = true);
    QList<PluginItem *> mPluginToolsItems;
    QList<PluginItem *> mPluginTextItems;
};
