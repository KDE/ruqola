/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "categorywidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
CategoryWidget::CategoryWidget(QWidget *parent)
    : QWidget{parent}
    , mCategoryLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Category:"), this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    mCategoryLineEdit->setObjectName(u"mCategoryLineEdit"_s);
    mCategoryLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mCategoryLineEdit);
    label->setBuddy(mCategoryLineEdit);

    connect(mCategoryLineEdit, &QLineEdit::textChanged, this, &CategoryWidget::slotTextChanged);
}

CategoryWidget::~CategoryWidget() = default;

void CategoryWidget::slotTextChanged(const QString &str)
{
    Q_EMIT updateOkButton(!str.trimmed().isEmpty());
}

QString CategoryWidget::categoryName() const
{
    return mCategoryLineEdit->text().trimmed();
}

void CategoryWidget::setCategoryName(const QString &name)
{
    mCategoryLineEdit->setText(name);
}

#include "moc_categorywidget.cpp"
