/*
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
#include "GameDataSimpleSlide.h"

#include "CommonPhase/PhaseShowFinalImage.h"
#include "CommonPhase/PhaseCleared.h"
#include "Fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSlideEnding.h"
#include "PhaseSimpleSlideGaming.h"

#include "Application.h"
#include "AnimationFactory.h"
#include "SaveDataSimpleSlide.h"
#include "SlideGameBoard.h"
#include "SlideEndingBoard.h"

#include <QDebug>

namespace Slide {

GameDataSimpleSlide::GameDataSimpleSlide(const SourceImage &img, const UniquePosition &defaultBlankPos, const QSize &xyCount) :
    m_sourceImg(img),
    m_board(QSharedPointer<GameBoard>::create(QSharedPointer<BoardInformation>::create(xyCount, img.size()), m_currentBlankPos)),
    m_defaultBlankPos(defaultBlankPos),
    m_currentBlankPos(defaultBlankPos.selectedPosition()),
    m_currentPhaseType(AbstractPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSlide::GameDataSimpleSlide(const SaveDataSimpleSlide &loadedSavedata) :
    m_sourceImg(loadedSavedata.sourceImage()),
    m_board(QSharedPointer<GameBoard>::create(loadedSavedata.boardInformation(), m_currentBlankPos)),
    m_defaultBlankPos(loadedSavedata.specifiedPosition()),
    m_currentBlankPos(loadedSavedata.currentBlankPosition()),
    m_currentPhaseType(loadedSavedata.currentPhase())
{
    Q_ASSERT(loadedSavedata.isValid());

    m_board->initPieces(m_sourceImg.pixmap(), loadedSavedata.defaultPositions());

    createBlankPiece();
}

GameDataPointer GameDataSimpleSlide::cloneAsNewGame() const
{
    return QSharedPointer<GameDataSimpleSlide>::create(
                m_sourceImg, m_defaultBlankPos, m_board->boardInfo()->xyCount());
}

QString GameDataSimpleSlide::gameName() const
{
    return Application::gameName(Application::SimpleSlide);
}

PhasePointer GameDataSimpleSlide::createPhase(AbstractPhase::PhaseType phaseType)
{
    m_currentPhaseType = phaseType;

    switch (phaseType) {
    case AbstractPhase::PhaseReady:
        if (m_defaultBlankPos.isRandom()) {
            m_defaultBlankPos.randomSelect(m_board->boardInfo()->xyCount());
            m_currentBlankPos = m_defaultBlankPos.selectedPosition();
        }

        m_board->initPieces(m_sourceImg.pixmap());

        createBlankPiece();

        m_board->shufflePieces();

        m_currentPhaseType = AbstractPhase::PhaseGaming;

        [[fallthrough]];

    case AbstractPhase::PhaseGaming:
        return QSharedPointer<PhaseSimpleSlideGaming>::create(
                    m_board, m_currentBlankPos, m_defaultBlankPos.selectedPosition()
                  , AbstractPhase::PhaseEnding);

    case AbstractPhase::PhaseEnding: {
        auto board = QSharedPointer<EndingBoard>::create(m_board->boardInfo(), m_sourceImg.pixmap()
                                                       , m_defaultBlankPos.selectedPosition());
        return QSharedPointer<PhaseSimpleSlideEnding>::create(board, AbstractPhase::PhaseCleared);
    }

    case AbstractPhase::PhaseCleared:
        return QSharedPointer<PhaseCleared>::create(m_sourceImg, AbstractPhase::PhaseReady);
    }
}

AbstractPhase::PhaseType GameDataSimpleSlide::currentPhase() const
{
    return m_currentPhaseType;
}

const SourceImage &GameDataSimpleSlide::sourceImage() const
{
    return m_sourceImg;
}

FinalImagePointer GameDataSimpleSlide::finalImage() const
{
    return FinalImagePointer::create(m_sourceImg.pixmap());
}

BoardInfoPointer GameDataSimpleSlide::boardInfo() const
{
    return m_board->boardInfo();
}

bool GameDataSimpleSlide::save(QStringView fileName) const
{
    if (m_currentPhaseType == AbstractPhase::PhaseCleared)
        return m_sourceImg.saveImage();

    SaveDataSimpleSlide savedata(fileName, m_board->boardInfo()->xyCount(), m_defaultBlankPos
                               , m_sourceImg, m_currentPhaseType, m_board->defaultPiecesPos(), m_currentBlankPos);

    return savedata.write();
}

void GameDataSimpleSlide::createBlankPiece()
{
    auto blankPiece = QSharedPointer<Fifteen::SlideBlankPiece>::create(
                          m_board->boardInfo(), m_defaultBlankPos.selectedPosition(), Qt::black
                        , m_board->frameCountToMove());

    blankPiece->setPosWithoutAnimation(m_currentBlankPos);

    EffectPointer graduallyFrame = AnimationFactory::graduallyBlinkFrame(
                                       15, QColor(0, 0, 0), QColor(0, 0, 0)
                                     , QColor(64, 192, 224, 224), QColor(16, 64, 96, 224), 120);

    blankPiece->setEffect(graduallyFrame);

    m_board->piece(m_currentBlankPos) = blankPiece;
}

} // Slide
