/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AUTOTRANSLATELANGUAGESMODEL_H
#define AUTOTRANSLATELANGUAGESMODEL_H

#include <QAbstractListModel>
#include "libruqola_private_export.h"
#include "autotranslatelanguages.h"

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguagesModel : public QAbstractListModel
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
    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    void parseLanguages(const QJsonObject &obj);

private:
    AutotranslateLanguages mAutoTranslateLanguages;
};

#endif // AUTOTRANSLATELANGUAGESMODEL_H
