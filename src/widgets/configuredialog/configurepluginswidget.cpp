/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidget.h"

#include "room/plugins/plugintext.h"
#include "room/plugins/plugintool.h"
#include "room/textpluginmanager.h"
#include "room/toolspluginmanager.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QHeaderView>
using namespace Qt::Literals::StringLiterals;
ConfigurePluginsWidget::ConfigurePluginsWidget(QWidget *parent)
    : TextAddonsWidgets::ConfigurePluginsWidget{parent}
{
    connect(this, &ConfigurePluginsWidget::configureClicked, this, &ConfigurePluginsWidget::slotConfigureClicked);
}

ConfigurePluginsWidget::~ConfigurePluginsWidget() = default;

void ConfigurePluginsWidget::save(const QString &configFile)
{
    savePlugins(configFile, ToolsPluginManager::self()->configGroupName(), ToolsPluginManager::self()->configPrefixSettingKey(), mPluginToolsItems);
    savePlugins(configFile, TextPluginManager::self()->configGroupName(), TextPluginManager::self()->configPrefixSettingKey(), mPluginTextItems);
}

void ConfigurePluginsWidget::restoreToDefaults()
{
    // TODO
}

QString toolsPluginGroupName()
{
    return u"pluginToolsPluginGroupName"_s;
}

QString textPluginGroupName()
{
    return u"pluginTextPluginGroupName"_s;
}

void ConfigurePluginsWidget::initialize()
{
    mTreePluginWidget->clear();
    // Load Tools Plugins
    fillTopItems(ToolsPluginManager::self()->pluginDataList(),
                 i18n("Tools Plugins"),
                 ToolsPluginManager::self()->configGroupName(),
                 ToolsPluginManager::self()->configPrefixSettingKey(),
                 mPluginToolsItems,
                 toolsPluginGroupName());

    fillTopItems(TextPluginManager::self()->pluginDataList(),
                 i18n("Text Plugins"),
                 TextPluginManager::self()->configGroupName(),
                 TextPluginManager::self()->configPrefixSettingKey(),
                 mPluginTextItems,
                 textPluginGroupName());
    mTreePluginWidget->expandAll();
}

void ConfigurePluginsWidget::savePlugins(const QString &configFile,
                                         const QString &groupName,
                                         const QString &prefixSettingKey,
                                         const QList<PluginItem *> &listItems)
{
    if (listItems.isEmpty()) {
        return;
    }
    QStringList enabledPlugins;
    QStringList disabledPlugins;
    for (PluginItem *item : listItems) {
        if (item->checkState(0) == Qt::Checked) {
            enabledPlugins << item->mIdentifier;
        } else {
            disabledPlugins << item->mIdentifier;
        }
    }
    TextAddonsWidgets::PluginUtil::savePluginSettings(configFile, groupName, prefixSettingKey, enabledPlugins, disabledPlugins);
}

void ConfigurePluginsWidget::slotConfigureClicked(const QString &groupName, const QString &identifier)
{
    if (!groupName.isEmpty() && !identifier.isEmpty()) {
        if (groupName == toolsPluginGroupName()) {
            const auto p = ToolsPluginManager::self()->pluginFromIdentifier(identifier);
            if (p) {
                p->showConfigureDialog(this);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find tool plugin " << identifier;
            }
        } else if (groupName == textPluginGroupName()) {
            const auto p = TextPluginManager::self()->pluginFromIdentifier(identifier);
            if (p) {
                p->showConfigureDialog(this);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find text plugin " << identifier;
            }
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "plugin group name not supported " << groupName;
        }
    }
}

#include "moc_configurepluginswidget.cpp"
