/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorcustomuserstatuscreatewidget.h"
#include "misc/statuscombobox.h"
#include "model/statusmodel.h"
#include "model/statusmodelfilterproxymodel.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

AdministratorCustomUserStatusCreateWidget::AdministratorCustomUserStatusCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mStatusCombobox(new StatusCombobox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    mStatusCombobox->setObjectName(QStringLiteral("mStatusCombobox"));
    auto *statusProxyModel = new StatusModelFilterProxyModel(this);
    statusProxyModel->setUseOnlyStandardStatus(true);
    statusProxyModel->setSourceModel(new StatusModel(this));
    mStatusCombobox->setModel(statusProxyModel);

    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Status:"), mStatusCombobox);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
    connect(mStatusCombobox, QOverload<int>::of(&StatusCombobox::activated), this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
}

AdministratorCustomUserStatusCreateWidget::~AdministratorCustomUserStatusCreateWidget()
{
}

void AdministratorCustomUserStatusCreateWidget::slotCheckValues()
{
    Q_EMIT updateOkButton(!mName->text().trimmed().isEmpty() && (mStatusCombobox->status() != User::PresenceStatus::Unknown));
}

AdministratorCustomUserStatusCreateWidget::UserStatusInfo AdministratorCustomUserStatusCreateWidget::userStatusInfo() const
{
    AdministratorCustomUserStatusCreateWidget::UserStatusInfo info;
    info.name = mName->text();
    info.statusType = mStatusCombobox->status();
    return info;
}

void AdministratorCustomUserStatusCreateWidget::setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info)
{
    mName->setText(info.name);
    mStatusCombobox->setStatus(info.statusType);
}
