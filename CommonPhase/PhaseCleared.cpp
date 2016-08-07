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
#include "PhaseCleared.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"

#include <QFileDialog>

PhaseCleared::PhaseCleared(const SourceImage &image, PhaseType nextPhase, QObject *parent) :
    IPhase(parent),
    sourceImage(image),
    nextPhase(nextPhase),
    effect(std::make_unique<Effect::GraduallyBlinkFrame>(10, QColor(255, 224, 96, 224), QColor(255, 255, 96, 0), QColor(255, 255, 96, 32), QColor(255, 255, 96, 0), 240, true))
{
    Q_ASSERT(!image.isNull());
}

void PhaseCleared::click(const QPoint &)
{
    emit toNextPhase(nextPhase);
}

void PhaseCleared::onTickFrame()
{
    effect->onTickFrame();
}

void PhaseCleared::draw(QPainter &painter)
{
    painter.drawPixmap(painter.viewport(), sourceImage.pixmap);
    effect->draw(painter, painter.viewport());
}

bool PhaseCleared::canSave() const
{
    return true;
}

bool PhaseCleared::canLoad() const
{
    return false;
}

bool PhaseCleared::save(QDataStream &) const
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save final image as..."), "untitled.png", "Image (*.png)");

    if (fileName.isEmpty())
        return true;

    return sourceImage.pixmap.save(fileName, "PNG");
}

bool PhaseCleared::load(QDataStream &)
{
    Q_ASSERT(false);
    return false;
}

QString PhaseCleared::information() const
{
    return "Cleared!";
}
