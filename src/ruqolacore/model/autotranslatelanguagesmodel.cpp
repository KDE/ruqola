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

#include "autotranslatelanguagesmodel.h"
#include <QJsonObject>

AutotranslateLanguagesModel::AutotranslateLanguagesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

AutotranslateLanguagesModel::~AutotranslateLanguagesModel()
{

}

int AutotranslateLanguagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAutoTranslateLanguages.count();
}

QVariant AutotranslateLanguagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAutoTranslateLanguages.count()) {
        return {};
    }
    const AutotranslateLanguage language = mAutoTranslateLanguages.at(index.row());

    switch (role) {
    case AutotranslateLanguagesModel::LanguageName:
        return language.language();
    case AutotranslateLanguagesModel::DisplayName:
        return language.displayLanguage();
    }
    return {};
}

QHash<int, QByteArray> AutotranslateLanguagesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AutotranslateLanguagesModel::LanguageName] = QByteArrayLiteral("language");
    roles[AutotranslateLanguagesModel::DisplayName] = QByteArrayLiteral("displaylanguage");
    return roles;
}

void AutotranslateLanguagesModel::parseLanguages(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAutoTranslateLanguages.count() - 1);
        mAutoTranslateLanguages.clear();
        endRemoveRows();
    }
    mAutoTranslateLanguages.parseLanguages(obj);
    if (!mAutoTranslateLanguages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mAutoTranslateLanguages.count() - 1);
        endInsertRows();
    }
}
