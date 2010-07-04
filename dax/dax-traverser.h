/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#ifndef __DAX_TRAVERSER_H__
#define __DAX_TRAVERSER_H__

#include <glib-object.h>

#include "dax-dom-node.h"

#include "dax-element-animate.h"
#include "dax-element-circle.h"
#include "dax-element-g.h"
#include "dax-element-handler.h"
#include "dax-element-image.h"
#include "dax-element-line.h"
#include "dax-element-path.h"
#include "dax-element-polyline.h"
#include "dax-element-rect.h"
#include "dax-element-script.h"
#include "dax-element-svg.h"
#include "dax-element-text.h"
#include "dax-types.h"

G_BEGIN_DECLS

#define DAX_TYPE_TRAVERSER dax_traverser_get_type()

#define DAX_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_TRAVERSER, DaxTraverser))

#define DAX_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_TRAVERSER, DaxTraverserClass))

#define DAX_IS_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_TRAVERSER))

#define DAX_IS_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_TRAVERSER))

#define DAX_TRAVERSER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_TRAVERSER, DaxTraverserClass))

typedef struct _DaxTraverser DaxTraverser;
typedef struct _DaxTraverserClass DaxTraverserClass;
typedef struct _DaxTraverserPrivate DaxTraverserPrivate;

struct _DaxTraverser
{
    GObject parent;

    DaxTraverserPrivate *priv;
};

struct _DaxTraverserClass
{
    GObjectClass parent_class;

    void (*traverse_svg)        (DaxTraverser  *traverser,
                                 DaxElementSvg *node);
    void (*traverse_g)          (DaxTraverser *traverser,
                                 DaxElementG  *node);
    void (*traverse_path)       (DaxTraverser   *traverser,
                                 DaxElementPath *node);
    void (*traverse_rect)       (DaxTraverser   *traverser,
                                 DaxElementRect *node);
    void (*traverse_polyline)   (DaxTraverser       *traverser,
                                 DaxElementPolyline *node);
    void (*traverse_animate)    (DaxTraverser      *traverser,
                                 DaxElementAnimate *node);
    void (*traverse_circle)     (DaxTraverser     *traverser,
                                 DaxElementCircle *node);
    void (*traverse_script)     (DaxTraverser     *traverser,
                                 DaxElementScript *node);
    void (*traverse_handler)    (DaxTraverser      *traverser,
                                 DaxElementHandler *node);
    void (*traverse_line)       (DaxTraverser   *traverser,
                                 DaxElementLine *node);
    void (*traverse_text)       (DaxTraverser   *traverser,
                                 DaxElementText *node);
    void (*traverse_image)      (DaxTraverser    *traverser,
                                 DaxElementImage *node);
};

GType               dax_traverser_get_type          (void) G_GNUC_CONST;

DaxTraverser *      dax_traverser_new               (DaxDomNode *root);
const DaxMatrix *   dax_traverser_get_ctm           (DaxTraverser *self);
void                dax_traverser_set_root          (DaxTraverser *self,
                                                     DaxDomNode   *root);
void                dax_traverser_apply             (DaxTraverser *self);

void                dax_traverser_traverse_svg      (DaxTraverser  *self,
                                                     DaxElementSvg *node);
void                dax_traverser_traverse_g        (DaxTraverser *self,
                                                     DaxElementG  *node);
void                dax_traverser_traverse_path     (DaxTraverser   *self,
                                                     DaxElementPath *node);
void                dax_traverser_traverse_rect     (DaxTraverser   *self,
                                                     DaxElementRect *node);
void                dax_traverser_traverse_polyline (DaxTraverser      *self,
                                                     DaxElementPolyline *node);
void                dax_traverser_traverse_animate  (DaxTraverser      *self,
                                                     DaxElementAnimate *node);
void                dax_traverser_traverse_circle   (DaxTraverser     *self,
                                                     DaxElementCircle *node);
void                dax_traverser_traverse_script   (DaxTraverser     *self,
                                                     DaxElementScript *node);
void                dax_traverser_traverse_handler  (DaxTraverser      *self,
                                                     DaxElementHandler *node);
void                dax_traverser_traverse_line     (DaxTraverser   *self,
                                                     DaxElementLine *node);
void                dax_traverser_traverse_text     (DaxTraverser   *self,
                                                     DaxElementText *node);
void                dax_traverser_traverse_image    (DaxTraverser    *self,
                                                     DaxElementImage *image);
G_END_DECLS

#endif /* __DAX_TRAVERSER_H__ */
