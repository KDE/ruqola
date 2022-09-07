/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QListWidget;
class KListWidgetSearchLine;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TranslatorConfigureLanguageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslatorConfigureLanguageListWidget(const QString &labelText, QWidget *parent = nullptr);
    ~TranslatorConfigureLanguageListWidget() override;

    void clear();

    void addItem(const QPair<QString, QString> &lang);

    Q_REQUIRED_RESULT QStringList selectedLanguages() const;
    void setSelectedLanguages(const QStringList &list);

private:
    enum LanguageData {
        LanguageCode = Qt::UserRole + 1,
    };
    QListWidget *const mLanguageListWidget;
    KListWidgetSearchLine *const mListSearchLine;
    QLabel *const mLabel;
};
