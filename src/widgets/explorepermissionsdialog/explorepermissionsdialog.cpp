/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionsdialog.h"

#include "explorepermissionswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myExplorePermissionDialogConfigGroupName[] = "ExplorePermissionDialog";
}
using namespace Qt::Literals::StringLiterals;
ExplorePermissionsDialog::ExplorePermissionsDialog(QWidget *parent)
    : QDialog(parent)
    , mExplorePermissionWidget(new ExplorePermissionsWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Permissions"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mExplorePermissionWidget->setObjectName(u"mExplorePermissionWidget"_s);
    mainLayout->addWidget(mExplorePermissionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExplorePermissionsDialog::reject);

    readConfig();
}

ExplorePermissionsDialog::~ExplorePermissionsDialog() = default;

void ExplorePermissionsDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myExplorePermissionDialogConfigGroupName), QSize(400, 300));
}

void ExplorePermissionsDialog::setPermissions(const QList<Permission> &permissions)
{
    mExplorePermissionWidget->setPermissions(permissions);
}

void ExplorePermissionsDialog::setOwnRoles(const QStringList &roleStr)
{
    mExplorePermissionWidget->setOWnRoles(roleStr);
}

#include "moc_explorepermissionsdialog.cpp"
