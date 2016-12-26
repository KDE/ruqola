/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QAbstractListModel>

struct Room {
    QString name;
    bool selected = false;
    QString id;
};

class RoomModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum RoomRoles {
        RoomName = Qt::UserRole + 1,
        RoomSelected,
        RoomID
    };
    
    RoomModel (QObject *parent = 0);
    virtual ~RoomModel();
    
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//     void setCurrentRoom(const QString &newRoom);
//     QString getCurrentRoom() const;
    
    Q_INVOKABLE void addRoom(const QString& roomID, const QString& roomName, bool selected = false);
    
    static Room fromJSon(const QJsonObject &source);
    static QByteArray serialize(const Room &r);
    
    void reset();
    void clear();
protected:
    virtual QHash<int, QByteArray> roleNames() const;
    
private:
    
    QHash< QString, Room > m_roomsList;
};

#endif // ROOMMODEL_H
