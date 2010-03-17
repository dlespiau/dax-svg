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

#include <math.h>
#include <string.h>

#include "castet-knot-sequence.h"

static void _register_transform_funcs (GType type);

G_DEFINE_TYPE_WITH_CODE (CastetKnotSequence,
                         castet_knot_sequence,
                         G_TYPE_OBJECT,
                         _register_transform_funcs (g_define_type_id))

#define KNOT_SEQUENCE_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      CASTET_TYPE_KNOT_SEQUENCE,    \
                                      CastetKnotSequencePrivate))

#define CASTET_KNOT_SEQUENCE_FLAG_NONE              0x0
#define CASTET_KNOT_SEQUENCE_FLAG_STATIC_ARRAY      0x1

#if 0
static void
str_skip_space (char **str)
{
	while (g_ascii_isspace (**str))
		(*str)++;
}
#endif

static void
str_skip_space_and_char (char  **str,
                         gchar   skip_me)
{
	while (g_ascii_isspace (**str) || **str == skip_me)
		(*str)++;
}

static gboolean
str_parse_float (char **str, gfloat *x)
{
	char *end, *c;
	gboolean integer_part = FALSE;
	gboolean fractional_part = FALSE;
	gboolean exponent_part = FALSE;
	gfloat mantissa = 0.0;
	gfloat exponent = 0.0;
	gfloat divisor;
	gboolean mantissa_sign = 1.0;
	gboolean exponent_sign = 1.0;

	c = *str;

	if (*c == '-') {
		mantissa_sign = -1.0;
		c++;
	} else if (*c == '+')
		c++;

	if (*c >= '0' && *c <= '9') {
		integer_part = TRUE;
		mantissa = *c - '0';
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa = mantissa * 10.0 + *c - '0';
			c++;
		}
	}


	if (*c == '.')
		c++;
	else if (!integer_part)
		return FALSE;

	if (*c >= '0' && *c <= '9') {
		fractional_part = TRUE;
		mantissa += (*c - '0') * 0.1;
		divisor = 0.01;
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa += (*c - '0') * divisor;
			divisor *= 0.1;
			c++;
		}
	}

	if (!fractional_part && !integer_part)
		return FALSE;

	end = c;

	if (*c == 'E' || *c == 'e') {
		c++;

		if (*c == '-') {
			exponent_sign = -1.0;
			c++;
		} else if (*c == '+')
			c++;

		if (*c >= '0' && *c <= '9') {
			exponent_part = TRUE;
			exponent = *c - '0';
			c++;

			while (*c >= '0' && *c <= '9') {
				exponent = exponent * 10.0 + *c - '0';
				c++;
			}
		}
	}

	if (exponent_part) {
		end = c;
		*x = mantissa_sign * mantissa * pow (10.0, exponent_sign * exponent);
	} else
		*x = mantissa_sign * mantissa;

	*str = end;

	return TRUE;
}

static gboolean
str_parse_float_list (gchar  **str,
                      guint    n_values,
                      gfloat  *values)
{
	char *ptr = *str;
	unsigned int i;

	str_skip_space_and_char (str, ',');

	for (i = 0; i < n_values; i++) {
		if (!str_parse_float (str, &values[i])) {
			*str = ptr;
			return FALSE;
		}
        str_skip_space_and_char (str, ',');
	}

	return TRUE;
}

static guint
str_count_commas (const gchar *str)
{
    guint nb_commas = 0;

    while (strchr (str, ',')) {
        nb_commas++;
        str++;
    }

    return nb_commas;
}
struct _CastetKnotSequencePrivate
{
  gfloat *data;
  guint   nb_knots;
  guint   flags;
};

static void
castet_value_transform_knot_sequence_string (const GValue *src,
                                             GValue       *dest)
{
    gchar *string = castet_knot_sequence_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
castet_value_transform_string_knot_sequence (const GValue *src,
                                             GValue       *dest)
{
    CastetKnotSequence *seq;

    seq = castet_knot_sequence_new_from_string (g_value_get_string (src));

    g_value_take_object (dest, seq);
}

static void
_register_transform_funcs (GType type)
{
    g_value_register_transform_func (type, G_TYPE_STRING,
                                     castet_value_transform_knot_sequence_string);

    g_value_register_transform_func (G_TYPE_STRING, type,
                                     castet_value_transform_string_knot_sequence);
}

/*
 * GObject overloading
 */

static void
castet_knot_sequence_get_property (GObject    *object,
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
castet_knot_sequence_set_property (GObject      *object,
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
castet_knot_sequence_finalize (GObject *object)
{
    CastetKnotSequence *seq = CASTET_KNOT_SEQUENCE (object);
    CastetKnotSequencePrivate *priv = seq->priv;

    if (!(priv->flags & CASTET_KNOT_SEQUENCE_FLAG_STATIC_ARRAY))
        g_free (priv->data);

    G_OBJECT_CLASS (castet_knot_sequence_parent_class)->finalize (object);
}

static void
castet_knot_sequence_class_init (CastetKnotSequenceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetKnotSequencePrivate));

    object_class->get_property = castet_knot_sequence_get_property;
    object_class->set_property = castet_knot_sequence_set_property;
    object_class->finalize = castet_knot_sequence_finalize;
}

static void
castet_knot_sequence_init (CastetKnotSequence *self)
{
    self->priv = KNOT_SEQUENCE_PRIVATE (self);
}

CastetKnotSequence *
castet_knot_sequence_new (void)
{
    return g_object_new (CASTET_TYPE_KNOT_SEQUENCE, NULL);
}

CastetKnotSequence *
castet_knot_sequence_new_from_static_array (gfloat *data,
                                            guint   nb_knots)
{
    CastetKnotSequence *seq;
    CastetKnotSequencePrivate *priv;

    seq = castet_knot_sequence_new ();
    priv = seq->priv;
    priv->data = data;
    priv->nb_knots = nb_knots;
    priv->flags |= CASTET_KNOT_SEQUENCE_FLAG_STATIC_ARRAY;

    return seq;
}

/**
 * castet_knot_sequence_new_from_string:
 * @string: FIXME
 *
 * Return value: the newly allocated #CastetKnotSequence. Use
 *   castet_knot_sequence_unref() to free the resources
 *
 * Since: 0.2
 */
CastetKnotSequence *
castet_knot_sequence_new_from_string (const gchar *string)
{
    CastetKnotSequence *seq;
    CastetKnotSequencePrivate *priv;
    gchar *cur = (gchar *)string;
    guint nb_commas, nb_floats = 0;

    nb_commas = str_count_commas (string);

    seq = castet_knot_sequence_new ();
    priv = seq->priv;
    priv->data = g_malloc (sizeof (gfloat) * nb_commas * 2);
    priv->nb_knots = 0;

    while (str_parse_float_list (&cur, 2, priv->data + nb_floats) &&
           nb_floats < nb_commas * 2)
    {
        str_skip_space_and_char (&cur, ',');
        nb_floats += 2;
    }
    priv->nb_knots = nb_floats / 2;

    return seq;
}

gfloat *
castet_knot_sequence_get_array (const CastetKnotSequence *seq)
{
    CastetKnotSequencePrivate *priv = seq->priv;

    return priv->data;
}

guint
castet_knot_sequence_get_size (const CastetKnotSequence *seq)
{
    CastetKnotSequencePrivate *priv = seq->priv;

    return priv->nb_knots;
}

gchar *
castet_knot_sequence_to_string (const CastetKnotSequence *seq)
{
    CastetKnotSequencePrivate *priv = seq->priv;
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
