/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatuswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/statuscombobox.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

ModifyStatusWidget::ModifyStatusWidget(QWidget *parent)
    : QWidget(parent)
    , mStatusCombobox(new StatusCombobox(this))
    , mStatusLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mStatusCombobox->setObjectName(u"mStatusCombobox"_s);

    mStatusCombobox->setUseOnlyStandardStatus();

    mStatusLineEdit->setObjectName(u"mStatusLineEdit"_s);
    mStatusLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mStatusLineEdit);

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
