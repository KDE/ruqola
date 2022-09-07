/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelanguagelistwidget.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

TranslatorConfigureLanguageListWidget::TranslatorConfigureLanguageListWidget(const QString &labelText, QWidget *parent)
    : QWidget{parent}
    , mLanguageListWidget(new QListWidget(this))
    , mListSearchLine(new KListWidgetSearchLine(this))
    , mLabel(new QLabel(labelText, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mLabel->setObjectName(QStringLiteral("mLabel"));
    mainLayout->addWidget(mLabel);

    mListSearchLine->setObjectName(QStringLiteral("mListSearchLine"));
    mainLayout->addWidget(mListSearchLine);
    mListSearchLine->setPlaceholderText(i18n("Search..."));
    mListSearchLine->setListWidget(mLanguageListWidget);

    mLanguageListWidget->setObjectName(QStringLiteral("mLanguageListWidget"));
    mainLayout->addWidget(mLanguageListWidget);
}

TranslatorConfigureLanguageListWidget::~TranslatorConfigureLanguageListWidget() = default;

void TranslatorConfigureLanguageListWidget::clear()
{
    mLanguageListWidget->clear();
}

void TranslatorConfigureLanguageListWidget::addItem(const QPair<QString, QString> &lang)
{
    QListWidgetItem *item = new QListWidgetItem(mLanguageListWidget);
    item->setText(lang.first);
    item->setData(LanguageCode, lang.second);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
}

QStringList TranslatorConfigureLanguageListWidget::selectedLanguages() const
{
    QStringList langs;
    for (int i = 0; i < mLanguageListWidget->count(); ++i) {
        const auto item = mLanguageListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            langs.append(item->data(LanguageCode).toString());
        }
    }
    return langs;
}

void TranslatorConfigureLanguageListWidget::setSelectedLanguages(const QStringList &list)
{
    for (int i = 0; i < mLanguageListWidget->count(); ++i) {
        const auto item = mLanguageListWidget->item(i);
        item->setCheckState(list.contains(item->data(LanguageCode).toString()) ? Qt::Checked : Qt::Unchecked);
    }
}
