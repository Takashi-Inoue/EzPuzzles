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
#include "DialogImageHistory.h"
#include "ui_DialogImageHistory.h"

#include "EzPuzzles.h"
#include "StringListHistory.h"

#include <QApplication>
#include <QFileInfo>
#include <QPainter>
#include <QPushButton>

#include <QCloseEvent>
#include <QMouseEvent>
#include <QDebug>

DialogImageHistory::DialogImageHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageHistory),
    shortcutRemove(QKeySequence(Qt::Key_Delete), this, SLOT(removeSelected())),
    isHistoryChanged(false)
{
    ui->setupUi(this);

    ui->listWidget->viewport()->setMouseTracking(true);
    ui->listWidget->viewport()->installEventFilter(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    shortcutRemove.setEnabled(false);

    setupRemoveButton();

    iconBase = QPixmap(ui->listWidget->iconSize());
    iconBase.fill(QColor(0, 0, 0, 0));

    StringListHistory history;
    history.load(EzPuzzles::imageHistoryPath());

    imagePaths = history.stringList();

    if (imagePaths.isEmpty())
        return;

    QIcon icon(iconBase);

    for (const auto &string : imagePaths) {
        ui->listWidget->addItem(new QListWidgetItem(icon, ""));
        imageLoader.append(string);
    }

    imageLoader.moveToThread(&loadThread);
    connect(&imageLoader, SIGNAL(loaded(QString,QPixmap)), this, SLOT(imageLoaded(QString,QPixmap)));
    connect(&imageLoader, SIGNAL(failedToLoad(QString)),   this, SLOT(removeImagePath(QString)));

    loadThread.start();
    imageLoader.start();
}

DialogImageHistory::~DialogImageHistory()
{
    imageLoader.stop();
    imageLoader.wait();
    loadThread.quit();
    loadThread.wait();

    delete ui;
}

QString DialogImageHistory::selectedImagePath() const
{
    auto rows = ui->listWidget->selectionModel()->selectedRows();

    if (rows.isEmpty())
        return "";

    return imagePaths.at(rows.front().row());
}

bool DialogImageHistory::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != ui->listWidget->viewport() || event->type() != QEvent::MouseMove)
        return QDialog::eventFilter(obj, event);

    QMouseEvent *e = static_cast<QMouseEvent *>(event);

    static int oldRow = -1;

    int nowRow = ui->listWidget->indexAt(e->pos()).row();

    if (oldRow == nowRow)
        return true;

    oldRow = nowRow;

    if (nowRow == -1) {
        buttonRemove.close();
        return true;
    }

    int width  = ui->listWidget->sizeHintForColumn(0);
    int height = ui->listWidget->sizeHintForRow(0);

    int x = e->pos().x() / width;
    int y = e->pos().y() / height;

    x = (x + 1) * width - buttonRemove.width() - 2;
    y = y * height + 2;

    buttonRemove.move(x, y);

    if (!buttonRemove.isVisible())
        buttonRemove.show();

    return true;
}

void DialogImageHistory::done(int result)
{
    if (isHistoryChanged && result == QDialog::Accepted) {
        StringListHistory history;

        history.addStrings(imagePaths, false);
        history.save(EzPuzzles::imageHistoryPath());
    }

    QDialog::done(result);
}

void DialogImageHistory::imageLoaded(const QString &imagePath, const QPixmap &pixmap)
{
    const int edgeWidth = 5;

    QPixmap iconImage(pixmap.scaled(iconImageSize(edgeWidth), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QRect iconImageRect(iconImageTopLeft(iconImage.size()), iconImage.size());

    iconBase.fill(QColor(0, 0, 0, 0));
    drawIconEdge(edgeWidth, iconImageRect);

    QPainter painter(&iconBase);
    painter.drawPixmap(iconImageRect, iconImage);

    int index = imagePaths.indexOf(imagePath);

    if (index != -1)
        ui->listWidget->item(index)->setIcon(QIcon(iconBase));
}

void DialogImageHistory::removeImagePath(const QString &imagePath)
{
    int index = -1;

    while ((index = imagePaths.indexOf(imagePath)) != -1)
        removeHistory(index);
}

void DialogImageHistory::removeSelected()
{
    auto indexes = ui->listWidget->selectionModel()->selectedRows();

    if (indexes.isEmpty())
        return;

    removeHistory(indexes.at(0).row()); // single select
}

void DialogImageHistory::onRemoveButtonPressed()
{
    removeHistory(ui->listWidget->indexAt(buttonRemove.pos()).row());
}

void DialogImageHistory::on_listWidget_doubleClicked(const QModelIndex &/*index*/)
{
    accept();
}

void DialogImageHistory::on_listWidget_itemSelectionChanged()
{
    bool hasSelection = ui->listWidget->selectionModel()->hasSelection();

    shortcutRemove.setEnabled(hasSelection);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isHistoryChanged | hasSelection);
}

void DialogImageHistory::removeHistory(int index)
{
    Q_ASSERT(index >= 0 && index < imagePaths.size());

    imagePaths.removeAt(index);
    ui->listWidget->model()->removeRow(index);

    isHistoryChanged = true;

    if (buttonRemove.isVisible() && !ui->listWidget->indexAt(buttonRemove.pos()).isValid())
        buttonRemove.close();
}

void DialogImageHistory::setupRemoveButton()
{
    buttonRemove.setParent(ui->listWidget);
    buttonRemove.close();
    buttonRemove.setToolTip(tr("Remove this image from history"));
    buttonRemove.setGeometry(0, 0, 16, 16);
    buttonRemove.setIconSize(QSize(12, 12));
    buttonRemove.setIcon(QIcon(":ico/exit"));

    connect(&buttonRemove, SIGNAL(pressed()), this, SLOT(onRemoveButtonPressed()));
}

void DialogImageHistory::drawIconEdge(int edgeWidth, QRect iconImageRect)
{
    QPainter painter(&iconBase);

    iconImageRect += QMargins(edgeWidth, edgeWidth, edgeWidth, edgeWidth);
    painter.fillRect(iconImageRect, QApplication::palette().midlight());

    iconImageRect -= QMargins(1, 1, 1, 1);
    painter.fillRect(iconImageRect, QApplication::palette().light());
}

QSize DialogImageHistory::iconImageSize(int edgeWidth) const
{
    return iconBase.size() - QSize(edgeWidth * 2, edgeWidth * 2);
}

QPoint DialogImageHistory::iconImageTopLeft(const QSize &iconPixmapSize) const
{
    QSize sz = (iconBase.size() - iconPixmapSize) / 2;

    return QPoint(sz.width(), sz.height());
}
