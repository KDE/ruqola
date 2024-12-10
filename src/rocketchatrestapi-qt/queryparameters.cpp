/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "queryparameters.h"
#include "rocketchatqtrestapi_debug.h"

namespace RocketChatRestApi
{

QString QueryParameters::filter() const
{
    return mFilter;
}

bool QueryParameters::useSyntaxRc70() const
{
    return mUseSyntaxRc70;
}

void QueryParameters::setUseSyntaxRc70(bool newUseSyntaxRc70)
{
    mUseSyntaxRc70 = newUseSyntaxRc70;
}

void QueryParameters::setFilter(const QString &filter)
{
    mFilter = filter;
}

int QueryParameters::offset() const
{
    return mOffset;
}

void QueryParameters::setOffset(int offset)
{
    mOffset = offset;
}

int QueryParameters::count() const
{
    return mCount;
}

void QueryParameters::setCount(int count)
{
    mCount = count;
}

bool QueryParameters::isValid() const
{
    return (mCount >= 0) || (mOffset >= 0) || (!mSorting.isEmpty()) || !mCustom.isEmpty() || !mSearchString.isEmpty() || !mFilter.isEmpty();
}

QMap<QString, QueryParameters::SortOrder> QueryParameters::sorting() const
{
    return mSorting;
}

void QueryParameters::setSorting(const QMap<QString, QueryParameters::SortOrder> &sorting)
{
    mSorting = sorting;
}

QMap<QString, QString> QueryParameters::custom() const
{
    return mCustom;
}

void QueryParameters::setCustom(const QMap<QString, QString> &custom)
{
    mCustom = custom;
}

void QueryParameters::generateQueryParameter(const QueryParameters &queryParameters, QUrlQuery &urlQuery)
{
    if (queryParameters.count() >= 0) {
        urlQuery.addQueryItem(QStringLiteral("count"), QString::number(queryParameters.count()));
    }
    if (queryParameters.offset() >= 0) {
        urlQuery.addQueryItem(QStringLiteral("offset"), QString::number(queryParameters.offset()));
    }
    if (!queryParameters.filter().isEmpty()) {
        urlQuery.addQueryItem(QStringLiteral("filter"), queryParameters.filter());
    }

    const QMap<QString, QString> custom = queryParameters.custom();
    if (!custom.isEmpty()) {
        QMapIterator<QString, QString> i(custom);
        if (queryParameters.useSyntaxRc70()) {
            while (i.hasNext()) {
                i.next();
                urlQuery.addQueryItem(i.key(), i.value());
            }
        } else {
            QString str;
            while (i.hasNext()) {
                i.next();
                if (!str.isEmpty()) {
                    str += QLatin1Char(',');
                }
                str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
                str += QLatin1Char('"') + i.value() + QLatin1Char('"');
            }
            str = QStringLiteral("{%1}").arg(str);

            urlQuery.addQueryItem(QStringLiteral("query"), str);
        }
    }
    if (!queryParameters.searchString().isEmpty()) {
        if (queryParameters.useSyntaxRc70()) {
            urlQuery.addQueryItem(QLatin1String("name"), queryParameters.searchString());
        } else {
            const QString str = QStringLiteral(R"({"name":{"$regex":"%1","$options":"i"}})").arg(queryParameters.searchString());
            urlQuery.addQueryItem(QStringLiteral("query"), str);
        }
    }

    if (!queryParameters.sorting().isEmpty()) {
        // example    sort={"name" : -1,"status" : 1}
        QMapIterator<QString, QueryParameters::SortOrder> i(queryParameters.sorting());
        QString str;
        while (i.hasNext()) {
            i.next();
            if (!str.isEmpty()) {
                str += QLatin1Char(',');
            }
            str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
            switch (i.value()) {
            case QueryParameters::SortOrder::Ascendant:
                str += QString::number(1);
                break;
            case QueryParameters::SortOrder::Descendant:
                str += QString::number(-1);
                break;
            case QueryParameters::SortOrder::NoSorting:
                qCWarning(ROCKETCHATQTRESTAPI_LOG) << "It's not a sorting attribute";
                break;
            }
        }
        str = QStringLiteral("{%1}").arg(str);

        // It's ok for getAllMentions....
        urlQuery.addQueryItem(QStringLiteral("sort"), str);
    }
}

const QString &QueryParameters::searchString() const
{
    return mSearchString;
}

void QueryParameters::setSearchString(const QString &newSearchString)
{
    mSearchString = newSearchString;
}

} // namespace RocketChatRestApi
