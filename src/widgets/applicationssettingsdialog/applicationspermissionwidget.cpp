/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationspermissionwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QTextEdit>
#include <QVBoxLayout>
ApplicationsPermissionWidget::ApplicationsPermissionWidget(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTextEdit->setObjectName(u"mTextEdit"_s);
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

ApplicationsPermissionWidget::~ApplicationsPermissionWidget() = default;

void ApplicationsPermissionWidget::setApplicationPermission(const QString &desc)
{
    if (desc.isEmpty()) {
        mTextEdit->setHtml(u"<i>%1</i>"_s.arg(i18n("No permissions information found.")));
    } else {
        mTextEdit->setHtml(desc);
    }
}

#include "moc_applicationspermissionwidget.cpp"
