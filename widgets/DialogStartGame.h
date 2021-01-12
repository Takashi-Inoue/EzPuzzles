/*
 * Copyright 2021 Takashi Inoue
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

#ifndef DIALOGSTARTGAME_H
#define DIALOGSTARTGAME_H

#include <QDialog>

namespace Ui {
class DialogStartGame;
}

class IGame;

class DialogStartGame : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStartGame(QWidget *parent = nullptr);
    DialogStartGame(QStringView imageFullPathName, QWidget *parent = nullptr);
    ~DialogStartGame() override;

    QSharedPointer<IGame> buildGame() const;

protected:
    void done(int result) override;

private:
    static constexpr char m_geometryKey[] = "DialogStartGame_Geometry";

    Ui::DialogStartGame *ui;
};

#endif // DIALOGSTARTGAME_H
