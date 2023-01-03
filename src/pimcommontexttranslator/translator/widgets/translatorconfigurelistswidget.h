/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_export.h"
#include <PimCommonTextTranslator/TranslatorUtil>
#include <QWidget>
namespace PimCommonTextTranslator
{
/**
 * @brief The TranslatorConfigureListsWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorConfigureListsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslatorConfigureListsWidget(QWidget *parent = nullptr);
    ~TranslatorConfigureListsWidget() override;

    void save();
    void load();

private:
    void loadLanguagesList();
    void fillLanguages(const QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> &listLanguage);
    void slotEngineChanged(const QString &engine);
    class TranslatorConfigureListsWidgetPrivate;
    std::unique_ptr<TranslatorConfigureListsWidgetPrivate> const d;
};
}
