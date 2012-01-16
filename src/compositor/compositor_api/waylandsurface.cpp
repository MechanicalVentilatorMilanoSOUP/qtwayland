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

#include "waylandsurface.h"

#include <private/qobject_p.h>

#include "wayland_wrapper/wlsurface.h"
#include "wayland_wrapper/wlextendedsurface.h"
#include "wayland_wrapper/wlsubsurface.h"
#include "wayland_wrapper/wlcompositor.h"

#ifdef QT_COMPOSITOR_QUICK
#include "waylandsurfaceitem.h"
#endif

class WaylandSurfacePrivate : public QObjectPrivate
{
public:
    WaylandSurfacePrivate(Wayland::Surface *srfc)
        : surface(srfc)
#ifdef QT_COMPOSITOR_QUICK
        , surface_item(0)
#endif
    {}

    ~WaylandSurfacePrivate()
    {
#ifdef QT_COMPOSITOR_QUICK
        if (surface_item)
            surface_item->setSurface(0);
#endif
    }

    Wayland::Surface *surface;
#ifdef QT_COMPOSITOR_QUICK
    WaylandSurfaceItem *surface_item;
#endif
};

WaylandSurface::WaylandSurface(Wayland::Surface *surface)
    : QObject(*new WaylandSurfacePrivate(surface))
{

}

WaylandSurface *WaylandSurface::parentSurface() const
{
    Q_D(const WaylandSurface);
    if (d->surface->subSurface()) {
        return d->surface->subSurface()->parent()->waylandSurface();
    }
    return 0;
}

QLinkedList<WaylandSurface *> WaylandSurface::subSurfaces() const
{
    Q_D(const WaylandSurface);
    if (d->surface->subSurface()) {
        return d->surface->subSurface()->subSurfaces();
    }
    return QLinkedList<WaylandSurface *>();
}

WaylandSurface::Type WaylandSurface::type() const
{
    Q_D(const WaylandSurface);
    return d->surface->type();
}

bool WaylandSurface::isYInverted() const
{
    Q_D(const WaylandSurface);
    return d->surface->isYInverted();
}

bool WaylandSurface::visible() const
{
    Q_D(const WaylandSurface);
    return d->surface->visible();
}

QPointF WaylandSurface::pos() const
{
    Q_D(const WaylandSurface);
    return d->surface->pos();
}

void WaylandSurface::setPos(const QPointF &pos)
{
    Q_D(WaylandSurface);
    d->surface->setPos(pos);
}

QSize WaylandSurface::size() const
{
    Q_D(const WaylandSurface);
    return d->surface->size();
}

void WaylandSurface::setSize(const QSize &size)
{
    Q_D(WaylandSurface);
    d->surface->setSize(size);
}

QImage WaylandSurface::image() const
{
    Q_D(const WaylandSurface);
    return d->surface->image();
}

#ifdef QT_COMPOSITOR_WAYLAND_GL
GLuint WaylandSurface::texture(QOpenGLContext *context) const
{
    Q_D(const WaylandSurface);
    return d->surface->textureId(context);
}
#else //QT_COMPOSITOR_WAYLAND_GL
uint WaylandSurface::texture(QOpenGLContext *) const
{
    return 0;
}
#endif

Wayland::Surface * WaylandSurface::handle() const
{
    Q_D(const WaylandSurface);
    return d->surface;
}

#ifdef QT_COMPOSITOR_QUICK
WaylandSurfaceItem *WaylandSurface::surfaceItem() const
{
    Q_D(const WaylandSurface);
    return d->surface_item;
}

void WaylandSurface::setSurfaceItem(WaylandSurfaceItem *surfaceItem)
{
    Q_D(WaylandSurface);
    d->surface_item = surfaceItem;
}
#endif //QT_COMPOSITOR_QUICK

qint64 WaylandSurface::processId() const
{
    Q_D(const WaylandSurface);
    return d->surface->processId();
}

QByteArray WaylandSurface::authenticationToken() const
{
    Q_D(const WaylandSurface);
    return d->surface->authenticationToken();
}

QVariantMap WaylandSurface::windowProperties() const
{
    Q_D(const WaylandSurface);
    return d->surface->windowProperties();
}

void WaylandSurface::setWindowProperty(const QString &name, const QVariant &value)
{
    Q_D(WaylandSurface);
    d->surface->setWindowProperty(name, value);
}

QPointF WaylandSurface::mapToParent(const QPointF &pos) const
{
    return pos + this->pos();
}

QPointF WaylandSurface::mapTo(WaylandSurface *parent, const QPointF &pos) const
{
    QPointF p = pos;
    if (parent) {
        const WaylandSurface * surface = this;
        while (surface != parent) {
            Q_ASSERT_X(surface, "WaylandSurface::mapTo(WaylandSurface *parent, const QPoint &pos)",
                       "parent must be in parent hierarchy");
            p = surface->mapToParent(p);
            surface = surface->parentSurface();
        }
    }
    return p;

}

WaylandCompositor *WaylandSurface::compositor() const
{
    Q_D(const WaylandSurface);
    return d->surface->compositor()->qtCompositor();
}

void WaylandSurface::frameFinished()
{
    Q_D(WaylandSurface);
    d->surface->frameFinished();
}

void WaylandSurface::sendOnScreenVisibilityChange(bool visible)
{
    Q_D(WaylandSurface);
    d->surface->sendOnScreenVisibilityChange(visible);
}
