/*
 * Castet - Load and draw SVG
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

#include "castet-animate-element.h"
#include "castet-circle-element.h"
#include "castet-desc-element.h"
#include "castet-g-element.h"
#include "castet-path-element.h"
#include "castet-polyline-element.h"
#include "castet-rect-element.h"
#include "castet-script-element.h"
#include "castet-svg-element.h"
#include "castet-title-element.h"
#include "castet-document.h"

G_DEFINE_TYPE (CastetDocument, castet_document, CASTET_TYPE_DOM_DOCUMENT)

#define DOCUMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      CASTET_TYPE_DOCUMENT, \
                                      CastetDocumentPrivate))

struct _CastetDocumentPrivate
{
};

/*
 * CastetDomDocument overloading
 */

static CastetDomElement *
castet_document_create_element(CastetDomDocument  *self,
                               const gchar        *tag_name,
                               GError            **err)
{
    if (strcmp (tag_name, "svg") == 0)
        return castet_svg_element_new ();
    if (strcmp (tag_name, "g") == 0)
        return castet_g_element_new ();
    if (strcmp (tag_name, "path") == 0)
        return castet_path_element_new ();
    if (strcmp (tag_name, "rect") == 0)
        return castet_rect_element_new ();
    if (strcmp (tag_name, "animate") == 0)
        return castet_animate_element_new ();
    if (strcmp (tag_name, "polyline") == 0)
        return castet_polyline_element_new ();
    if (strcmp (tag_name, "circle") == 0)
        return castet_circle_element_new ();
    if (strcmp (tag_name, "desc") == 0)
        return castet_desc_element_new ();
    if (strcmp (tag_name, "script") == 0)
        return castet_script_element_new ();
    if (strcmp (tag_name, "title") == 0)
        return castet_title_element_new ();

    return NULL;
}

/*
 * GObject overloading
 */

static void
castet_document_get_property (GObject    *object,
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
castet_document_set_property (GObject      *object,
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
castet_document_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_document_parent_class)->dispose (object);
}

static void
castet_document_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_document_parent_class)->finalize (object);
}

static void
castet_document_class_init (CastetDocumentClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    CastetDomDocumentClass *document_class = CASTET_DOM_DOCUMENT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (CastetDocumentPrivate)); */

    object_class->get_property = castet_document_get_property;
    object_class->set_property = castet_document_set_property;
    object_class->dispose = castet_document_dispose;
    object_class->finalize = castet_document_finalize;

    document_class->create_element = castet_document_create_element;
}

static void
castet_document_init (CastetDocument *self)
{
    /* self->priv = DOCUMENT_PRIVATE (self); */
}

CastetDomDocument *
castet_document_new (void)
{
    return g_object_new (CASTET_TYPE_DOCUMENT, NULL);
}
