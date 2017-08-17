/*

 * Copyright 2017  Riccardo Iaconelli <riccardo@kde.org>
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

#ifndef RUQOLAUTILS_H
#define RUQOLAUTILS_H

#include "libruqolacore_export.h"
#include <QObject>

class QString;
class QQmlEngine;
class QJSEngine;

class LIBRUQOLACORE_EXPORT RuqolaUtils : public QObject
{
    Q_OBJECT
public:
    static RuqolaUtils *self();
    Q_INVOKABLE QString markdownToRichText(const QString &markDown);
    Q_INVOKABLE void openUrl(const QString &url);

private:
    explicit RuqolaUtils(QObject *parent = nullptr);
};

inline static QObject *ruqolautils_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    RuqolaUtils *utils = RuqolaUtils::self();
    return utils;
}

#endif // RUQOLAUTILS_H
