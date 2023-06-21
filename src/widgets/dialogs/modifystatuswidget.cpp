/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatuswidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/statuscombobox.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

ModifyStatusWidget::ModifyStatusWidget(QWidget *parent)
    : QWidget(parent)
    , mStatusCombobox(new StatusCombobox(this))
    , mStatusLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mStatusCombobox->setObjectName(QStringLiteral("mStatusCombobox"));

    mStatusCombobox->setUseOnlyStandardStatus();

    mStatusLineEdit->setObjectName(QStringLiteral("mStatusLineEdit"));
    mStatusLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mStatusLineEdit, this);

    mainLayout->addRow(i18n("Message Status:"), mStatusLineEdit);
    mainLayout->addRow(i18n("Status:"), mStatusCombobox);
}

ModifyStatusWidget::~ModifyStatusWidget() = default;

void ModifyStatusWidget::setMessageStatus(const QString &statusStr)
{
    mStatusLineEdit->setText(statusStr);
}

QString ModifyStatusWidget::messageStatus() const
{
    return mStatusLineEdit->text();
}

User::PresenceStatus ModifyStatusWidget::status() const
{
    return mStatusCombobox->status();
}

void ModifyStatusWidget::setStatus(User::PresenceStatus status)
{
    mStatusCombobox->setStatus(status);
}

#include "moc_modifystatuswidget.cpp"
