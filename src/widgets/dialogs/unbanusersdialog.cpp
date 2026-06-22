/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanusersdialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
UnbanUsersDialog::UnbanUsersDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Upload File"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    // mUploadFileWidget->setObjectName(u"mUploadFileWidget"_s);
    // mainLayout->addWidget(mUploadFileWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &UnbanUsersDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UnbanUsersDialog::reject);
    mainLayout->addWidget(buttonBox);
}

UnbanUsersDialog::~UnbanUsersDialog() = default;
