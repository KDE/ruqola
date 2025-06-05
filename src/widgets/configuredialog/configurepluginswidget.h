/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeWidgetItem>
#include <QWidget>
class KTreeWidgetSearchLineWidget;
class QTreeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigurePluginsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurePluginsWidget(QWidget *parent = nullptr);
    ~ConfigurePluginsWidget() override;

    void save();
    void load();

private:
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

    KTreeWidgetSearchLineWidget *mSearchLineEdit = nullptr;
    QTreeWidget *const mTreePluginWidget;
};
