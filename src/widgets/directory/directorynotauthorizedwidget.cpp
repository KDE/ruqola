/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorynotauthorizedwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>

DirectoryNotAuthorizedWidget::DirectoryNotAuthorizedWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    auto label = new QLabel(u"<qt><b>"_s + i18n("You are not authorized to view this page.") + u"</b></qt>"_s, this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::RichText);
    mainLayout->addWidget(label, 0, Qt::AlignCenter);
}

DirectoryNotAuthorizedWidget::~DirectoryNotAuthorizedWidget() = default;

#include "moc_directorynotauthorizedwidget.cpp"
