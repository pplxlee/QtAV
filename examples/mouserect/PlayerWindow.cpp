/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QtAVWidgets>

using namespace QtAV;

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    m_unit = 1000;
    setWindowTitle(QString::fromLatin1("QtAV simple player example"));
    m_player = new AVPlayer(this);
    m_player->setInterruptTimeout(-1);
    m_player->setInterruptOnTimeout(false);
    m_player->setBufferValue(1);
    m_player->masterClock()->setClockAuto(true);
    m_player->masterClock()->setClockType(AVClock::VideoClock);
    QVBoxLayout *vl = new QVBoxLayout();
    setLayout(vl);
    m_vr = VideoRenderer::create(VideoRendererId_Direct2D);
//    m_vr->setMouseRectColor(QColor(100, 100, 250));
    QVariantHash opt;
    QVariantHash ffmpegopt;
    ffmpegopt["skip_frame"] = 0;
    ffmpegopt["skip_idct"] = 0;
    ffmpegopt["skip_loop_filter"] = 0;
    ffmpegopt["threads"] = 3;
    ffmpegopt["thread_type"] = 1;
    opt["FFmpeg"] = ffmpegopt;
    m_player->setOptionsForVideoCodec(opt);
    if (!m_vr->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vr);
    vl->addWidget(m_vr->widget());
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(m_slider, SIGNAL(sliderPressed()), SLOT(seekBySlider()));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider(qint64)));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));
    connect(m_player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));

    vl->addWidget(m_slider);
    QHBoxLayout *hb = new QHBoxLayout();
    vl->addLayout(hb);
    m_openBtn = new QPushButton(tr("Open"));
    m_playBtn = new QPushButton(tr("Play/Pause"));
    m_stopBtn = new QPushButton(tr("Stop"));
    hb->addWidget(m_openBtn);
    hb->addWidget(m_playBtn);
    hb->addWidget(m_stopBtn);
    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
}

void PlayerWindow::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;
    m_player->play(file);
}

void PlayerWindow::seekBySlider(int value)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value*m_unit));
}

void PlayerWindow::seekBySlider()
{
    seekBySlider(m_slider->value());
}

void PlayerWindow::playPause()
{
    if (!m_player->isPlaying()) {
        m_player->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
}

void PlayerWindow::updateSlider(qint64 value)
{
    m_slider->setRange(0, int(m_player->duration()/m_unit));
    m_slider->setValue(int(value/m_unit));
}

void PlayerWindow::updateSlider()
{
    updateSlider(m_player->position());
}

void PlayerWindow::updateSliderUnit()
{
    m_unit = m_player->notifyInterval();
    updateSlider();
}

void PlayerWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        QPoint pos = m_vr->widget()->mapFromParent(event->pos());
        m_mouseRect.setTopLeft(pos);
        m_mouseRect.setBottomRight(pos);
        m_vr->setMouseRect(m_mouseRect);
    }
    else {
        QWidget::mousePressEvent(event);
    }
}

void PlayerWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if(m_objectRects.size() >= 30) {
            m_objectRects.clear();
        }
        m_objectRects.append({m_mouseRect, QColor(200 * m_objectRects.size() / 30 + 50,
                              50, 200 * (30 - m_objectRects.size()) / 30 + 50)});
//        m_vr->setObjectRectsWidth(m_objectRects.size() * .5);
        m_vr->setObjectRects(m_objectRects);
        m_mouseRect = QRect();
        m_vr->setMouseRect(m_mouseRect);
    }
    else {
        QWidget::mouseReleaseEvent(event);
    }
}

void PlayerWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) {
        QWidget::mouseDoubleClickEvent(event);
        return;
    }

    bool visible = this->isFullScreen();
    if(this->isFullScreen()) {
        foreach (QObject *obj, this->children()) {
            if(obj->isWidgetType() && obj != m_vr->widget())
                static_cast<QWidget *>(obj)->setVisible(visible);
        }
        this->layout()->setMargin(3);
        this->showNormal();
    }
    else {
        foreach (QObject *obj, this->children()) {
            if(obj->isWidgetType() && obj != m_vr->widget())
                static_cast<QWidget *>(obj)->setVisible(visible);
        }
        this->layout()->setMargin(0);
        this->showFullScreen();
    }
}

void PlayerWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 条件 !(m_mouseRect == QRect())可以防止双击时的触发
    // 双击时，会在release的情况下，触发一次有LeftButton按下的event，导致绘制一次不正常的框选
    if(!(m_mouseRect == QRect()) && (event->buttons() &= Qt::LeftButton)) {
        QPoint pos = m_vr->widget()->mapFromParent(event->pos());
        m_mouseRect.setBottomRight(pos);
        m_vr->setMouseRect(m_mouseRect);
    }
    else {
        QWidget::mouseMoveEvent(event);
    }
}
