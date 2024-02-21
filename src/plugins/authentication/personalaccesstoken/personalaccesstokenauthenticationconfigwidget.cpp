/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigwidget.h"
#include <KLocalizedString>
#include <QFormLayout>

PersonalAccessTokenAuthenticationConfigWidget::PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent)
    : QWidget{parent}
{
}

PersonalAccessTokenAuthenticationConfigWidget::~PersonalAccessTokenAuthenticationConfigWidget() = default;

#include "moc_personalaccesstokenauthenticationconfigwidget.cpp"
