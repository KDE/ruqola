/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autotranslate/autotranslatelanguages.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AutotranslateLanguagesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AutoTranslateLanguesRoles {
        LanguageName = Qt::UserRole + 1,
        DisplayName,
    };
    Q_ENUM(AutoTranslateLanguesRoles)

    explicit AutotranslateLanguagesModel(QObject *parent = nullptr);
    ~AutotranslateLanguagesModel() override;
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void parseLanguages(const QJsonObject &obj);

    [[nodiscard]] int currentLanguage(const QString &lang) const;
    [[nodiscard]] QString selectedLanguage(int index) const;

private:
    AutotranslateLanguages mAutoTranslateLanguages;
};
