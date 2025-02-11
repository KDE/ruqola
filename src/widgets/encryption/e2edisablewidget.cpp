/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <KSeparator>
#include <QLabel>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
E2eDisableWidget::E2eDisableWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(
        i18n("Disabling E2EE will compromise the privacy of this groupe privé. Access to any encrypted content will be lost for all groupe privé members.\n"
             "Encryption can be re-enabled later. Proceed with caution.\n"
             "If no one is able to access the encrypted content you can reset encryption keys instead."),
        this);
    label->setObjectName("label"_L1);
    mainLayout->addWidget(label);

    auto separator = new KSeparator(this);
    separator->setObjectName("separator"_L1);
    mainLayout->addWidget(separator);
}

E2eDisableWidget::~E2eDisableWidget() = default;

#include "moc_e2edisablewidget.cpp"
