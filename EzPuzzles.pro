#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T20:19:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += ezpz_ja.ts

SOURCES += main.cpp\
    AbstractSaveData.cpp \
    Application.cpp \
    CellSelectionGrid.cpp \
    ImageHistory.cpp \
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
    ThreadFrameTimer.cpp \
    ThreadGameInfoLoader.cpp \
    ThreadImageLoader.cpp \
    ThreadOperation.cpp \
    ImageFragmentPiece.cpp \
    MineSweeper/MineLocker.cpp \
    DialogImageHistory.cpp \
    StringListHistory.cpp \
    DialogSavedata.cpp \
    SourceImage.cpp \
    commands/RemoveListWidgetItem.cpp \
    fifteen/PuzzlePiece.cpp \
    MineSweeper/MinePiecesFactory.cpp \
    fifteen/SaveDataFifteen.cpp \
    fifteen/SimplePiecesFactory.cpp \
    fifteen/FifteenSlideShuffler.cpp \
    fifteen/FifteenPieceMover.cpp \
    GameID.cpp \
    fifteen/FifteenSwapShuffler.cpp \
    BrokenSaveData.cpp \
    UnknownSaveData.cpp \
    AnimationObject/AbstractAnimationObject.cpp \
    AnimationObject/Animation/AnimationLineMove.cpp \
    AnimationObject/Animation/AnimationWarpMove.cpp \
    AnimationObject/Effect/EffectSimpleFrame.cpp \
    AnimationObject/Effect/EffectGraduallyBlinkFrame.cpp \
    BoardInformation.cpp \
    Position.cpp \
    fifteen/SlideBlankPiece.cpp \
    AnimationObject/Effect/CompositeEffect.cpp \
    GridLines.cpp \
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
    fifteen/EffectSwapper.cpp \
    MineSweeper/PhaseMineSweeperEnding.cpp \
    AnimationObject/Effect/EffectGraduallyImage.cpp \
    AnimationObject/Transform/WaitTransform.cpp \
    AnimationObject/Effect/TimeLimitedEffect.cpp \
    widgets/ListWidgetImageHistory.cpp \
    widgets/ListWidgetImages.cpp

HEADERS  += MainWindow.h \
    AbstractSaveData.h \
    Application.h \
    CellSelectionGrid.h \
    FormFinalImage.h \
    DialogSettingsFifteen.h \
    ImageHistory.h \
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
    ImagePiece.h \
    NumberPiece.h \
    ISwitchPiece.h \
    BlockPiece.h \
    SwitchImagePiece.h \
    MineSweeper/IMinePiece.h \
    MineSweeper/SafePiece.h \
    MineSweeper/NumberPieceFactory.h \
    ThreadFrameTimer.h \
    ThreadGameInfoLoader.h \
    ThreadImageLoader.h \
    ThreadOperation.h \
    ImageFragmentPiece.h \
    MineSweeper/MineLocker.h \
    Utility.h \
    DialogImageHistory.h \
    StringListHistory.h \
    DialogSavedata.h \
    SourceImage.h \
    commands/RemoveListWidgetItem.h \
    fifteen/IPuzzlePiece.h \
    fifteen/PuzzlePiece.h \
    MineSweeper/MinePiecesFactory.h \
    fifteen/SaveDataFifteen.h \
    fifteen/SimplePiecesFactory.h \
    fifteen/FifteenSlideShuffler.h \
    fifteen/FifteenPieceMover.h \
    GameID.h \
    fifteen/FifteenSwapShuffler.h \
    IDialogGameSettings.h \
    BrokenSaveData.h \
    UnknownSaveData.h \
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
    AnimationObject/Effect/CompositeEffect.h \
    GridLines.h \
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
    fifteen/EffectSwapper.h \
    MineSweeper/PhaseMineSweeperEnding.h \
    AnimationObject/Effect/EffectGraduallyImage.h \
    AnimationObject/Transform/WaitTransform.h \
    AnimationObject/Effect/TimeLimitedEffect.h \
    widgets/ListWidgetImageHistory.h \
    widgets/ListWidgetImages.h

FORMS    += MainWindow.ui \
    DialogSettingsFifteen.ui \
    DialogGameStart.ui \
    DialogSettingsMineSweeper.ui \
    DialogImageHistory.ui \
    DialogSavedata.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    gameCore.qmodel \
    Games.qmodel

VERSION = 0.1.0.0
#RC_ICONS = res/images/icon.ico
QMAKE_TARGET_PRODUCT = EzPuzzles
QMAKE_TARGET_COPYRIGHT = "Copyright 2016 Takashi Inoue"
