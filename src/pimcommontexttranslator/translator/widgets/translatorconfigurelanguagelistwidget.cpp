/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelanguagelistwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;

TranslatorConfigureLanguageListWidget::TranslatorConfigureLanguageListWidget(const QString &labelText, QWidget *parent)
    : QWidget{parent}
    , mLanguageListWidget(new QListView(this))
    , mListSearchLine(new QLineEdit(this))
    , mLabel(new QLabel(labelText, this))
    , mModel(new QStandardItemModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mLabel->setObjectName(QStringLiteral("mLabel"));
    mainLayout->addWidget(mLabel);

    mModel->setObjectName(QStringLiteral("mModel"));

    mListSearchLine->setObjectName(QStringLiteral("mListSearchLine"));
    mainLayout->addWidget(mListSearchLine);
    mListSearchLine->setPlaceholderText(i18n("Search..."));
    auto filterProxyModel = new QSortFilterProxyModel(this);
    filterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    filterProxyModel->setSourceModel(mModel);
    connect(mListSearchLine, &QLineEdit::textChanged, this, [filterProxyModel](const QString &str) {
        filterProxyModel->setFilterFixedString(str);
    });

    mLanguageListWidget->setObjectName(QStringLiteral("mLanguageListWidget"));
    mainLayout->addWidget(mLanguageListWidget);
    mLanguageListWidget->setModel(filterProxyModel);
}

TranslatorConfigureLanguageListWidget::~TranslatorConfigureLanguageListWidget() = default;

void TranslatorConfigureLanguageListWidget::clear()
{
    mModel->clear();
}

void TranslatorConfigureLanguageListWidget::addItem(const QString &translatedStr, const QString &languageCode)
{
    QStandardItem *item = new QStandardItem();
    item->setText(translatedStr);
    item->setData(languageCode, LanguageCode);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setEditable(false);
    item->setCheckState(Qt::Unchecked);
    mModel->appendRow(item);
}

QStringList TranslatorConfigureLanguageListWidget::selectedLanguages() const
{
    QStringList langs;
    for (int i = 0; i < mModel->rowCount(); ++i) {
        const auto item = mModel->item(i);
        if (item->checkState() == Qt::Checked) {
            langs.append(item->data(LanguageCode).toString());
        }
    }
    return langs;
}

void TranslatorConfigureLanguageListWidget::setSelectedLanguages(const QStringList &list)
{
    for (int i = 0; i < mModel->rowCount(); ++i) {
        const auto item = mModel->item(i);
        item->setCheckState(list.contains(item->data(LanguageCode).toString()) ? Qt::Checked : Qt::Unchecked);
    }
}
