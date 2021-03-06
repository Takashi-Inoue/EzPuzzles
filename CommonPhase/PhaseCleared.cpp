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
#include "PhaseCleared.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"

PhaseCleared::PhaseCleared(const SourceImage &image, PhaseType nextPhase, QObject *parent)
    : AbstractPhase(nextPhase, parent)
    , m_sourceImage(image)
    , m_effect(new Effect::GraduallyBlinkFrame(10, QColor(255, 224, 96, 224), QColor(255, 255, 96, 0)
                                                 , QColor(255, 255, 96, 32),  QColor(255, 255, 96, 0)
                                                 , 240, true))
{
    Q_ASSERT(!image.isNull());
}

void PhaseCleared::click(const QPoint &)
{
    emit toNextPhase(m_nextPhaseType);
}

void PhaseCleared::onTickFrame()
{
    m_effect->onTickFrame();
}

void PhaseCleared::draw(QPainter &painter)
{
    painter.drawPixmap(painter.viewport(), m_sourceImage.pixmap());
    m_effect->draw(painter, painter.viewport());
}

bool PhaseCleared::canSave() const
{
    return true;
}

bool PhaseCleared::canLoad() const
{
    return false;
}

QString PhaseCleared::information() const
{
    return QStringLiteral("Cleared!");
}
