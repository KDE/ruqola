/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "permissionseditwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
PermissionsEditDialog::PermissionsEditDialog(QWidget *parent, const QString &permission)
    : QDialog(parent)
    , mPermissionsWidget(new PermissionsEditWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Edit \'%1\' Permission", permission));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mPermissionsWidget->setObjectName(u"mPermissionsWidget"_s);
    mainLayout->addWidget(mPermissionsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &PermissionsEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &PermissionsEditDialog::accept);
    resize(350, 50);
}

PermissionsEditDialog::~PermissionsEditDialog() = default;

void PermissionsEditDialog::setRoles(const QStringList &lst)
{
    mPermissionsWidget->setRoles(lst);
}

QStringList PermissionsEditDialog::roles() const
{
    return mPermissionsWidget->roles();
}

void PermissionsEditDialog::setRolesInfo(const QList<RoleInfo> &rolesinfo)
{
    mPermissionsWidget->setRolesInfo(rolesinfo);
}

#include "moc_permissionseditdialog.cpp"
