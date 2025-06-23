/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatewidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mName->setObjectName(u"mName"_s);
    mName->setClearButtonEnabled(true);
    mStatusCombobox->setObjectName(u"mStatusCombobox"_s);
    mStatusCombobox->setUseOnlyStandardStatus();

    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Status:"), mStatusCombobox);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
    connect(mStatusCombobox, &StatusCombobox::activated, this, &AdministratorCustomUserStatusCreateWidget::slotCheckValues);
}

AdministratorCustomUserStatusCreateWidget::~AdministratorCustomUserStatusCreateWidget() = default;

void AdministratorCustomUserStatusCreateWidget::slotCheckValues()
{
    const QString currentName{mName->text().trimmed()};
    Q_EMIT updateOkButton(!currentName.isEmpty() && (mStatusCombobox->status() != User::PresenceStatus::Unknown)
                          && !mExistingCustomUserNames.contains(currentName));
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

void AdministratorCustomUserStatusCreateWidget::setExistingCustomUserNames(const QStringList &names)
{
    mExistingCustomUserNames = names;
}

#include "moc_administratorcustomuserstatuscreatewidget.cpp"
