/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationspermissionwidget.h"
#include <KLocalizedString>
#include <QTextEdit>
#include <QVBoxLayout>
ApplicationsPermissionWidget::ApplicationsPermissionWidget(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTextEdit->setObjectName(QStringLiteral("mTextEdit"));
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

ApplicationsPermissionWidget::~ApplicationsPermissionWidget() = default;

void ApplicationsPermissionWidget::setApplicationPermission(const QString &desc)
{
    if (desc.isEmpty()) {
        mTextEdit->setHtml(QStringLiteral("<i>%1</i>").arg(i18n("No permissions information found.")));
    } else {
        mTextEdit->setHtml(desc);
    }
}

#include "moc_applicationspermissionwidget.cpp"
