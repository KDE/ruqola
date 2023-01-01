/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolesmodel.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
RolesModel::RolesModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

RolesModel::~RolesModel() = default;

void RolesModel::createItem(const QString &displayStr, const QString &identifier)
{
    auto item = new QStandardItem(displayStr);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(identifier, Identifier);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    item->setToolTip(displayStr);
    appendRow(item);
}

void RolesModel::setRoles(const QVector<RoleInfo> &newRoles)
{
    auto item = new QStandardItem(i18n("Roles"));
    item->setSelectable(false);
    appendRow(item);
    for (const RoleInfo &info : newRoles) {
        createItem(info.name(), info.identifier());
    }
    fillRoleSelected();
}

QStringList RolesModel::rolesSelected() const
{
    QStringList lst;
    const int rowCountNb = rowCount();
    // First one is not a message type
    for (int i = 0; i < rowCountNb; i++) {
        QStandardItem *itemModel = item(i);
        if (itemModel) {
            if (itemModel->isCheckable() && itemModel->checkState() == Qt::Checked) {
                lst.append(itemModel->data(Identifier).toString());
            }
        }
    }
    return lst;
}

void RolesModel::setRolesSelected(const QStringList &newRolesSelected)
{
    mRolesSelected = newRolesSelected;
    fillRoleSelected();
}

void RolesModel::fillRoleSelected()
{
    const int rowCountNb = rowCount();
    // First one is not a message type
    QStringList copyList = mRolesSelected;
    for (int i = 0; i < rowCountNb; i++) {
        QStandardItem *itemModel = item(i);
        if (itemModel) {
            for (const QString &s : std::as_const(mRolesSelected)) {
                if (itemModel->data(Identifier).toString() == s) {
                    itemModel->setCheckState(Qt::Checked);
                    copyList.removeAll(s);
                    break;
                }
            }
        }
    }
    // Remove empty string otherwise it will signal it.
    copyList.removeAll(QLatin1String(""));
    if (!copyList.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "role is not implemented here " << copyList;
    }
}
