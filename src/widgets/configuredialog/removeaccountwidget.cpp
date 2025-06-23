/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

RemoveAccountWidget::RemoveAccountWidget(QWidget *parent)
    : QWidget{parent}
    , mRemoveLogs(new QCheckBox(i18nc("@option:check", "Remove logs stored on disk"), this))
    , mInfo(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mInfo->setObjectName(u"mInfo"_s);
    mainLayout->addWidget(mInfo);

    mRemoveLogs->setObjectName(u"mRemoveLogs"_s);
    mainLayout->addWidget(mRemoveLogs);
}

RemoveAccountWidget::~RemoveAccountWidget() = default;

bool RemoveAccountWidget::removeLogs() const
{
    return mRemoveLogs->isChecked();
}

void RemoveAccountWidget::setAccountName(const QString &accountName)
{
    mInfo->setText(i18n("Do you want to remove this account \'%1\'?", accountName));
}

#include "moc_removeaccountwidget.cpp"
