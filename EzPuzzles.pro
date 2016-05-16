#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T20:19:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EzPuzzles
TEMPLATE = app

TRANSLATIONS += ezpz_ja.ts

SOURCES += main.cpp\
        MainWindow.cpp \
    FormFinalImage.cpp \
    DialogSettingsFifteen.cpp \
    ImageWidget.cpp \
    GameWidget.cpp \
    DialogGameStart.cpp \
    DialogSettingsMineSweeper.cpp \
    mine/GameMineSweeper.cpp \
    mine/MinePiece.cpp \
    mine/WallPiece.cpp \
    Dragger.cpp \
    SubFrame.cpp \
    mine/GraduallyDrawer.cpp \
    ImagePiece.cpp \
    NumberPiece.cpp \
    BlockPiece.cpp \
    SwitchImagePiece.cpp \
    mine/SafePiece.cpp \
    mine/NumberPieceFactory.cpp \
    ThreadOperation.cpp \
    GarbageCollector.cpp \
    ImageFragmentPiece.cpp \
    mine/MineLocker.cpp \
    DialogImageHistory.cpp \
    ImageLoader.cpp \
    StringListHistory.cpp \
    EzPuzzles.cpp \
    ToolIconWindow.cpp \
    DialogSavedata.cpp \
    SourceImage.cpp \
    fifteen/PuzzlePiece.cpp \
    mine/MinePiecesFactory.cpp \
    GridSplitter.cpp \
    fifteen/GameLikeFifteen.cpp \
    fifteen/SimplePiecesFactory.cpp \
    fifteen/GameSimpleSlide.cpp \
    fifteen/FifteenSlideShuffler.cpp \
    fifteen/FifteenPieceMover.cpp \
    GameID.cpp \
    fifteen/GameSimpleSwap.cpp \
    fifteen/FifteenSwapShuffler.cpp \
    GameInfoLoader.cpp \
    BrokenSaveData.cpp \
    UnknownSaveData.cpp \
    mine/MineSweeperSaveData.cpp \
    fifteen/SimpleSwapSaveData.cpp \
    fifteen/SimpleSlideSaveData.cpp \
    MoveToTrashBox.cpp

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
    mine/GameMineSweeper.h \
    mine/MinePiece.h \
    mine/WallPiece.h \
    ISubWidget.h \
    Dragger.h \
    SubFrame.h \
    IDrawer.h \
    mine/GraduallyDrawer.h \
    ImagePiece.h \
    NumberPiece.h \
    ISwitchPiece.h \
    BlockPiece.h \
    SwitchImagePiece.h \
    mine/IMinePiece.h \
    mine/SafePiece.h \
    mine/NumberPieceFactory.h \
    ThreadOperation.h \
    GarbageCollector.h \
    ImageFragmentPiece.h \
    mine/MineLocker.h \
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
    mine/MinePiecesFactory.h \
    GridSplitter.h \
    fifteen/GameLikeFifteen.h \
    fifteen/SimplePiecesFactory.h \
    fifteen/GameSimpleSlide.h \
    fifteen/FifteenIShuffler.h \
    fifteen/FifteenSlideShuffler.h \
    fifteen/FifteenPieceMover.h \
    GameID.h \
    fifteen/GameSimpleSwap.h \
    fifteen/FifteenSwapShuffler.h \
    IDialogGameSettings.h \
    GameInfoLoader.h \
    ISaveData.h \
    BrokenSaveData.h \
    UnknownSaveData.h \
    mine/MineSweeperSaveData.h \
    fifteen/SimpleSwapSaveData.h \
    fifteen/SimpleSlideSaveData.h \
    MoveToTrashBox.h

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
    GameLikeFifteen.qmodel \
    Pieces.qmodel \
    gameCore.qmodel
