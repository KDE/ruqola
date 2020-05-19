/*
 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
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

#pragma once

#include <QObject>

namespace RuqolaQmlPlain {
namespace Controllers {

class SettingsController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool compactMode READ compactMode NOTIFY compactModeChanged)

  public:
    SettingsController(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_REQUIRED_RESULT bool compactMode() const
    {
        return m_compactMode;
    }

    Q_INVOKABLE void setCompactMode(bool compactMode)
    {
        if (compactMode != m_compactMode) {
            m_compactMode = compactMode;
            Q_EMIT compactModeChanged();
        }
    }

    Q_INVOKABLE void reset()
    {
        setCompactMode(false);
    }

  Q_SIGNALS:
    void compactModeChanged();

  private:
    bool m_compactMode = false;
};

} // namespace Controllers
} // namespace RuqolaQmlPlain
