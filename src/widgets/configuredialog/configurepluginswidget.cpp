/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidget.h"
#include "configurepluginstreewidgetdelegate.h"

#include <KLineEditEventHandler>
#include <KTreeWidgetSearchLine>
#include <KTreeWidgetSearchLineWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
ConfigurePluginsWidget::ConfigurePluginsWidget(QWidget *parent)
    : QWidget{parent}
    , mTreePluginWidget(new QTreeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mTreePluginWidget->setItemDelegate(new ConfigurePluginsTreeWidgetDelegate(this));
    mTreePluginWidget->setObjectName(QStringLiteral("mTreePluginWidget"));
    mTreePluginWidget->setSortingEnabled(true);
    mTreePluginWidget->sortItems(0, Qt::AscendingOrder);
    mTreePluginWidget->setHeaderHidden(true);
    mTreePluginWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTreePluginWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setStretchLastSection(false);

    mSearchLineEdit = new KTreeWidgetSearchLineWidget(this, mTreePluginWidget);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->searchLine()->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::BottomEdge}));
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit->searchLine());

    connect(mTreePluginWidget, &QTreeWidget::itemChanged, this, &ConfigurePluginsWidget::slotItemChanged);

    mainLayout->addWidget(mSearchLineEdit);
    mainLayout->addWidget(mTreePluginWidget);
}

ConfigurePluginsWidget::~ConfigurePluginsWidget() = default;

void ConfigurePluginsWidget::slotItemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item)
    if (mInitializeDone) {
        if (column == 0) {
            Q_EMIT changed();
        }
    }
}
void ConfigurePluginsWidget::save()
{
    // TODO
}

void ConfigurePluginsWidget::load()
{
    initializeDone();
    // TODO
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
    PluginUtils::savePluginSettings(groupName, prefixSettingKey, enabledPlugins, disabledPlugins);
}

void ConfigurePluginsWidget::fillTopItems(const QList<PluginUtils::PluginUtilData> &lst,
                                          const QString &topLevelItemName,
                                          const QString &groupName,
                                          const QString &prefixKey,
                                          QList<PluginItem *> &itemsList,
                                          const QString &configureGroupName,
                                          bool checkable)
{
    itemsList.clear();
    if (!lst.isEmpty()) {
        auto topLevel = new QTreeWidgetItem(mTreePluginWidget, {topLevelItemName});
        topLevel->setFlags(topLevel->flags() & ~Qt::ItemIsSelectable);
        const QPair<QStringList, QStringList> pair = PluginUtils::loadPluginSetting(groupName, prefixKey);
        for (const PluginUtils::PluginUtilData &data : lst) {
            auto subItem = new PluginItem(topLevel);
            subItem->setData(0, ConfigurePluginsWidget::PluginItemData::Description, data.mDescription);
            subItem->setText(0, data.mName);
            subItem->mIdentifier = data.mIdentifier;
            subItem->mEnableByDefault = data.mEnableByDefault;
            // subItem->mHasConfigureSupport = data.mHasConfigureDialog;
            if (checkable) {
                const bool isPluginActivated = PluginUtils::isPluginActivated(pair.first, pair.second, data.mEnableByDefault, data.mIdentifier);
                subItem->mEnableFromUserSettings = isPluginActivated;
                subItem->setCheckState(0, isPluginActivated ? Qt::Checked : Qt::Unchecked);
            }
            /*
            if (data.mHasConfigureDialog) {
                auto but = new QToolButton(mListWidget);
                auto act = new QAction(but);
                const QStringList actData{configureGroupName, data.mIdentifier};
                act->setData(actData);
                but->setDefaultAction(act);
                but->setIcon(QIcon::fromTheme(QStringLiteral("configure")));
                but->setText(i18n("..."));
                but->setFixedWidth(28);
                but->setToolTip(i18nc("@info:tooltip", "Configure"));
                but->setAutoFillBackground(true);
                but->setEnabled(subItem->mHasConfigureSupport);
                mListWidget->setItemWidget(subItem, 1, but);
                connect(but, &QToolButton::triggered, this, &ConfigurePluginsListWidget::slotConfigureClicked);
            }
            */
            itemsList.append(subItem);
        }
    }
}

#include "moc_configurepluginswidget.cpp"
