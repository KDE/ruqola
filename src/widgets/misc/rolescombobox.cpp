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
#include "rolescombobox.h"
#include "connection.h"
#include "misc/roleslistjob.h"
#include "model/rolesmodel.h"
#include "rocketchataccount.h"
#include "roleinfo.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

RolesComboBox::RolesComboBox(QWidget *parent)
    : QComboBox(parent)
    , mRolesModel(new RolesModel(this))
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setModel(mRolesModel);
}

RolesComboBox::~RolesComboBox()
{
}

void RolesComboBox::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    qDebug() << " void RolesComboBox::initialize()";
    auto job = new RocketChatRestApi::RolesListJob(this);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RolesListJob::rolesListDone, this, &RolesComboBox::slotRolesListDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RolesListJob job";
    }
}

void RolesComboBox::slotRolesListDone(const QJsonObject &obj)
{
    const QJsonArray array = obj[QLatin1String("roles")].toArray();
    QVector<RoleInfo> roleInfo;
    roleInfo.reserve(array.count());
    for (const QJsonValue &current : array) {
        const QJsonObject roleObject = current.toObject();
        RoleInfo info;
        info.parseRoleInfo(roleObject);
        roleInfo.append(info);
    }
    mRolesModel->setRoles(roleInfo);
}

void RolesComboBox::setRoles(const QStringList &lst)
{
    mRolesModel->setRolesSelected(lst);
}

QStringList RolesComboBox::roles() const
{
    return mRolesModel->rolesSelected();
}
