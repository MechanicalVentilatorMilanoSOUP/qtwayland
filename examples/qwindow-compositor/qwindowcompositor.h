/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINDOWCOMPOSITOR_H
#define QWINDOWCOMPOSITOR_H

#include "waylandcompositor.h"
#include "waylandsurface.h"
#include "textureblitter.h"
#include "qopenglwindow.h"

#include <QtGui/private/qopengltexturecache_p.h>
#include <QObject>
#include <QOpenGLFunctions>

class QWindowCompositor : public QObject, public WaylandCompositor, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    QWindowCompositor(QOpenGLWindow *window);
private slots:
    void surfaceDestroyed(QObject *object);
    void surfaceMapped();
    void surfaceDamaged(const QRect &rect);

    void render();
protected:
    void surfaceDamaged(WaylandSurface *surface, const QRect &rect);
    void surfaceCreated(WaylandSurface *surface);

    WaylandSurface* surfaceAt(const QPoint &point, QPoint *local = 0);

    GLuint composeSurface(WaylandSurface *surface);
    void paintChildren(WaylandSurface *surface, WaylandSurface *window);


    bool eventFilter(QObject *obj, QEvent *event);
    QPointF toSurface(WaylandSurface *surface, const QPointF &pos) const;

    void changeCursor(const QImage &image, int hotspotX, int hotspotY);

private:
    QOpenGLWindow *m_window;
    QImage m_backgroundImage;
    GLuint m_backgroundTexture;
    QList<WaylandSurface *> m_surfaces;
    TextureBlitter *m_textureBlitter;
    QOpenGLTextureCache *m_textureCache;
    GLuint m_surface_fbo;
    QTimer m_renderScheduler;

    //Dragging windows around
    WaylandSurface *m_draggingWindow;
    bool m_dragKeyIsPressed;
    QPoint m_drag_diff;

};

#endif // QWINDOWCOMPOSITOR_H
