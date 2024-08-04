/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleeditwidget.h"
#include "rolescopecombobox.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

RoleEditWidget::RoleEditWidget(QWidget *parent)
    : QWidget{parent}
    , mName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mTwoFactor(new QCheckBox(i18nc("@option:check", "Use Two Factor Authentication"), this))
    , mRoleScopeComboBox(new RoleScopeComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    KLineEditEventHandler::catchReturnKey(mName);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    KLineEditEventHandler::catchReturnKey(mDescription);
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
    mDescription->setReadOnly(info.mIsProtected);
    mRoleScopeComboBox->setEnabled(!info.mIsProtected);
}

RoleEditWidget::RoleEditDialogInfo RoleEditWidget::roleEditDialogInfo() const
{
    RoleEditWidget::RoleEditDialogInfo info;
    info.mName = mName->text();
    info.mDescription = mDescription->text();
    info.mTwoFactor = mTwoFactor->isChecked();
    info.mScope = mRoleScopeComboBox->currentData().toString();
    info.mIsProtected = !mRoleScopeComboBox->isEnabled();
    return info;
}

bool RoleEditWidget::RoleEditDialogInfo::operator==(const RoleEditDialogInfo &other) const
{
    return mName == other.mName && mDescription == other.mDescription && mScope == other.mScope && mTwoFactor == other.mTwoFactor
        && mIsProtected == other.mIsProtected;
}

QDebug operator<<(QDebug d, const RoleEditWidget::RoleEditDialogInfo &info)
{
    d << "mName : " << info.mName;
    d << "mDescription : " << info.mDescription;
    d << "mScope : " << info.mScope;
    d << "mTwoFactor : " << info.mTwoFactor;
    d << "mIsProtected" << info.mIsProtected;
    return d;
}

#include "moc_roleeditwidget.cpp"
