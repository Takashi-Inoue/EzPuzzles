﻿/*
 * Copyright 2016 Takashi Inoue
 *
 * This file is part of EzPuzzles.
 *
 * EzPuzzles is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EzPuzzles is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EzPuzzles.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IGAME_H
#define IGAME_H

class QPixmap;
class QPainter;
class QPoint;
class SourceImage;

#include "GameID.h"

#include <QObject>
#include <QSize>
#include <QString>

class IGame : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual GameID gameID() const = 0;

    virtual QSharedPointer<IGame> cloneAsNewGame() const = 0;
    virtual void save(QStringView saveDirPath, const QSize &screenshotSize) const = 0;

    virtual void onTickFrame() = 0;

    virtual void mouseMove(const QSize &fieldSize, const QPoint &cursorPos) = 0;
    virtual void mousePress(const QSize &fieldSize, const QPoint &cursorPos) = 0;
    virtual void mouseRelease(const QSize &fieldSize, const QPoint &cursorPos) = 0;

    virtual void draw(QPainter &dest) = 0;
    virtual void drawFinalImage(QPainter &dest) const = 0;

    virtual bool hasFinalImage() const = 0;
    virtual QSize maxFieldSize() const = 0;
    virtual QString shortInformation() const = 0;
    virtual const SourceImage &sourceImage() const = 0;

signals:
    void finalImageUpdated();
    void informationUpdated(QString);

protected:
    virtual void saveScreenshot(QStringView saveDirPath, const QSize &screenshotSize) const = 0;

private:
    IGame(const IGame &) = delete;
    IGame(IGame &&) = delete;
    IGame &operator=(const IGame &) = delete;
    IGame &operator=(IGame &&) = delete;
};

#endif // IGAME_H
