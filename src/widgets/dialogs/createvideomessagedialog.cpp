/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagedialog.h"
#include "createvideomessagewidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

CreateVideoMessageDialog::CreateVideoMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateVideoMessageWidget(new CreateVideoMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateVideoMessageWidget->setObjectName(QStringLiteral("mCreateVideoMessageWidget"));
    mainLayout->addWidget(mCreateVideoMessageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateVideoMessageDialog::reject);
    mainLayout->addWidget(buttonBox);
}

CreateVideoMessageDialog::~CreateVideoMessageDialog() = default;
