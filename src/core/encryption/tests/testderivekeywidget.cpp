/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testderivekeywidget.h"
#include "encryption/encryptionutils.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>

TestDeriveKeyWidget::TestDeriveKeyWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
}

#include "moc_testderivekeywidget.cpp"
