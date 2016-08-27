#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T20:19:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EzPuzzles
TEMPLATE = app

QMAKE_LFLAGS_DEBUG += /LTCG
QMAKE_LFLAGS_RELEASE += /LTCG

TRANSLATIONS += ezpz_ja.ts

SOURCES += main.cpp\
        MainWindow.cpp \
    FormFinalImage.cpp \
    DialogSettingsFifteen.cpp \
    ImageWidget.cpp \
    GameWidget.cpp \
    DialogGameStart.cpp \
    DialogSettingsMineSweeper.cpp \
    MineSweeper/MinePiece.cpp \
    MineSweeper/WallPiece.cpp \
    Dragger.cpp \
    SubFrame.cpp \
    ImagePiece.cpp \
    NumberPiece.cpp \
    BlockPiece.cpp \
    SwitchImagePiece.cpp \
    MineSweeper/SafePiece.cpp \
    MineSweeper/NumberPieceFactory.cpp \
    ThreadOperation.cpp \
    GarbageCollector.cpp \
    ImageFragmentPiece.cpp \
    MineSweeper/MineLocker.cpp \
    DialogImageHistory.cpp \
    ImageLoader.cpp \
    StringListHistory.cpp \
    EzPuzzles.cpp \
    ToolIconWindow.cpp \
    DialogSavedata.cpp \
    SourceImage.cpp \
    fifteen/PuzzlePiece.cpp \
    MineSweeper/MinePiecesFactory.cpp \
    fifteen/SimplePiecesFactory.cpp \
    fifteen/FifteenSlideShuffler.cpp \
    fifteen/FifteenPieceMover.cpp \
    GameID.cpp \
    fifteen/FifteenSwapShuffler.cpp \
    GameInfoLoader.cpp \
    BrokenSaveData.cpp \
    UnknownSaveData.cpp \
    MoveToTrashBox.cpp \
    AnimationObject/AbstractAnimationObject.cpp \
    AnimationObject/Animation/AnimationLineMove.cpp \
    AnimationObject/Animation/AnimationWarpMove.cpp \
    AnimationObject/Effect/EffectSimpleFrame.cpp \
    AnimationObject/Effect/EffectGraduallyBlinkFrame.cpp \
    BoardInformation.cpp \
    Position.cpp \
    fifteen/SlideBlankPiece.cpp \
    FrameTimer.cpp \
    AnimationObject/Effect/CompositeEffect.cpp \
    GridLines.cpp \
    SelectCellGrid.cpp \
    UniquePosition.cpp \
    CommonPhase/PhaseShowFinalImage.cpp \
    CommonPhase/PhaseShuffle.cpp \
    CommonPhase/PhaseCleared.cpp \
    Slide/PhaseSimpleSlideGaming.cpp \
    Board.cpp \
    Slide/GameDataSimpleSlide.cpp \
    Slide/SaveDataSimpleSlide.cpp \
    Slide/PhaseSimpleSlideEnding.cpp \
    AnimationObject/Effect/EffectRoundMoveFrame.cpp \
    Swap/GameDataSimpleSwap.cpp \
    Swap/SaveDataSimpleSwap.cpp \
    Swap/PhaseSimpleSwapEnding.cpp \
    Swap/PhaseSimpleSwapGaming.cpp \
    GameCore.cpp \
    MineSweeper/PhaseMineSweeperGaming.cpp \
    MineSweeper/GameDataMineSweeper.cpp \
    MineSweeper/MineField.cpp \
    MineSweeper/SaveDataMineSweeper.cpp \
    MineSweeper/GameCoreMineSweeper.cpp \
    FinalImage.cpp \
    MineSweeper/MineSweeperFinalImage.cpp \
    AnimationObject/Transform/TransformExpand.cpp \
    AnimationObject/Transform/ChainedTransform.cpp \
    AnimationObject/Transform/AbstractTransform.cpp \
    fifteen/EffectSwapper.cpp

HEADERS  += MainWindow.h \
    FormFinalImage.h \
    DialogSettingsFifteen.h \
    ImageWidget.h \
    GameWidget.h \
    IGame.h \
    IShuffler.h \
    IPiece.h \
    DialogGameStart.h \
    DialogSettingsMineSweeper.h \
    MineSweeper/MinePiece.h \
    MineSweeper/WallPiece.h \
    ISubWidget.h \
    Dragger.h \
    SubFrame.h \
    IDrawer.h \
    ImagePiece.h \
    NumberPiece.h \
    ISwitchPiece.h \
    BlockPiece.h \
    SwitchImagePiece.h \
    MineSweeper/IMinePiece.h \
    MineSweeper/SafePiece.h \
    MineSweeper/NumberPieceFactory.h \
    ThreadOperation.h \
    GarbageCollector.h \
    ImageFragmentPiece.h \
    MineSweeper/MineLocker.h \
    Utility.h \
    DialogImageHistory.h \
    ImageLoader.h \
    StringListHistory.h \
    EzPuzzles.h \
    ToolIconWindow.h \
    DialogSavedata.h \
    SourceImage.h \
    fifteen/IPuzzlePiece.h \
    fifteen/PuzzlePiece.h \
    MineSweeper/MinePiecesFactory.h \
    fifteen/SimplePiecesFactory.h \
    fifteen/FifteenSlideShuffler.h \
    fifteen/FifteenPieceMover.h \
    GameID.h \
    fifteen/FifteenSwapShuffler.h \
    IDialogGameSettings.h \
    GameInfoLoader.h \
    ISaveData.h \
    BrokenSaveData.h \
    UnknownSaveData.h \
    MoveToTrashBox.h \
    AnimationObject/AbstractAnimationObject.h \
    AnimationObject/IAnimationObject.h \
    AnimationObject/Animation/AbstractAnimation.h \
    AnimationObject/Effect/AbstractEffect.h \
    AnimationObject/Animation/AnimationLineMove.h \
    AnimationObject/Animation/AnimationWarpMove.h \
    AnimationObject/Effect/EffectSimpleFrame.h \
    AnimationObject/Effect/EffectGraduallyBlinkFrame.h \
    BoardInformation.h \
    Position.h \
    fifteen/SlideBlankPiece.h \
    fifteen/FifteenAbstractShuffler.h \
    FrameTimer.h \
    AnimationObject/Effect/CompositeEffect.h \
    GridLines.h \
    SelectCellGrid.h \
    UniquePosition.h \
    IPhase.h \
    IGameData.h \
    CommonPhase/PhaseShowFinalImage.h \
    CommonPhase/PhaseShuffle.h \
    CommonPhase/PhaseCleared.h \
    Slide/PhaseSimpleSlideGaming.h \
    Board.h \
    Slide/GameDataSimpleSlide.h \
    Slide/SaveDataSimpleSlide.h \
    Slide/PhaseSimpleSlideEnding.h \
    AnimationObject/Effect/EffectRoundMoveFrame.h \
    Swap/GameDataSimpleSwap.h \
    Swap/SaveDataSimpleSwap.h \
    Swap/PhaseSimpleSwapEnding.h \
    Swap/PhaseSimpleSwapGaming.h \
    GameCore.h \
    MineSweeper/PhaseMineSweeperGaming.h \
    MineSweeper/GameDataMineSweeper.h \
    MineSweeper/MineField.h \
    MineSweeper/SaveDataMineSweeper.h \
    MineSweeper/GameCoreMineSweeper.h \
    FinalImage.h \
    MineSweeper/MineSweeperFinalImage.h \
    AnimationObject/Transform/AbstractTransform.h \
    AnimationObject/Transform/TransformExpand.h \
    AnimationObject/Transform/ChainedTransform.h \
    fifteen/EffectSwapper.h

FORMS    += MainWindow.ui \
    FormFinalImage.ui \
    DialogSettingsFifteen.ui \
    ImageWidget.ui \
    DialogGameStart.ui \
    DialogSettingsMineSweeper.ui \
    DialogImageHistory.ui \
    DialogSavedata.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    gameCore.qmodel \
    Games.qmodel
