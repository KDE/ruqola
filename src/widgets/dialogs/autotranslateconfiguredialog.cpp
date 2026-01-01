/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfiguredialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mAutoTranslateConfigureWidget->setObjectName(u"mAutoTranslateConfigureWidget"_s);
    mainLayout->addWidget(mAutoTranslateConfigureWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AutoTranslateConfigureDialog::reject);
}

AutoTranslateConfigureDialog::~AutoTranslateConfigureDialog() = default;

void AutoTranslateConfigureDialog::setRoom(Room *room)
{
    mAutoTranslateConfigureWidget->setRoom(room);
}

#include "moc_autotranslateconfiguredialog.cpp"
