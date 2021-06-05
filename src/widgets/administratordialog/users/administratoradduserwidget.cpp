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
#include "administratoradduserwidget.h"
#include "misc/roleslistjob.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorAddUserWidget::AdministratorAddUserWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
{
    mName->setObjectName(QStringLiteral("mName"));
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mEmail->setObjectName(QStringLiteral("mEmail"));
    listRoles();
}

AdministratorAddUserWidget::~AdministratorAddUserWidget()
{
}

RocketChatRestApi::UsersCreateJob::CreateInfo AdministratorAddUserWidget::createInfo() const
{
    return {};
}

void AdministratorAddUserWidget::listRoles()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::RolesListJob(this);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RolesListJob::rolesListDone, this, &AdministratorAddUserWidget::slotRolesListDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SetUserActiveStatusJob job";
    }
}

void AdministratorAddUserWidget::slotRolesListDone(const QJsonObject &obj)
{
    qDebug() << "obj " << obj;
}
