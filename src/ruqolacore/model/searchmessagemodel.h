/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SEARCHMESSAGEMODEL_H
#define SEARCHMESSAGEMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include "libruqola_private_export.h"
#include "searchmessage.h"

class LIBRUQOLACORE_TESTS_EXPORT SearchMessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SearchMessageRoles {
        MessageConvertedText = Qt::UserRole + 1,
        Timestamp,
        UserId,
        SystemMessageType,
        MessageId,
    };
    Q_ENUM(SearchMessageRoles)

    explicit SearchMessageModel(QObject *parent = nullptr);
    ~SearchMessageModel() override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = {}) const override;
    void parseResult(const QJsonObject &obj, bool restApi = false);
    void clear();
private:
    Q_DISABLE_COPY(SearchMessageModel)
    void setMessages(const QVector<SearchMessage> &channels);
    QVector<SearchMessage> mSearchMessages;
};

#endif // SEARCHMESSAGEMODEL_H
