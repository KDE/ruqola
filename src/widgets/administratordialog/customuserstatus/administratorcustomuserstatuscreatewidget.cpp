/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatewidget.h"
#include "misc/statuscombobox.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

AdministratorCustomUserStatusCreateWidget::AdministratorCustomUserStatusCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mStatusCombobox(new StatusCombobox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    mName->setClearButtonEnabled(true);
    mStatusCombobox->setObjectName(QStringLiteral("mStatusCombobox"));
    mStatusCombobox->setUseOnlyStandardStatus();

    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Status:"), mStatusCombobox);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
    connect(mStatusCombobox, &StatusCombobox::activated, this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
}

AdministratorCustomUserStatusCreateWidget::~AdministratorCustomUserStatusCreateWidget() = default;

void AdministratorCustomUserStatusCreateWidget::slotCheckValues()
{
    Q_EMIT updateOkButton(!mName->text().trimmed().isEmpty() && (mStatusCombobox->status() != User::PresenceStatus::Unknown));
}

AdministratorCustomUserStatusCreateWidget::UserStatusInfo AdministratorCustomUserStatusCreateWidget::userStatusInfo() const
{
    AdministratorCustomUserStatusCreateWidget::UserStatusInfo info;
    info.name = mName->text();
    info.statusType = mStatusCombobox->status();
    return info;
}

void AdministratorCustomUserStatusCreateWidget::setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info)
{
    mName->setText(info.name);
    mStatusCombobox->setStatus(info.statusType);
}
