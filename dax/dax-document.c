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

#include <string.h>

#include "dax-dom.h"

#include "dax-element-animate.h"
#include "dax-element-circle.h"
#include "dax-element-desc.h"
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
#include "dax-element-title.h"
#include "dax-element-tspan.h"
#include "dax-internals.h"
#include "dax-paramspec.h"
#include "dax-private.h"

#include "dax-document.h"

G_DEFINE_TYPE (DaxDocument, dax_document, DAX_TYPE_DOM_DOCUMENT)

#define DOCUMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_DOCUMENT,    \
                                      DaxDocumentPrivate))

struct _DaxDocumentPrivate
{
};

/*
 * DaxDomDocument implementation
 */

static DaxDomElement *
dax_document_create_element(DaxDomDocument  *self,
                            const gchar     *tag_name,
                            GError         **err)
{
    if (strcmp (tag_name, "svg") == 0)
        return dax_element_svg_new ();
    if (strcmp (tag_name, "g") == 0)
        return dax_element_g_new ();
    if (strcmp (tag_name, "path") == 0)
        return dax_element_path_new ();
    if (strcmp (tag_name, "rect") == 0)
        return dax_element_rect_new ();
    if (strcmp (tag_name, "text") == 0)
        return dax_element_text_new ();
    if (strcmp (tag_name, "tspan") == 0)
        return dax_element_tspan_new ();
    if (strcmp (tag_name, "animate") == 0)
        return dax_element_animate_new ();
    if (strcmp (tag_name, "polyline") == 0)
        return dax_element_polyline_new ();
    if (strcmp (tag_name, "image") == 0)
        return dax_element_image_new ();
    if (strcmp (tag_name, "circle") == 0)
        return dax_element_circle_new ();
    if (strcmp (tag_name, "handler") == 0)
        return dax_element_handler_new ();
    if (strcmp (tag_name, "script") == 0)
        return dax_element_script_new ();
    if (strcmp (tag_name, "desc") == 0)
        return dax_element_desc_new ();
    if (strcmp (tag_name, "title") == 0)
        return dax_element_title_new ();
    if (strcmp (tag_name, "line") == 0)
        return dax_element_line_new ();

    return NULL;
}

/*
 * GObject overloading
 */

static void
dax_document_get_property (GObject    *object,
                           guint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_document_set_property (GObject      *object,
                           guint         property_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_document_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_document_parent_class)->dispose (object);
}

static void
dax_document_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_document_parent_class)->finalize (object);
}

static void
dax_document_class_init (DaxDocumentClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomDocumentClass *document_class = DAX_DOM_DOCUMENT_CLASS (klass);

#if 0
    g_type_class_add_private (klass, sizeof (DaxDocumentPrivate));
#endif

    object_class->get_property = dax_document_get_property;
    object_class->set_property = dax_document_set_property;
    object_class->dispose = dax_document_dispose;
    object_class->finalize = dax_document_finalize;

    document_class->create_element = dax_document_create_element;
}

static void
dax_document_init (DaxDocument *self)
{
#if 0
    self->priv = DOCUMENT_PRIVATE (self);
#endif
}

DaxDomDocument *
dax_document_new (void)
{
    return g_object_new (DAX_TYPE_DOCUMENT, NULL);
}
