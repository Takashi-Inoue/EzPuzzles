/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PhaseShowFinalImage.h"

PhaseShowFinalImage::PhaseShowFinalImage(const SourceImage &img, PhaseType nextPhase, QObject *parent) :
    IPhase(parent),
    sourceImage(img),
    nextPhase(nextPhase)
{
    Q_ASSERT(!img.isNull());
}

void PhaseShowFinalImage::click(const QPoint &)
{
    emit toNextPhase(nextPhase);
}

void PhaseShowFinalImage::draw(QPainter &painter)
{
    painter.drawPixmap(painter.viewport(), sourceImage.pixmap);
}

bool PhaseShowFinalImage::canSave() const
{
    return true;
}

bool PhaseShowFinalImage::canLoad() const
{
    return true;
}

QString PhaseShowFinalImage::information() const
{
    return "";
}
