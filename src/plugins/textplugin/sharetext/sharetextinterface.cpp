/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sharetextinterface.h"

#include "purposemenuwidget.h"

#include <QMenu>

using namespace Qt::Literals::StringLiterals;
ShareTextInterface::ShareTextInterface(QWidget *parentWidget, QObject *parent)
    : PluginTextInterface(parentWidget, parent)
    , mPurposeMenuWidget(new PurposeMenuWidget(this))
{
    connect(mPurposeMenuWidget, &PurposeMenuWidget::errorMessage, this, &ShareTextInterface::errorMessage);
    connect(mPurposeMenuWidget, &PurposeMenuWidget::successMessage, this, &ShareTextInterface::successMessage);
}

ShareTextInterface::~ShareTextInterface() = default;

void ShareTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    menu->addMenu(mPurposeMenuWidget->menu());
}

void ShareTextInterface::setSelectedText(const QString &str)
{
    mPurposeMenuWidget->setSelectedText(str);
}

#include "moc_sharetextinterface.cpp"
