/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploreappstranslationwidget.h"
#include <KTreeWidgetSearchLine>
#include <QHeaderView>

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QTreeWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreAppsTranslationWidget::ExploreAppsTranslationWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeWidget(new QTreeWidget(this))
    , mSearchLineEdit(new KTreeWidgetSearchLine(this, mTreeWidget))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search…"));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mainLayout->addWidget(mSearchLineEdit);

    mTreeWidget->setObjectName(u"mTreeWidget"_s);
    mTreeWidget->setColumnCount(2);
    mainLayout->addWidget(mTreeWidget);

    mTreeWidget->setHeaderLabels({i18n("Identifier"), i18n("Translation")});

    mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTreeWidget, &QTreeWidget::customContextMenuRequested, this, &ExploreAppsTranslationWidget::slotContextMenu);
}

ExploreAppsTranslationWidget::~ExploreAppsTranslationWidget() = default;

void ExploreAppsTranslationWidget::slotContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeWidget->indexAt(pos);
    menu.addAction(QIcon::fromTheme(u"edit-copy"_s), i18nc("@action", "Copy"), this, [index]() {
        const QString currentValue = index.data().toString();
        QClipboard *clip = QApplication::clipboard();
        clip->setText(currentValue, QClipboard::Clipboard);
        clip->setText(currentValue, QClipboard::Selection);
    });
    menu.exec(mTreeWidget->viewport()->mapToGlobal(pos));
}

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

void ExploreAppsTranslationWidget::restoreState(const QByteArray &state)
{
    mTreeWidget->header()->restoreState(state);
}

QByteArray ExploreAppsTranslationWidget::saveState() const
{
    return mTreeWidget->header()->saveState();
}

#include "moc_exploreappstranslationwidget.cpp"
