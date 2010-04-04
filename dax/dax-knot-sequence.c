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

#include "dax-utils.h"
#include "dax-knot-sequence.h"

static void _register_transform_funcs (GType type);

G_DEFINE_TYPE_WITH_CODE (DaxKnotSequence,
                         dax_knot_sequence,
                         G_TYPE_OBJECT,
                         _register_transform_funcs (g_define_type_id))

#define KNOT_SEQUENCE_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_KNOT_SEQUENCE,   \
                                      DaxKnotSequencePrivate))

#define DAX_KNOT_SEQUENCE_FLAG_NONE              0x0
#define DAX_KNOT_SEQUENCE_FLAG_STATIC_ARRAY      0x1

struct _DaxKnotSequencePrivate
{
  gfloat *data;
  guint   nb_knots;
  guint   flags;
};

static gboolean
str_parse_float_list (gchar  **str,
                      guint    n_values,
                      gfloat  *values)
{
	char *ptr = *str;
	unsigned int i;

	_dax_utils_skip_space_and_char (str, ',');

	for (i = 0; i < n_values; i++) {
		if (!_dax_utils_parse_float (str, &values[i])) {
			*str = ptr;
			return FALSE;
		}
        _dax_utils_skip_space_and_char (str, ',');
	}

	return TRUE;
}

static void
dax_value_transform_knot_sequence_string (const GValue *src,
                                          GValue       *dest)
{
    gchar *string = dax_knot_sequence_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
dax_value_transform_string_knot_sequence (const GValue *src,
                                          GValue       *dest)
{
    DaxKnotSequence *seq;

    seq = dax_knot_sequence_new_from_string (g_value_get_string (src));

    g_value_take_object (dest, seq);
}

static void
_register_transform_funcs (GType type)
{
    g_value_register_transform_func (type, G_TYPE_STRING,
                                     dax_value_transform_knot_sequence_string);

    g_value_register_transform_func (G_TYPE_STRING, type,
                                     dax_value_transform_string_knot_sequence);
}

/*
 * GObject overloading
 */

static void
dax_knot_sequence_get_property (GObject    *object,
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
dax_knot_sequence_set_property (GObject      *object,
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
dax_knot_sequence_finalize (GObject *object)
{
    DaxKnotSequence *seq = DAX_KNOT_SEQUENCE (object);
    DaxKnotSequencePrivate *priv = seq->priv;

    if (!(priv->flags & DAX_KNOT_SEQUENCE_FLAG_STATIC_ARRAY))
        g_free (priv->data);

    G_OBJECT_CLASS (dax_knot_sequence_parent_class)->finalize (object);
}

static void
dax_knot_sequence_class_init (DaxKnotSequenceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxKnotSequencePrivate));

    object_class->get_property = dax_knot_sequence_get_property;
    object_class->set_property = dax_knot_sequence_set_property;
    object_class->finalize = dax_knot_sequence_finalize;
}

static void
dax_knot_sequence_init (DaxKnotSequence *self)
{
    self->priv = KNOT_SEQUENCE_PRIVATE (self);
}

DaxKnotSequence *
dax_knot_sequence_new (void)
{
    return g_object_new (DAX_TYPE_KNOT_SEQUENCE, NULL);
}

DaxKnotSequence *
dax_knot_sequence_new_from_static_array (gfloat *data,
                                         guint   nb_knots)
{
    DaxKnotSequence *seq;
    DaxKnotSequencePrivate *priv;

    seq = dax_knot_sequence_new ();
    priv = seq->priv;
    priv->data = data;
    priv->nb_knots = nb_knots;
    priv->flags |= DAX_KNOT_SEQUENCE_FLAG_STATIC_ARRAY;

    return seq;
}

/**
 * dax_knot_sequence_new_from_string:
 * @string: FIXME
 *
 * Return value: a new #DaxKnotSequence creating from @string
 *
 * Since: 0.2
 */
DaxKnotSequence *
dax_knot_sequence_new_from_string (const gchar *string)
{
    DaxKnotSequence *seq;
    DaxKnotSequencePrivate *priv;
    gchar *cur = (gchar *)string;
    guint nb_commas, nb_floats = 0;

    nb_commas = _dax_utils_count_commas (string);

    seq = dax_knot_sequence_new ();
    priv = seq->priv;
    priv->data = g_malloc (sizeof (gfloat) * nb_commas * 2);
    priv->nb_knots = 0;

    while (str_parse_float_list (&cur, 2, priv->data + nb_floats) &&
           nb_floats < nb_commas * 2)
    {
        _dax_utils_skip_space_and_char (&cur, ',');
        nb_floats += 2;
    }
    priv->nb_knots = nb_floats / 2;

    return seq;
}

const gfloat *
dax_knot_sequence_get_array (const DaxKnotSequence *seq)
{
    DaxKnotSequencePrivate *priv = seq->priv;

    return priv->data;
}

guint
dax_knot_sequence_get_size (const DaxKnotSequence *seq)
{
    DaxKnotSequencePrivate *priv = seq->priv;

    return priv->nb_knots;
}

gchar *
dax_knot_sequence_to_string (const DaxKnotSequence *seq)
{
    DaxKnotSequencePrivate *priv = seq->priv;
    GString *string;
    gchar *ret;
    guint i;

    /* approximate the lengh to reduce the number of reallocs:
     * '10.00,100.00 ' * nb_knots */
    string = g_string_sized_new  (priv->nb_knots * 11);
    for (i = 0; i < priv->nb_knots; i++)
        g_string_append_printf (string, "%.02f,%.02f ",
                                priv->data[i * 2],
                                priv->data[i * 2  + 1]);
    ret = string->str;
    g_string_free (string, FALSE);

    return ret;
}
