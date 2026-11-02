/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managecategoriesdialog.h"
#include "managecategorieswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
using namespace Qt::Literals::StringLiterals;
namespace
{
const char myManageCategoriesDialogGroupName[] = "ManageCategoriesDialog";
}
ManageCategoriesDialog::ManageCategoriesDialog(QWidget *parent)
    : QDialog(parent)
    , mManageCategoriesWidget(new ManageCategoriesWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Manage Categories"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mManageCategoriesWidget->setObjectName(u"mManageCategoriesWidget"_s);
    mainLayout->addWidget(mManageCategoriesWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ManageCategoriesDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ManageCategoriesDialog::accept);

    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myManageCategoriesDialogGroupName), QSize(400, 300));
}

ManageCategoriesDialog::~ManageCategoriesDialog() = default;

void ManageCategoriesDialog::setCategories(const QList<Category> &categories)
{
    mManageCategoriesWidget->setCategories(categories);
}

QList<Category> ManageCategoriesDialog::categories() const
{
    return mManageCategoriesWidget->categories();
}

#include "moc_managecategoriesdialog.cpp"
