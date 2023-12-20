/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpasswordwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>

ResetPasswordWidget::ResetPasswordWidget(QWidget *parent)
    : QWidget{parent}
{
}

ResetPasswordWidget::~ResetPasswordWidget() = default;

#include "moc_resetpasswordwidget.cpp"
