/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidget.h"
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

    mainLayout->addWidget(mSearchLineEdit);
    mainLayout->addWidget(mTreePluginWidget);
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
