/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <TextAddonsWidgets/ConfigurePluginsWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigurePluginsWidget : public TextAddonsWidgets::ConfigurePluginsWidget
{
    Q_OBJECT
public:
    explicit ConfigurePluginsWidget(QWidget *parent = nullptr);
    ~ConfigurePluginsWidget() override;

    void save() override;
    void restoreToDefaults();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize() override;
    LIBRUQOLAWIDGETS_NO_EXPORT void savePlugins(const QString &groupName, const QString &prefixSettingKey, const QList<PluginItem *> &listItems);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureClicked(const QString &groupName, const QString &identifier);

    QList<PluginItem *> mPluginToolsItems;
    QList<PluginItem *> mPluginTextItems;
};
