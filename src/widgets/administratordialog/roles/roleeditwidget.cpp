/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    mName->setReadOnly(true); // Not allow to modify name.
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
