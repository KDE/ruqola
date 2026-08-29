/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>
#include <QTemporaryDir>

class PixmapCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit PixmapCacheTest(QObject *parent = nullptr);
    ~PixmapCacheTest() override = default;

private Q_SLOTS:
    void initTestCase();

    void shouldReturnNullPixmapForMissingFile();
    void shouldNotCacheMissingFile();

    void shouldReturnUnscaledPixmap();
    void shouldReuseCachedUnscaledPixmap();

    void shouldScaleToRequestedSize();
    void shouldScaleForDevicePixelRatio();
    void shouldReuseCachedScaledPixmap();
    void shouldKeepAspectRatio();

    void shouldNotShareEntryBetweenScaledAndUnscaled();
    void shouldNotShareEntryBetweenDevicePixelRatios();

    void shouldRespectMaxEntries();
    void shouldClearCache();

private:
    [[nodiscard]] QString squarePath() const;
    [[nodiscard]] QString widePath() const;
    QTemporaryDir mTempDir;
};
