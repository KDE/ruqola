/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidget.h"

#include "configurepluginstreewidgetdelegate.h"
#include "room/plugins/plugintext.h"
#include "room/plugins/plugintool.h"
#include "room/textpluginmanager.h"
#include "room/toolspluginmanager.h"
#include "ruqolawidgets_debug.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageWidget>
#include <KTreeWidgetSearchLine>
#include <KTreeWidgetSearchLineWidget>
#include <QHeaderView>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ConfigurePluginsWidget::ConfigurePluginsWidget(QWidget *parent)
    : QWidget{parent}
    , mTreePluginWidget(new QTreeWidget(this))
    , mMessageWidget(new KMessageWidget(i18n("Restart is necessary for applying the changes."), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setVisible(false);
    mMessageWidget->setPosition(KMessageWidget::Header);

    mTreePluginWidget->setItemDelegate(new ConfigurePluginsTreeWidgetDelegate(this));
    mTreePluginWidget->setObjectName(u"mTreePluginWidget"_s);
    mTreePluginWidget->setSortingEnabled(true);
    mTreePluginWidget->sortItems(0, Qt::AscendingOrder);
    mTreePluginWidget->setHeaderHidden(true);
    mTreePluginWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTreePluginWidget->setColumnCount(2);
    mTreePluginWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setStretchLastSection(false);

    mSearchLineEdit = new KTreeWidgetSearchLineWidget(this, mTreePluginWidget);
    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->searchLine()->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::BottomEdge}));
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit->searchLine());

    connect(mTreePluginWidget, &QTreeWidget::itemChanged, this, &ConfigurePluginsWidget::slotItemChanged);

    mainLayout->addWidget(mMessageWidget);
    mainLayout->addWidget(mSearchLineEdit);
    mainLayout->addWidget(mTreePluginWidget);
}

ConfigurePluginsWidget::~ConfigurePluginsWidget() = default;

void ConfigurePluginsWidget::slotItemChanged([[maybe_unused]] QTreeWidgetItem *item, int column)
{
    if (mInitializeDone) {
        if (column == 0) {
            mMessageWidget->animatedShow();
        }
    }
}

void ConfigurePluginsWidget::save()
{
    savePlugins(ToolsPluginManager::self()->configGroupName(), ToolsPluginManager::self()->configPrefixSettingKey(), mPluginToolsItems);
    savePlugins(TextPluginManager::self()->configGroupName(), TextPluginManager::self()->configPrefixSettingKey(), mPluginTextItems);
}

void ConfigurePluginsWidget::load()
{
    initialize();
    initializeDone();
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

void ConfigurePluginsWidget::initializeDone()
{
    mInitializeDone = true;
}

void ConfigurePluginsWidget::savePlugins(const QString &groupName, const QString &prefixSettingKey, const QList<PluginItem *> &listItems)
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
#if HAVE_PLUGIN_UTILS_SUPPORT
    TextAddonsWidgets::PluginUtil::savePluginSettings({}, groupName, prefixSettingKey, enabledPlugins, disabledPlugins);
#else
    PluginUtils::savePluginSettings(groupName, prefixSettingKey, enabledPlugins, disabledPlugins);
#endif
}

#if HAVE_PLUGIN_UTILS_SUPPORT
void ConfigurePluginsWidget::fillTopItems(const QList<TextAddonsWidgets::PluginUtilData> &lst,
                                          const QString &topLevelItemName,
                                          const QString &groupName,
                                          const QString &prefixKey,
                                          QList<PluginItem *> &itemsList,
                                          const QString &configureGroupName,
                                          bool checkable)
#else
void ConfigurePluginsWidget::fillTopItems(const QList<PluginUtils::PluginUtilData> &lst,
                                          const QString &topLevelItemName,
                                          const QString &groupName,
                                          const QString &prefixKey,
                                          QList<PluginItem *> &itemsList,
                                          const QString &configureGroupName,
                                          bool checkable)
#endif
{
    itemsList.clear();
    if (!lst.isEmpty()) {
        auto topLevel = new QTreeWidgetItem(mTreePluginWidget, {topLevelItemName});
        topLevel->setFlags(topLevel->flags() & ~Qt::ItemIsSelectable);
#if HAVE_PLUGIN_UTILS_SUPPORT
        const TextAddonsWidgets::PluginUtil::PluginsStateList states = TextAddonsWidgets::PluginUtil::loadPluginSetting({}, groupName, prefixKey);
#else
        const PluginUtils::PluginsStateList states = PluginUtils::loadPluginSetting(groupName, prefixKey);
#endif
        for (const auto &data : lst) {
            auto subItem = new PluginItem(topLevel);
            subItem->setData(0, ConfigurePluginsWidget::PluginItemData::Description, data.mDescription);
            subItem->setText(0, data.mName);
            subItem->mIdentifier = data.mIdentifier;
            subItem->mEnableByDefault = data.mEnableByDefault;
            subItem->mHasConfigureSupport = data.mHasConfigureDialog;
            if (checkable) {
#if HAVE_PLUGIN_UTILS_SUPPORT
                const bool isPluginActivated = TextAddonsWidgets::PluginUtil::isPluginActivated(states.enabledPluginList,
                                                                                                states.disabledPluginList,
                                                                                                data.mEnableByDefault,
                                                                                                data.mIdentifier);
#else
                const bool isPluginActivated =
                    PluginUtils::isPluginActivated(states.enabledPluginList, states.disabledPluginList, data.mEnableByDefault, data.mIdentifier);
#endif
                subItem->mEnableFromUserSettings = isPluginActivated;
                subItem->setCheckState(0, isPluginActivated ? Qt::Checked : Qt::Unchecked);
            }
            if (data.mHasConfigureDialog) {
                auto but = new QToolButton(mTreePluginWidget);
                auto act = new QAction(but);
                const QStringList actData{configureGroupName, data.mIdentifier};
                act->setData(actData);
                but->setDefaultAction(act);
                but->setIcon(QIcon::fromTheme(u"configure"_s));
                but->setText(i18n("..."));
                but->setFixedWidth(28);
                but->setToolTip(i18nc("@info:tooltip", "Configure"));
                but->setAutoFillBackground(true);
                but->setEnabled(subItem->mHasConfigureSupport);
                mTreePluginWidget->setItemWidget(subItem, 1, but);
                connect(but, &QToolButton::triggered, this, &ConfigurePluginsWidget::slotConfigureClicked);
            }
            itemsList.append(subItem);
        }
    }
}

void ConfigurePluginsWidget::slotConfigureClicked(QAction *act)
{
    if (act) {
        const QStringList lst = act->data().toStringList();
        if (lst.count() == 2) {
            const QString &groupName = lst.at(0);
            const QString &identifier = lst.at(1);
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
    }
}

#include "moc_configurepluginswidget.cpp"
