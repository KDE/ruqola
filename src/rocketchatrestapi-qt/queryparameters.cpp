/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "queryparameters.h"
using namespace Qt::Literals::StringLiterals;

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
        urlQuery.addQueryItem(u"count"_s, QString::number(queryParameters.count()));
    }
    if (queryParameters.offset() >= 0) {
        urlQuery.addQueryItem(u"offset"_s, QString::number(queryParameters.offset()));
    }
    if (!queryParameters.filter().isEmpty()) {
        urlQuery.addQueryItem(u"filter"_s, queryParameters.filter());
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
                    str += u',';
                }
                str += QLatin1Char('"') + i.key() + u'"' + u':';
                str += u'"' + i.value() + u'"';
            }
            str = u"{%1}"_s.arg(str);

            urlQuery.addQueryItem(u"query"_s, str);
        }
    }
    if (!queryParameters.searchString().isEmpty()) {
        if (queryParameters.useSyntaxRc70()) {
            urlQuery.addQueryItem(QLatin1String("name"), queryParameters.searchString());
        } else {
            const QString str = QStringLiteral(R"({"name":{"$regex":"%1","$options":"i"}})").arg(queryParameters.searchString());
            urlQuery.addQueryItem(u"query"_s, str);
        }
    }

    if (!queryParameters.sorting().isEmpty()) {
        // example    sort={"name" : -1,"status" : 1}
        QMapIterator<QString, QueryParameters::SortOrder> i(queryParameters.sorting());
        QString str;
        while (i.hasNext()) {
            i.next();
            if (!str.isEmpty()) {
                str += u',';
            }
            str += QLatin1Char('"') + i.key() + u'"' + u':';
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
        str = u"{%1}"_s.arg(str);

        // It's ok for getAllMentions....
        urlQuery.addQueryItem(u"sort"_s, str);
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
