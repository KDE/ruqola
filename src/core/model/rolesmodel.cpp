/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "rolesmodel.h"
#include "ruqola_debug.h"
RolesModel::RolesModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

RolesModel::~RolesModel()
{
}

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
