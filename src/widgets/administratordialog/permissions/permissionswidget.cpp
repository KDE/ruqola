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

#include "permissionswidget.h"
#include "permissions/permissionslistalljob.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

PermissionsWidget::PermissionsWidget(QWidget *parent)
    : QWidget(parent)
    , mTreeView(new QTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);
}

PermissionsWidget::~PermissionsWidget()
{
}

void PermissionsWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto permissionsListAllJob = new RocketChatRestApi::PermissionsListAllJob(this);
    rcAccount->restApi()->initializeRestApiJob(permissionsListAllJob);
    connect(permissionsListAllJob, &RocketChatRestApi::PermissionsListAllJob::permissionListAllDone, this, &PermissionsWidget::slotPermissionListAllDone);
    if (!permissionsListAllJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ServerInfoJob";
    }
}

void PermissionsWidget::slotPermissionListAllDone(const QJsonObject &obj)
{
    qDebug() << "obj" << obj;
}
