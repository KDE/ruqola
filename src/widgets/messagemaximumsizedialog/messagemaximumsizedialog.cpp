/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizedialog.h"
#include "messagemaximumsizewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

MessageMaximumSizeDialog::MessageMaximumSizeDialog(QWidget *parent)
    : QDialog(parent)
    , mMessageMaximumSizeWidget(new MessageMaximumSizeWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Convert Message As Attachment"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mMessageMaximumSizeWidget->setObjectName(QStringLiteral("mMessageMaximumSizeWidget"));
    mainLayout->addWidget(mMessageMaximumSizeWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &MessageMaximumSizeDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &MessageMaximumSizeDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mMessageMaximumSizeWidget, &MessageMaximumSizeWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

MessageMaximumSizeDialog::~MessageMaximumSizeDialog() = default;

QString MessageMaximumSizeDialog::description() const
{
    return mMessageMaximumSizeWidget->description();
}

QString MessageMaximumSizeDialog::fileName() const
{
    return mMessageMaximumSizeWidget->fileName();
}

#include "moc_messagemaximumsizedialog.cpp"
