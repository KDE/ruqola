/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "librocketchatrestapi-qt_export.h"
#include <QDebug>
#include <QMap>
#include <QString>
#include <QUrlQuery>
namespace RocketChatRestApi
{

class LIBROCKETCHATRESTAPI_QT_EXPORT QueryParameters
{
    Q_GADGET
public:
    enum class SortOrder : uint8_t {
        Ascendant,
        Descendant,
        NoSorting,
    };
    Q_ENUM(SortOrder);
    QueryParameters();

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int count() const;
    void setCount(int count);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QMap<QString, SortOrder> sorting() const;
    void setSorting(const QMap<QString, SortOrder> &sorting);

    [[nodiscard]] QMap<QString, QString> custom() const;
    void setCustom(const QMap<QString, QString> &custom);

    static void generateQueryParameter(const QueryParameters &queryParameters, QUrlQuery &urlQuery);

    [[nodiscard]] const QString &searchString() const;
    void setSearchString(const QString &newSearchString);

    void setFilter(const QString &filter);
    [[nodiscard]] QString filter() const;

    [[nodiscard]] bool useSyntaxRc70() const;
    void setUseSyntaxRc70(bool newUseSyntaxRc70);

private:
    int mOffset = -1;
    int mCount = -1;
    QMap<QString, SortOrder> mSorting;
    QMap<QString, QString> mCustom;
    QString mSearchString;
    QString mFilter;
    bool mUseSyntaxRc70 = false;
};

} // namespace RocketChatRestApi
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::QueryParameters &t);
