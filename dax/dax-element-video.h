/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DAX_ELEMENT_VIDEO_H__
#define __DAX_ELEMENT_VIDEO_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_VIDEO dax_element_video_get_type()

#define DAX_ELEMENT_VIDEO(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_VIDEO,    \
                                 DaxElementVideo))

#define DAX_ELEMENT_VIDEO_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_VIDEO,   \
                              DaxElementVideoClass))

#define DAX_IS_ELEMENT_VIDEO(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_VIDEO))

#define DAX_IS_ELEMENT_VIDEO_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_VIDEO))

#define DAX_ELEMENT_VIDEO_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_ELEMENT_VIDEO, \
                                DaxElementVideoClass))

typedef struct _DaxElementVideo DaxElementVideo;
typedef struct _DaxElementVideoClass DaxElementVideoClass;
typedef struct _DaxElementVideoPrivate DaxElementVideoPrivate;

struct _DaxElementVideo
{
    DaxElement parent;

    DaxElementVideoPrivate *priv;
};

struct _DaxElementVideoClass
{
    DaxElementClass parent_class;
};

GType               dax_element_video_get_type      (void) G_GNUC_CONST;

DaxDomElement *     dax_element_video_new           (void);

ClutterUnits *      dax_element_video_get_x         (DaxElementVideo *video);
ClutterUnits *      dax_element_video_get_y         (DaxElementVideo *video);
ClutterUnits *      dax_element_video_get_width     (DaxElementVideo *video);
ClutterUnits *      dax_element_video_get_height    (DaxElementVideo *video);
const gchar *       dax_element_video_get_uri       (DaxElementVideo *video);

G_END_DECLS

#endif /* __DAX_ELEMENT_VIDEO_H__ */
