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

#include "roleeditwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "rolescopecombobox.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

RoleEditWidget::RoleEditWidget(QWidget *parent)
    : QWidget{parent}
    , mName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mTwoFactor(new QCheckBox(i18n("Use Two Factor Authentication"), this))
    , mRoleScopeComboBox(new RoleScopeComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    new LineEditCatchReturnKey(mName, this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    new LineEditCatchReturnKey(mDescription, this);
    mTwoFactor->setObjectName(QStringLiteral("mTwoFactor"));

    mRoleScopeComboBox->setObjectName(QStringLiteral("mRoleScopeComboBox"));

    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Description:"), mDescription);
    mainLayout->addWidget(mTwoFactor);
    mainLayout->addRow(i18n("Scope:"), mRoleScopeComboBox);
    connect(mName, &QLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
}

RoleEditWidget::~RoleEditWidget() = default;

void RoleEditWidget::setRoleEditDialogInfo(const RoleEditDialogInfo &info)
{
    mName->setText(info.mName);
    mDescription->setText(info.mDescription);
    mTwoFactor->setChecked(info.mTwoFactor);
    mRoleScopeComboBox->setCurrentIndex(mRoleScopeComboBox->findData(info.mScope));
    mName->setEnabled(false); // Not allow to modify name.
    mRoleScopeComboBox->setEnabled(!info.mIsProtected);
}

RoleEditWidget::RoleEditDialogInfo RoleEditWidget::roleEditDialogInfo() const
{
    RoleEditWidget::RoleEditDialogInfo info;
    info.mName = mName->text();
    info.mDescription = mDescription->text();
    info.mTwoFactor = mTwoFactor->isChecked();
    info.mScope = mRoleScopeComboBox->currentData().toString();
    return info;
}
