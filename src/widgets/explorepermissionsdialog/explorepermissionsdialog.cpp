/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionsdialog.h"

#include "explorepermissionswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
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

ExplorePermissionsDialog::~ExplorePermissionsDialog()
{
    writeConfig();
}

void ExplorePermissionsDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myExplorePermissionDialogConfigGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myExplorePermissionDialogConfigGroupName), 400, 300);
#endif
}

void ExplorePermissionsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExplorePermissionDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

void ExplorePermissionsDialog::setPermissions(const QList<Permission> &permissions)
{
    mExplorePermissionWidget->setPermissions(permissions);
}

void ExplorePermissionsDialog::setOWnRoles(const QStringList &roleStr)
{
    mExplorePermissionWidget->setOWnRoles(roleStr);
}

#include "moc_explorepermissionsdialog.cpp"
