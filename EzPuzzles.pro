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
    AnimationObject/AbstractAnimationObject.cpp \
    AnimationObject/Animation/AnimationLineMove.cpp \
    AnimationObject/Animation/AnimationWarpMove.cpp \
    AnimationObject/Effect/CompositeEffect.cpp \
    AnimationObject/Effect/EffectGraduallyBlinkFrame.cpp \
    AnimationObject/Effect/EffectGraduallyImage.cpp \
    AnimationObject/Effect/EffectRoundMoveFrame.cpp \
    AnimationObject/Effect/EffectSimpleFrame.cpp \
    AnimationObject/Effect/TimeLimitedEffect.cpp \
    AnimationObject/Transform/AbstractTransform.cpp \
    AnimationObject/Transform/ChainedTransform.cpp \
    AnimationObject/Transform/TransformExpand.cpp \
    AnimationObject/Transform/WaitTransform.cpp \
    MineSweeper/Savers.cpp \
    commands/RemoveListWidgetItem.cpp \
    CommonPhase/PhaseCleared.cpp \
    CommonPhase/PhaseShowFinalImage.cpp \
    CommonPhase/PhaseShuffle.cpp \
    Fifteen/Board.cpp \
    Fifteen/EffectSwapper.cpp \
    Fifteen/FifteenPieceMover.cpp \
    Fifteen/FifteenSlideShuffler.cpp \
    Fifteen/FifteenSwapShuffler.cpp \
    Fifteen/PuzzlePiece.cpp \
    Fifteen/SaveDataFifteen.cpp \
    Fifteen/SimplePiecesFactory.cpp \
    Fifteen/SlideBlankPiece.cpp \
    ImageWidget/CellSelectionGrid.cpp \
    ImageWidget/ImageWidget.cpp \
    ImageWidget/SubFrame.cpp \
    MineSweeper/GameCoreMineSweeper.cpp \
    MineSweeper/GameDataMineSweeper.cpp \
    MineSweeper/MineField.cpp \
    MineSweeper/MinePiece.cpp \
    MineSweeper/MinePiecesFactory.cpp \
    MineSweeper/NumberPieceFactory.cpp \
    MineSweeper/PhaseMineSweeperEnding.cpp \
    MineSweeper/PhaseMineSweeperGaming.cpp \
    MineSweeper/SafePiece.cpp \
    MineSweeper/SaveDataMineSweeper.cpp \
    MineSweeper/WallPiece.cpp \
    SelectImage/ActionSelectImageFromHistory.cpp \
    SelectImage/ActionSelectImageFromLocal.cpp \
    Slide/GameDataSimpleSlide.cpp \
    Slide/PhaseSimpleSlideEnding.cpp \
    Slide/PhaseSimpleSlideGaming.cpp \
    Slide/SaveDataSimpleSlide.cpp \
    Swap/GameDataSimpleSwap.cpp \
    Swap/PhaseSimpleSwapEnding.cpp \
    Swap/PhaseSimpleSwapGaming.cpp \
    Swap/SaveDataSimpleSwap.cpp \
    Swap/SwapOperationSetEndingEffect.cpp \
    widgets/DialogStartGame.cpp \
    widgets/FormSettingsLikeFifteen.cpp \
    widgets/FormSettingsMineSweeper.cpp \
    widgets/FrameImageSelector.cpp \
    widgets/ListWidgetImageHistory.cpp \
    widgets/ListWidgetImages.cpp \
    AbstractSaveData.cpp \
    Application.cpp \
    BlockPiece.cpp \
    BoardInformation.cpp \
    BrokenSaveData.cpp \
    DialogImageHistory.cpp \
    DialogSavedata.cpp \
    Dragger.cpp \
    FinalImage.cpp \
    FormFinalImage.cpp \
    GameCore.cpp \
    GameID.cpp \
    GameWidget.cpp \
    GridLines.cpp \
    ImageFragmentPiece.cpp \
    ImageHistory.cpp \
    ImagePiece.cpp \
    MainWindow.cpp \
    NumberPiece.cpp \
    Position.cpp \
    SourceImage.cpp \
    StringListHistory.cpp \
    SwitchImagePiece.cpp \
    ThreadFrameTimer.cpp \
    ThreadGameInfoLoader.cpp \
    ThreadImageLoader.cpp \
    ThreadOperation.cpp \
    UniquePosition.cpp \
    UnknownSaveData.cpp


HEADERS  += MainWindow.h \
    AnimationObject/AbstractAnimationObject.h \
    AnimationObject/Animation/AbstractAnimation.h \
    AnimationObject/Animation/AnimationLineMove.h \
    AnimationObject/Animation/AnimationWarpMove.h \
    AnimationObject/Effect/AbstractEffect.h \
    AnimationObject/Effect/CompositeEffect.h \
    AnimationObject/Effect/EffectGraduallyBlinkFrame.h \
    AnimationObject/Effect/EffectGraduallyImage.h \
    AnimationObject/Effect/EffectRoundMoveFrame.h \
    AnimationObject/Effect/EffectSimpleFrame.h \
    AnimationObject/Effect/TimeLimitedEffect.h \
    AnimationObject/IAnimationObject.h \
    AnimationObject/Transform/AbstractTransform.h \
    AnimationObject/Transform/ChainedTransform.h \
    AnimationObject/Transform/TransformExpand.h \
    AnimationObject/Transform/WaitTransform.h \
    MineSweeper/Savers.h \
    commands/RemoveListWidgetItem.h \
    CommonPhase/PhaseCleared.h \
    CommonPhase/PhaseShowFinalImage.h \
    CommonPhase/PhaseShuffle.h \
    Fifteen/Board.h \
    Fifteen/EffectSwapper.h \
    Fifteen/FifteenAbstractShuffler.h \
    Fifteen/FifteenPieceMover.h \
    Fifteen/FifteenSlideShuffler.h \
    Fifteen/FifteenSwapShuffler.h \
    Fifteen/IOperationForPieces.h \
    Fifteen/IPuzzlePiece.h \
    Fifteen/PuzzlePiece.h \
    Fifteen/SaveDataFifteen.h \
    Fifteen/SimplePiecesFactory.h \
    Fifteen/SlideBlankPiece.h \
    ImageWidget/AbstractSubWidget.h \
    ImageWidget/CellSelectionGrid.h \
    ImageWidget/ImageWidget.h \
    ImageWidget/SubFrame.h \
    MineSweeper/AbstractMinePiece.h \
    MineSweeper/GameCoreMineSweeper.h \
    MineSweeper/GameDataMineSweeper.h \
    MineSweeper/IMinePiece.h \
    MineSweeper/MineField.h \
    MineSweeper/MinePiece.h \
    MineSweeper/MinePiecesFactory.h \
    MineSweeper/NumberPieceFactory.h \
    MineSweeper/PhaseMineSweeperEnding.h \
    MineSweeper/PhaseMineSweeperGaming.h \
    MineSweeper/SafePiece.h \
    MineSweeper/SaveDataMineSweeper.h \
    MineSweeper/WallPiece.h \
    SelectImage/AbstractSelectImageAction.h \
    SelectImage/AbstractSelectImageToolButton.h \
    SelectImage/ActionSelectImageFromHistory.h \
    SelectImage/ActionSelectImageFromLocal.h \
    SelectImage/ToolButtonSelectImageFromHistory.h \
    SelectImage/ToolButtonSelectImageFromLocal.h \
    Slide/GameDataSimpleSlide.h \
    Slide/PhaseSimpleSlideEnding.h \
    Slide/PhaseSimpleSlideGaming.h \
    Slide/SaveDataSimpleSlide.h \
    Swap/GameDataSimpleSwap.h \
    Swap/PhaseSimpleSwapEnding.h \
    Swap/PhaseSimpleSwapGaming.h \
    Swap/SaveDataSimpleSwap.h \
    Swap/SwapOperationSetEndingEffect.h \
    widgets/DialogStartGame.h \
    widgets/FormSettingsLikeFifteen.h \
    widgets/FormSettingsMineSweeper.h \
    widgets/FrameImageSelector.h \
    widgets/ListWidgetImageHistory.h \
    widgets/ListWidgetImages.h \
    AbstractPhase.h \
    AbstractSaveData.h \
    Application.h \
    BlockPiece.h \
    BoardInformation.h \
    BrokenSaveData.h \
    DialogImageHistory.h \
    DialogSavedata.h \
    Dragger.h \
    FinalImage.h \
    FormFinalImage.h \
    GameCore.h \
    GameID.h \
    GameWidget.h \
    GridLines.h \
    IGame.h \
    IGameData.h \
    ImageFragmentPiece.h \
    ImageHistory.h \
    ImagePiece.h \
    IPiece.h \
    IShuffler.h \
    ISwitchPiece.h \
    IWidgetGameSettings.h \
    NumberPiece.h \
    Position.h \
    SourceImage.h \
    StringListHistory.h \
    SwitchImagePiece.h \
    ThreadFrameTimer.h \
    ThreadGameInfoLoader.h \
    ThreadImageLoader.h \
    ThreadOperation.h \
    UniquePosition.h \
    UnknownSaveData.h \
    Utility.h

FORMS    += MainWindow.ui \
    DialogImageHistory.ui \
    DialogSavedata.ui \
    widgets/DialogStartGame.ui \
    widgets/FormSettingsLikeFifteen.ui \
    widgets/FormSettingsMineSweeper.ui \
    widgets/FrameImageSelector.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    gameCore.qmodel \
    Games.qmodel

VERSION = 0.2.0.0
RC_ICONS = res/icons/icon.ico
QMAKE_TARGET_PRODUCT = EzPuzzles
QMAKE_TARGET_COPYRIGHT = "Copyright 2016-2021 Takashi Inoue"
