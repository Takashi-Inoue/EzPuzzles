#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T20:19:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EzPuzzles
TEMPLATE = app

DEFINES += Q_COMPILER_INITIALIZER_LISTS


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
    SwitchPiece.cpp \
    DialogGameStart.cpp \
    DialogSettingsMineSweeper.cpp \
    mine/EmptyPiece.cpp \
    mine/GameMineSweeper.cpp \
    mine/MinePiece.cpp \
    mine/WallPiece.cpp \
    Dragger.cpp \
    SubFrame.cpp \
    mine/GraduallyDrawer.cpp

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
    SwitchPiece.h \
    DialogGameStart.h \
    DialogSettingsMineSweeper.h \
    IGameBuilder.h \
    mine/AbstractMinePiece.h \
    mine/EmptyPiece.h \
    mine/GameMineSweeper.h \
    mine/MinePiece.h \
    mine/WallPiece.h \
    ISubWidget.h \
    Dragger.h \
    SubFrame.h \
    IDrawer.h \
    mine/GraduallyDrawer.h

FORMS    += MainWindow.ui \
    FormFinalImage.ui \
    DialogSettingsFifteen.ui \
    ImageWidget.ui \
    DialogGameStart.ui \
    DialogSettingsMineSweeper.ui

RESOURCES += \
    resource.qrc
