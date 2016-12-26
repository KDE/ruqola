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

#include "roommodel.h"

#include <QtCore>

Room RoomModel::fromJSon(const QJsonObject& o)
{
    Room r;
    
    r.name = o["name"].toString();
    r.id = o["id"].toString();
    return r;
}

QByteArray RoomModel::serialize(const Room& r)
{
    QJsonDocument d;
    QJsonObject o;
    o["name"] = r.name;
    o["id"] = r.id;
    d.setObject(o);
    return d.toBinaryData();
}

RoomModel::RoomModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    
        // load cache
    if (cacheDir.exists(cacheDir.path())) {
        QFile f(cacheDir.absoluteFilePath("rooms"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray; quint32 length;
                in.readBytes(byteArray, length);
                QByteArray arr = QByteArray::fromRawData(byteArray, length);
                Room m = RoomModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                m_roomsList[m.name] = m;
//                 qDebug() << m.message;
            }
        }
    }
}

RoomModel::~RoomModel()
{
    QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }
    
    QFile f(cacheDir.absoluteFilePath("rooms"));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        foreach (const Room m, m_roomsList) {            
            QByteArray ms = RoomModel::serialize(m);
            out.writeBytes(ms, ms.size());
        }
    }
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = "name";
    roles[RoomID] = "id";
    roles[RoomSelected] = "selected";
    return roles;
}

int RoomModel::rowCount(const QModelIndex & parent) const
{
    return m_roomsList.size();
}

QVariant RoomModel::data(const QModelIndex & index, int role) const
{
    Room r = m_roomsList.values().at(index.row());
     if (role == RoomModel::RoomName) {
        return  r.name;
    } else if (role == RoomModel::RoomID) {
        return r.id;
    } else if (role == RoomModel::RoomSelected) {
        return r.selected;
    } else {
        return QVariant("0");
    }
}

void RoomModel::addRoom(const QString& roomID, const QString& roomName, bool selected)
{
    qDebug() << "Adding room" << roomID << rowCount();
    
    if (roomID.isEmpty()) {
        return;
    }
    
    int size = m_roomsList.size();
    beginInsertRows(index(size),  size, (size+1));
    Room r;
    r.id = roomID;
    r.name = roomName;
    r.selected = selected;
    m_roomsList[roomName] = r;
    endInsertRows();
}


// #include "roommodel.moc"
