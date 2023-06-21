/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguagesmodel.h"
#include <QJsonObject>

AutotranslateLanguagesModel::AutotranslateLanguagesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AutotranslateLanguagesModel::~AutotranslateLanguagesModel() = default;

int AutotranslateLanguagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
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
    case Qt::DisplayRole:
    case AutotranslateLanguagesModel::DisplayName:
        return language.displayLanguage();
    }
    return {};
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

int AutotranslateLanguagesModel::currentLanguage(const QString &lang) const
{
    for (int i = 0; i < mAutoTranslateLanguages.autotranslateLanguage().count(); ++i) {
        if (mAutoTranslateLanguages.autotranslateLanguage().at(i).language() == lang) {
            return i;
        }
    }
    return 1;
}

QString AutotranslateLanguagesModel::selectedLanguage(int index) const
{
    return mAutoTranslateLanguages.autotranslateLanguage().at(index).language();
}

#include "moc_autotranslatelanguagesmodel.cpp"
