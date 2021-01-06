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

#ifndef FRAMEIMAGESELECTOR_H
#define FRAMEIMAGESELECTOR_H

#include <QFrame>

namespace Ui {
class FrameImageSelector;
}

class FrameImageSelector : public QFrame
{
    Q_OBJECT

public:
    explicit FrameImageSelector(QWidget *parent = nullptr);
    ~FrameImageSelector() override;

    bool hasImage() const;
    const QPixmap &pixmap() const;
    QString imageFullPathName() const;

signals:
    void imageChanged();

private:
    Ui::FrameImageSelector *ui;

private slots:
    void onImageChanged();
};

#endif // FRAMEIMAGESELECTOR_H
