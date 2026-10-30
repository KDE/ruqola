/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categorydialog.h"
#include "category/categorywidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

CategoryDialog::CategoryDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , mCategoryWidget(new CategoryWidget(this))
{
    setWindowTitle(mode == Mode::Add ? i18nc("@title:window", "Add Category") : i18nc("@title:window", "Rename Category"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCategoryWidget->setObjectName(u"mCategoryWidget"_s);
    mainLayout->addWidget(mCategoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &CategoryDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &CategoryDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mCategoryWidget, &CategoryWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

CategoryDialog::~CategoryDialog() = default;

QString CategoryDialog::categoryName() const
{
    return mCategoryWidget->categoryName();
}

void CategoryDialog::setCategoryName(const QString &name)
{
    mCategoryWidget->setCategoryName(name);
}

#include "moc_categorydialog.cpp"
