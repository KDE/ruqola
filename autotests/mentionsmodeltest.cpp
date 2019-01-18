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

#include "mentionsmodeltest.h"
#include "model/mentionsmodel.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(MentionsModelTest)

MentionsModelTest::MentionsModelTest(QObject *parent)
    : QObject(parent)
{

}

void MentionsModelTest::shouldHaveDefaultValue()
{
    MentionsModel w;
    QSignalSpy rowInsertedSpy(&w, &MentionsModel::rowsInserted);
    // (if it had 0 columns, it would have to emit column insertions, too much trouble)
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QHash<int, QByteArray> roles;
    //roles[FilesForRoomModel::UserName] = QByteArrayLiteral("username");
    //TODO FIXME
    QCOMPARE(w.roleNames(), roles);
}
