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
#include "ruqola.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore>

Room RoomModel::fromJSon(const QJsonObject& o)
{
    Room r;
    
    r.id = o["id"].toString();
    r.type = o["t"].toString();
    r.name = o["name"].toString();
    r.userName = o["userName"].toString();
    r.userID = o["userID"].toString();
    r.topic = o["topic"].toString();
    r.mutedUsers = o["muted"].toString();
    r.jitsiTimeout = o["jitsiTimeout"].toDouble();
    r.ro = o["ro"].toBool();
    r.unread = o["unread"].toInt(0);
    
    return r;
}

QByteArray RoomModel::serialize(const Room& r)
{
    QJsonDocument d;
    QJsonObject o;

    o["id"] = r.id;
    o["t"] = r.type;
    o["name"] = r.name;
    o["userName"] = r.userName;
    o["userID"] = r.userID;
    o["topic"] = r.topic;
    o["muted"] = r.mutedUsers;
    o["jitsiTimeout"] = r.jitsiTimeout;
    o["ro"] = r.ro;
    o["unread"] = r.unread;

    d.setObject(o);
    return d.toBinaryData();
}

RoomWrapper::RoomWrapper(QObject *parent)
 : QObject(parent)
{}

RoomWrapper::RoomWrapper(const Room &r, QObject *parent)
 : QObject(parent)
{
    m_name = r.name;
    m_topic = r.topic;
    m_unread = r.unread;
    m_id = r.id;
    m_selected = r.selected;
}

QString RoomWrapper::getName() const
{
    return m_name;
}

QString RoomWrapper::getTopic() const
{
    return m_topic;
}

RoomModel::RoomModel(QObject* parent)
    : QAbstractListModel(parent)
{}

RoomModel::~RoomModel()
{
    QDir cacheDir(Ruqola::self()->cacheBasePath());
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

void RoomModel::clear()
{
    if (m_roomsList.size()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        m_roomsList.clear();
        QAbstractItemModel::endRemoveRows();
    }
}

RoomWrapper* RoomModel::findRoom(const QString& roomID) const
{
    foreach (const Room r, m_roomsList) {
        if (r.id == roomID) {
            return new RoomWrapper(r);
        }
    }
    Room r;
    return new RoomWrapper(r);
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    if (Ruqola::self()->cacheBasePath().isEmpty()) {
        return;
    }
    
    clear();
    
    QDir cacheDir(Ruqola::self()->cacheBasePath());
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
                addRoom(m.id, m.name, m.selected);
            }
        }
        qDebug() << "Cache Loaded";
    }
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = "name";
    roles[RoomID] = "room_id";
    roles[RoomSelected] = "selected";
    roles[RoomUnread] = "unread";
    return roles;
}

int RoomModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_roomsList.size();
}

QVariant RoomModel::data(const QModelIndex & index, int role) const
{
    Room r = m_roomsList.at(index.row());
    
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
    if (roomID.isEmpty() || roomName.isEmpty()) {
        return;
    }
    qDebug() << "Adding room" << roomID << roomName;
      
    Room r;
    r.id = roomID;
    r.name = roomName;
    r.selected = selected;
    addRoom(r);
}

void RoomModel::addRoom(const Room &room)
{
    auto existingRoom = qFind(m_roomsList.begin(), m_roomsList.end(), room);
    bool present = (existingRoom != m_roomsList.end());
    
    auto i = qUpperBound(m_roomsList.begin(), m_roomsList.end(),
                                               room);
    int pos = i-m_roomsList.begin();
    bool roomChanged = false;
    qDebug() << pos;
     
//     if (qFind(m_roomsList.begin(), m_roomsList.end(), room) != m_roomsList.end() && pos > 0) {
    if (present) {
//         qDebug() << (qFind(m_roomsList.begin(), m_roomsList.end(), room) - m_roomsList.begin());
//     if (pos != m_roomsList.size()) { // we're at the end
        qDebug() << "Room changed!";
        roomChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }
    
    if (roomChanged) {
        m_roomsList.replace(pos-1, room);
    } else {
        qDebug() << "Inserting room at position" <<pos;
        m_roomsList.insert(i, room);
    }
    
    if (roomChanged) {
        emit dataChanged(createIndex(1, 1), createIndex(pos, 1));
        
    } else {
        endInsertRows();
    }
    
    
    Ruqola::self()->getModelForRoom(room.id);
}


// #include "roommodel.moc"
