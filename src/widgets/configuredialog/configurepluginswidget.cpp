/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidget.h"
#include <KLineEditEventHandler>
#include <QHeaderView>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
ConfigurePluginsWidget::ConfigurePluginsWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mTreePluginWidget(new QTreeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);

    mTreePluginWidget->setObjectName(QStringLiteral("mTreePluginWidget"));
    mainLayout->addWidget(mTreePluginWidget);
    mTreePluginWidget->setSortingEnabled(true);
    mTreePluginWidget->sortItems(0, Qt::AscendingOrder);
    mTreePluginWidget->setHeaderHidden(true);
    mTreePluginWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTreePluginWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    mTreePluginWidget->header()->setStretchLastSection(false);

    /*
        mTreeWidgetSearchLineEdit = new KTreeWidgetSearchLineWidget(this, mListWidget);
    mTreeWidgetSearchLineEdit->setObjectName("mTreeWidgetSearchLineEdit"_L1);
    mTreeWidgetSearchLineEdit->searchLine()->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::BottomEdge}));
    KLineEditEventHandler::catchReturnKey(mTreeWidgetSearchLineEdit->searchLine());
    */
}

ConfigurePluginsWidget::~ConfigurePluginsWidget() = default;

void ConfigurePluginsWidget::save()
{
    // TODO
}

void ConfigurePluginsWidget::load()
{
    // TODO
}

#include "moc_configurepluginswidget.cpp"
