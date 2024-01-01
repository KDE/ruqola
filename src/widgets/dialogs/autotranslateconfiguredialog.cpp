/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfiguredialog.h"
#include "autotranslateconfigurewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

AutoTranslateConfigureDialog::AutoTranslateConfigureDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAutoTranslateConfigureWidget(new AutoTranslateConfigureWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Configure Auto-Translate"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAutoTranslateConfigureWidget->setObjectName(QStringLiteral("mAutoTranslateConfigureWidget"));
    mainLayout->addWidget(mAutoTranslateConfigureWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AutoTranslateConfigureDialog::reject);
}

AutoTranslateConfigureDialog::~AutoTranslateConfigureDialog() = default;

void AutoTranslateConfigureDialog::setRoom(Room *room)
{
    mAutoTranslateConfigureWidget->setRoom(room);
}

#include "moc_autotranslateconfiguredialog.cpp"
