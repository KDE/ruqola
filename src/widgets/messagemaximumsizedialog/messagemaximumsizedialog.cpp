/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizedialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

MessageMaximumSizeDialog::MessageMaximumSizeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Prune Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

//    mPruneMessageWidget->setObjectName(QStringLiteral("mPruneMessageWidget"));
//    mainLayout->addWidget(mPruneMessageWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &MessageMaximumSizeDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &MessageMaximumSizeDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    // TODO connect(mPruneMessageWidget, &PruneMessagesWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

MessageMaximumSizeDialog::~MessageMaximumSizeDialog()
{

}
