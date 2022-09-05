/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurelanguagelistwidget.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <QListWidget>
#include <QVBoxLayout>

TranslationConfigureLanguageListWidget::TranslationConfigureLanguageListWidget(QWidget *parent)
    : QWidget{parent}
    , mLanguageListWidget(new QListWidget(this))
    , mListSearchLine(new KListWidgetSearchLine(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mListSearchLine->setObjectName(QStringLiteral("mListSearchLine"));
    mainLayout->addWidget(mListSearchLine);
    mListSearchLine->setPlaceholderText(i18n("Search..."));
    mListSearchLine->setListWidget(mLanguageListWidget);

    mLanguageListWidget->setObjectName(QStringLiteral("mLanguageListWidget"));
    mainLayout->addWidget(mLanguageListWidget);
}

TranslationConfigureLanguageListWidget::~TranslationConfigureLanguageListWidget() = default;
