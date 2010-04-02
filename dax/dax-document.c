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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <string.h>

#include "dax-animate-element.h"
#include "dax-circle-element.h"
#include "dax-desc-element.h"
#include "dax-g-element.h"
#include "dax-handler-element.h"
#include "dax-path-element.h"
#include "dax-polyline-element.h"
#include "dax-rect-element.h"
#include "dax-script-element.h"
#include "dax-svg-element.h"
#include "dax-title-element.h"
#include "dax-document.h"

G_DEFINE_TYPE (DaxDocument, dax_document, DAX_TYPE_DOM_DOCUMENT)

#define DOCUMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_DOCUMENT, \
                                      DaxDocumentPrivate))

struct _DaxDocumentPrivate
{
};

/*
 * DaxDomDocument overloading
 */

static DaxDomElement *
dax_document_create_element(DaxDomDocument  *self,
                               const gchar        *tag_name,
                               GError            **err)
{
    if (strcmp (tag_name, "svg") == 0)
        return dax_svg_element_new ();
    if (strcmp (tag_name, "g") == 0)
        return dax_g_element_new ();
    if (strcmp (tag_name, "path") == 0)
        return dax_path_element_new ();
    if (strcmp (tag_name, "rect") == 0)
        return dax_rect_element_new ();
    if (strcmp (tag_name, "animate") == 0)
        return dax_animate_element_new ();
    if (strcmp (tag_name, "polyline") == 0)
        return dax_polyline_element_new ();
    if (strcmp (tag_name, "circle") == 0)
        return dax_circle_element_new ();
    if (strcmp (tag_name, "handler") == 0)
        return dax_handler_element_new ();
    if (strcmp (tag_name, "script") == 0)
        return dax_script_element_new ();
    if (strcmp (tag_name, "desc") == 0)
        return dax_desc_element_new ();
    if (strcmp (tag_name, "title") == 0)
        return dax_title_element_new ();

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

    /* g_type_class_add_private (klass, sizeof (DaxDocumentPrivate)); */

    object_class->get_property = dax_document_get_property;
    object_class->set_property = dax_document_set_property;
    object_class->dispose = dax_document_dispose;
    object_class->finalize = dax_document_finalize;

    document_class->create_element = dax_document_create_element;
}

static void
dax_document_init (DaxDocument *self)
{
    /* self->priv = DOCUMENT_PRIVATE (self); */
}

DaxDomDocument *
dax_document_new (void)
{
    return g_object_new (DAX_TYPE_DOCUMENT, NULL);
}
