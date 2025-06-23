/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploreappstranslationwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QTreeWidget>
#include <QVBoxLayout>

ExploreAppsTranslationWidget::ExploreAppsTranslationWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeWidget(new QTreeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTreeWidget->setObjectName(u"mTreeWidget"_s);
    mTreeWidget->setColumnCount(2);
    mainLayout->addWidget(mTreeWidget);

    mTreeWidget->setHeaderLabels({i18n("Identifier"), i18n("Translation")});
}

ExploreAppsTranslationWidget::~ExploreAppsTranslationWidget() = default;

void ExploreAppsTranslationWidget::setAppsLanguagesInfoMap(const QMap<QString, DownloadAppsLanguagesInfo> &map)
{
    for (const auto [key, value] : map.asKeyValueRange()) {
        auto itemTop = new QTreeWidgetItem(mTreeWidget);
        itemTop->setText(0, key);
        const QMap<QString, QMap<QString, QString>> languageMap = value.languageMap();
        for (const auto [key, value] : languageMap.asKeyValueRange()) {
            auto itemLang = new QTreeWidgetItem(itemTop);
            itemLang->setText(0, key);

            for (const auto [key, translation] : value.asKeyValueRange()) {
                auto translateItem = new QTreeWidgetItem(itemLang);
                translateItem->setText(0, key);
                translateItem->setText(1, translation);
            }
        }
    }
    mTreeWidget->expandAll();
}

#include "moc_exploreappstranslationwidget.cpp"
