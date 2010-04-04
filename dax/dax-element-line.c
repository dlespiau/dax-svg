/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include "dax-internals.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-element-line.h"

G_DEFINE_TYPE (DaxElementLine, dax_element_line, DAX_TYPE_ELEMENT)

#define LINE_PRIVATE(o)                                         \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_LINE,    \
                                      DaxElementLinePrivate))

enum {
    PROP_0,

    PROP_X1,
    PROP_Y1,
    PROP_X2,
    PROP_Y2
};

struct _DaxElementLinePrivate
{
    ClutterUnits *x1, *y1;
    ClutterUnits *x2, *y2;
};

static void
dax_element_line_set_x1 (DaxElementLine     *self,
                         const ClutterUnits *x1)
{
    DaxElementLinePrivate *priv = self->priv;

    if (priv->x1)
        clutter_units_free (priv->x1);

    priv->x1 = clutter_units_copy (x1);
}

static void
dax_element_line_set_y1 (DaxElementLine     *self,
                         const ClutterUnits *y1)
{
    DaxElementLinePrivate *priv = self->priv;

    if (priv->y1)
        clutter_units_free (priv->y1);

    priv->y1 = clutter_units_copy (y1);
}

static void
dax_element_line_set_x2 (DaxElementLine     *self,
                         const ClutterUnits *x2)
{
    DaxElementLinePrivate *priv = self->priv;

    if (priv->x2)
        clutter_units_free (priv->x2);

    priv->x2 = clutter_units_copy (x2);
}

static void
dax_element_line_set_y2 (DaxElementLine     *self,
                         const ClutterUnits *y2)
{
    DaxElementLinePrivate *priv = self->priv;

    if (priv->y2)
        clutter_units_free (priv->y2);

    priv->y2 = clutter_units_copy (y2);
}

/*
 * GObject implementation
 */

static void
dax_element_line_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    DaxElementLine *self = DAX_ELEMENT_LINE (object);
    DaxElementLinePrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_X1:
        clutter_value_set_units (value, priv->x1);
        break;
    case PROP_Y1:
        clutter_value_set_units (value, priv->y1);
        break;
    case PROP_X2:
        clutter_value_set_units (value, priv->x2);
        break;
    case PROP_Y2:
        clutter_value_set_units (value, priv->y2);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_line_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    DaxElementLine *self = DAX_ELEMENT_LINE (object);

    switch (property_id)
    {
    case PROP_X1:
        dax_element_line_set_x1 (self, clutter_value_get_units (value));
        break;
    case PROP_Y1:
        dax_element_line_set_y1 (self, clutter_value_get_units (value));
        break;
    case PROP_X2:
        dax_element_line_set_x2 (self, clutter_value_get_units (value));
        break;
    case PROP_Y2:
        dax_element_line_set_y2 (self, clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_line_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_line_parent_class)->dispose (object);
}

static void
dax_element_line_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_line_parent_class)->finalize (object);
}

static void
dax_element_line_class_init (DaxElementLineClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementLinePrivate));

    object_class->get_property = dax_element_line_get_property;
    object_class->set_property = dax_element_line_set_property;
    object_class->dispose = dax_element_line_dispose;
    object_class->finalize = dax_element_line_finalize;

    pspec = dax_param_spec_boxed ("x1",
                                  "x1",
                                  "The x-axis coordinate of the start of the "
                                  "line",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_X1, pspec);

    pspec = dax_param_spec_boxed ("y1",
                                  "y1",
                                  "The y-axis coordinate of the start of the "
                                  "line",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_Y1, pspec);

    pspec = dax_param_spec_boxed ("x2",
                                  "x2",
                                  "The x-axis coordinate of the end of the "
                                  "line",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_X2, pspec);

    pspec = dax_param_spec_boxed ("y2",
                                  "y2",
                                  "The y-axis coordinate of the end of the "
                                  "line",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_Y2, pspec);
}

static void
dax_element_line_init (DaxElementLine *self)
{
    self->priv = LINE_PRIVATE (self);
}

DaxDomElement *
dax_element_line_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_LINE, NULL);
}

ClutterUnits *
dax_element_line_get_x1 (const DaxElementLine *line)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_LINE (line), NULL);

    return line->priv->x1;
}

ClutterUnits *
dax_element_line_get_y1 (const DaxElementLine *line)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_LINE (line), NULL);

    return line->priv->y1;
}

ClutterUnits *
dax_element_line_get_x2 (const DaxElementLine *line)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_LINE (line), NULL);

    return line->priv->x2;
}

ClutterUnits *
dax_element_line_get_y2 (const DaxElementLine *line)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_LINE (line), NULL);

    return line->priv->y2;
}
