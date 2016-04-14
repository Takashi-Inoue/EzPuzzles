#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T20:19:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EzPuzzles
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    FormFinalImage.cpp \
    DialogSettingsFifteen.cpp \
    ImageWidget.cpp \
    SplitPainter.cpp \
    GameWidget.cpp \
    GameFifteen.cpp \
    SlideShuffler.cpp \
    PieceMover.cpp \
    PiecesGame.cpp \
    GameSwap.cpp \
    SwapShuffler.cpp \
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
    ImageFragmentPiece.cpp

HEADERS  += MainWindow.h \
    FormFinalImage.h \
    DialogSettingsFifteen.h \
    ImageWidget.h \
    SplitPainter.h \
    GameWidget.h \
    IGame.h \
    GameFifteen.h \
    IShuffler.h \
    SlideShuffler.h \
    PieceMover.h \
    PiecesGame.h \
    GameSwap.h \
    SwapShuffler.h \
    IPiece.h \
    DialogGameStart.h \
    DialogSettingsMineSweeper.h \
    IGameBuilder.h \
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
    ImageFragmentPiece.h

FORMS    += MainWindow.ui \
    FormFinalImage.ui \
    DialogSettingsFifteen.ui \
    ImageWidget.ui \
    DialogGameStart.ui \
    DialogSettingsMineSweeper.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    model.qmodel
