/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <string.h>

#include <gio/gio.h>

#include "dax-dom.h"
#include "dax-internals.h"
#include "dax-debug.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-js-context.h"
#include "dax-document.h"
#include "dax-element-svg.h"
#include "dax-element.h"

static void dax_xml_event_listener_init (DaxXmlEventListenerIface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (
        DaxElement,
        dax_element,
        DAX_TYPE_DOM_ELEMENT,
        G_IMPLEMENT_INTERFACE (DAX_TYPE_XML_EVENT_LISTENER,
                               dax_xml_event_listener_init)
)

#define ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_ELEMENT,     \
                                      DaxElementPrivate))

/*
 * FIXME: {get,set}_base_iri() probably belongs to DomElement
 */

enum
{
    PROP_0,

    PROP_FILL,
    PROP_FILL_OPACITY,
    PROP_STROKE,
    PROP_BASE_IRI,

    /* legacy event handlers */
    PROP_ONLOAD,
};

struct _DaxElementPrivate
{
    ClutterColor *fill;
    gfloat fill_opacity;
    ClutterColor *stroke;
    gchar *base_iri;                /* xml:base value in the DOM tree */
    gchar *resolved_base_iri;       /* cache resolved xml:base */

    gchar *onload_handler;
};

static void
clear_resolved_iri (DaxElement *element)
{
    DaxElementPrivate *priv = element->priv;

    if (priv->resolved_base_iri == NULL)
        return;

    /* resolved_base_iri can be base_iri in the case base_iri is an [UI]RI */
    if (priv->base_iri == priv->resolved_base_iri)
        g_free (priv->resolved_base_iri);

    priv->resolved_base_iri = NULL;
}

static void
on_load_event (DaxElement *element,
               gboolean    loaded,
               gpointer    user_data)
{
    DaxDomElement *dom_element = (DaxDomElement *) element;
    DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (element);
    DaxXmlEvent load_event;

    dax_xml_event_from_type (&load_event, DAX_XML_EVENT_TYPE_LOAD, target);

    dax_dom_element_handle_event (dom_element,
                                  dax_xml_event_copy (&load_event));
}

static void
dax_element_set_onload_handler (DaxElement  *element,
                                const gchar *script)
{
    DaxElementPrivate *priv = element->priv;

    if (priv->onload_handler) {
        g_free (priv->onload_handler);
    } else {
        /* time to add the listener */
        DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (element);
        DaxXmlEventListener *listener = DAX_XML_EVENT_LISTENER (element);

        dax_xml_event_target_add_event_listener (target,
                                                 "load",
                                                 listener,
                                                 FALSE);
    }

    priv->onload_handler = g_strdup_printf ("function __dax_handler(event) {"
                                                "let evt=event;"
                                                "%s"
                                            "}",
                                            script);

    if (dax_dom_element_get_loaded ((DaxDomElement *) element)) {
        on_load_event (element, TRUE, NULL);
    } else  {
        g_signal_connect (element, "loaded",
                          G_CALLBACK (on_load_event), NULL);
    }
}

/*
 * XmlEventListener implementation
 */

/* handle legacy event attributes */

static void
dax_element_handle_event (DaxXmlEventListener *listener,
                          DaxXmlEvent         *xml_event)
{
    DaxElement *element = DAX_ELEMENT (listener);
    DaxElementPrivate *priv = element->priv;
    DaxDomElement *target;
    DaxJsContext *js_context;
    DaxJsObject *event;
    gchar *code;

    target = (DaxDomElement *) element;

    switch (xml_event->type) {
    case DAX_XML_EVENT_TYPE_LOAD:
        code = priv->onload_handler;
        break;

    case DAX_XML_EVENT_TYPE_NONE:
    case DAX_XML_EVENT_TYPE_CLICK:
    default:
        g_warning ("Unhandled event %d", xml_event->type);
        return;
    }

    js_context = dax_js_context_get_default ();
    event = dax_js_context_new_object_from_xml_event (js_context, xml_event);

    dax_js_context_eval (js_context, code, strlen (code), "svg", NULL, NULL);
    dax_js_context_call_function (js_context, "__dax_handler", "o", event);
}

static void
dax_xml_event_listener_init (DaxXmlEventListenerIface *iface)
{
    iface->handle_event = dax_element_handle_event;
}

/*
 * DaxDomElement overloading
 */

static const gchar *
dax_element_get_attribute (DaxDomElement *self,
                           const gchar   *name)
{
#if 0
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
#endif
    return NULL;
}

static void
dax_element_set_attribute (DaxDomElement  *self,
                           const gchar    *name,
                           const gchar    *value,
                           GError        **err)
{
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
    GValue string_value = { 0, };
    GValue new_value = { 0, };
    gboolean success;

    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return;
    }

    /* we don't want to duplicate the string here */
    g_value_init (&string_value, G_TYPE_STRING);
    g_value_set_static_string (&string_value, value);

    /* this GValue holds the new value of the property we want to set */
    g_value_init (&new_value, pspec->value_type);

    success = g_value_transform (&string_value, &new_value);

    /* fallback to using the from_string() vfunc of DaxParamSpecClass */
    if (success == FALSE && DAX_IS_PARAM_SPEC_ARRAY (pspec)) {
        DaxParamSpecClass *pspec_klass;

        pspec_klass = DAX_PARAM_SPEC_GET_CLASS (pspec);
        success = pspec_klass->from_string (pspec, value, &new_value);
    }

    if (success == FALSE) {
        /* FIXME exception ? */
        g_warning ("Could not transform a string into a %s",
                   g_type_name (pspec->value_type));
        return;
    }

    DAX_NOTE (PARSING, "set %s to %s on %s",
              name,
              value,
              G_OBJECT_TYPE_NAME (self));

    g_object_set_property (G_OBJECT (self), name, &new_value);
}

/*
 * GObject overloading
 */

static void
dax_element_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    DaxElement *element = DAX_ELEMENT (object);
    DaxElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_FILL:
        clutter_value_set_color (value, priv->fill);
        break;
    case PROP_STROKE:
        clutter_value_set_color (value, priv->stroke);
        break;
    case PROP_FILL_OPACITY:
        g_value_set_float (value, priv->fill_opacity);
        break;
    case PROP_BASE_IRI:
        g_value_set_string (value, dax_element_get_base_iri (element));
        break;

    case PROP_ONLOAD:
        g_value_set_string (value, priv->onload_handler);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
    DaxElement *element = DAX_ELEMENT (object);
    DaxElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_FILL:
    {
        const ClutterColor *color;
        color = clutter_value_get_color (value);
        priv->fill = clutter_color_copy (color);
        break;
    }
    case PROP_STROKE:
    {
        const ClutterColor *color;
        color = clutter_value_get_color (value);
        priv->stroke = clutter_color_copy (color);
        break;
    }
    case PROP_FILL_OPACITY:
        priv->fill_opacity = g_value_get_float (value);
        break;
    case PROP_BASE_IRI:
        /* Remove cached base_iri if needed */
        clear_resolved_iri (element);
        /* Time to get a new iri */
        if (priv->base_iri)
            g_free (priv->base_iri);
        priv->base_iri = g_value_dup_string (value);
        break;

    case PROP_ONLOAD:
       dax_element_set_onload_handler (element, g_value_get_string (value));
       break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_parent_class)->dispose (object);
}

static void
dax_element_finalize (GObject *object)
{
    DaxElement *element = DAX_ELEMENT (object);
    DaxElementPrivate *priv = element->priv;

    if (priv->base_iri) {
        g_free (priv->base_iri);
        priv->base_iri = NULL;
    }
    clear_resolved_iri (element);

    G_OBJECT_CLASS (dax_element_parent_class)->finalize (object);
}

static void
dax_element_class_init (DaxElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomElementClass *dom_element_class = DAX_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementPrivate));

    object_class->get_property = dax_element_get_property;
    object_class->set_property = dax_element_set_property;
    object_class->dispose = dax_element_dispose;
    object_class->finalize = dax_element_finalize;

    dom_element_class->get_attribute = dax_element_get_attribute;
    dom_element_class->set_attribute = dax_element_set_attribute;

    pspec = g_param_spec_boxed ("fill",
                                "Fill color",
                                "The fill color of the element",
                                CLUTTER_TYPE_COLOR,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL, pspec);

    pspec = g_param_spec_boxed ("stroke",
                                "Stoke color",
                                "The color of the outline of the element",
                                CLUTTER_TYPE_COLOR,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_STROKE, pspec);

    pspec = g_param_spec_float ("fill-opacity",
                                "Fill opacity",
                                "The opacity of the painting operation used to "
                                "paint the interior of the element",
                                0.0f, 1.0f,
                                1.0f,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL_OPACITY, pspec);

    pspec = dax_param_spec_string ("base",
                                   "Base IRI",
                                   "base IRI other than the base IRI of the "
                                   "document or external entity",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_NONE,
                                   xml_ns);
    g_object_class_install_property (object_class, PROP_BASE_IRI, pspec);

    pspec = dax_param_spec_string ("onload",
                                   "onload",
                                   "Event fired when the element is loaded",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_VERSION_1_0 |
                                   DAX_PARAM_VERSION_1_1,
                                   svg_ns);
    g_object_class_install_property (object_class, PROP_ONLOAD, pspec);
}

static void
dax_element_init (DaxElement *self)
{
    DaxDomNode *node = DAX_DOM_NODE (self);

    node->namespace_uri = svg_ns;
    self->priv = ELEMENT_PRIVATE (self);
}

DaxDomElement *
dax_element_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT, NULL);
}

const ClutterColor *
dax_element_get_fill_color (DaxElement *element)
{
    DaxElementPrivate *priv;
    DaxDomNode *parent;

    g_return_val_if_fail (DAX_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->fill)
        return priv->fill;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((DaxDomNode  *)element)->parent_node;
    if (DAX_IS_ELEMENT_SVG (parent))
        return NULL;

    return dax_element_get_fill_color (DAX_ELEMENT (parent));
}

const ClutterColor *
dax_element_get_stroke_color (DaxElement *element)
{
    DaxElementPrivate *priv;
    DaxDomNode *parent;

    g_return_val_if_fail (DAX_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->stroke)
        return priv->stroke;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((DaxDomNode  *)element)->parent_node;
    if (DAX_IS_ELEMENT_SVG (parent))
        return NULL;

    return dax_element_get_stroke_color (DAX_ELEMENT (parent));
}

gfloat
dax_element_get_fill_opacity (DaxElement *element)
{
    g_return_val_if_fail (DAX_IS_ELEMENT (element), 1.0f);

    return element->priv->fill_opacity;
}

/* That's a bit restrictive as a definition of an URI but that's the two
 * schemes we want to support anyway */
static gboolean
is_iri (const char *str)
{
    return g_str_has_prefix (str, "http://") ||
           g_str_has_prefix (str, "file://");
}

static const char *
get_parent_base_iri (DaxElement *element)
{
    DaxDomNode *parent;

    parent = ((DaxDomNode *) element)->parent_node;
    if (DAX_IS_DOCUMENT (parent))
        return dax_document_get_base_iri ((DaxDocument *) parent);
    else
        return dax_element_get_base_iri ((DaxElement *) parent);
}

const gchar *
dax_element_get_base_iri (DaxElement *element)
{
    DaxElementPrivate *priv;

    g_return_val_if_fail (DAX_IS_ELEMENT (element), NULL);

    priv = element->priv;

    if (priv->resolved_base_iri)
        return priv->resolved_base_iri;

    /* If xml:base is set, it's time to do some work */
    if (priv->base_iri) {

        if (is_iri (priv->base_iri)) {
            /* the value on the attribute is an actual uri */
            priv->resolved_base_iri = priv->base_iri;
        } else {
            /* relative uri to resolve */
            const gchar *parent_base_uri;
            GFile *resolved_file, *parent_file;

            parent_base_uri = get_parent_base_iri (element);
            parent_file = g_file_new_for_uri (parent_base_uri);

            resolved_file = g_file_resolve_relative_path (parent_file,
                                                          priv->base_iri);
            priv->resolved_base_iri = g_file_get_uri (resolved_file);

            g_object_unref (parent_file);
            g_object_unref (resolved_file);
        }

        return priv->resolved_base_iri;
    }

    return get_parent_base_iri (element);
}

/*
 * TraitAccess
 */

gfloat
dax_element_getFloatTrait (DaxElement *element,
                           const char *name)
{
    GObjectClass *object_class;
    GParamSpec *pspec;
    gfloat value = 0.0f;

    /* FIXME, ensure @name is a trait */

    object_class = G_OBJECT_GET_CLASS (element);
    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return 0.0f;
    }

    if (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_FLOAT) {
        g_object_get (element, name, &value, NULL);
    } else if ((G_PARAM_SPEC_TYPE (pspec) == CLUTTER_TYPE_PARAM_UNITS) ||
               (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_BOXED)) {
        ClutterUnits *units;

        g_object_get (element, name, &units, NULL);
        value = clutter_units_to_pixels (units);
    }

    return value;
}

void
dax_element_setFloatTrait (DaxElement *element,
                           const char *name,
                           gfloat      value)
{
    GObjectClass *object_class;
    GParamSpec *pspec;

    object_class = G_OBJECT_GET_CLASS (element);
    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return;
    }

    if (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_FLOAT) {
        g_object_set (element, name, value, NULL);
    } else if ((G_PARAM_SPEC_TYPE (pspec) == CLUTTER_TYPE_PARAM_UNITS) ||
               (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_BOXED)) {
        ClutterUnits units;

        clutter_units_from_pixels (&units, value);
        g_object_set (element, name, &units, NULL);
    }
}
