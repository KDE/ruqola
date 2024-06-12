/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsdelegate.h"
#include "model/appsmarketplacemodel.h"

ApplicationsSettingsListView::ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent)
    : QTreeView(parent)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(this))
{
    mApplicationsSettingsListDelegate->setObjectName(QStringLiteral("mApplicationsSettingsListDelegate"));
    setItemDelegate(mApplicationsSettingsListDelegate);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    // setUniformRowHeights(true);
    setItemsExpandable(false);
    setIndentation(0);
    if (account) {
        setModel(account->appsMarketPlaceModel());
    }
}

ApplicationsSettingsListView::~ApplicationsSettingsListView() = default;

#include "moc_applicationssettingslistview.cpp"
