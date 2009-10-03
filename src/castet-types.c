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

#include "castet-internals.h"
#include "castet-types.h"

#define CASTET_KNOT_SEQUENCE_FLAG_NONE          0x0
#define CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA      0x1

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

/*
 * CastetKnotSequence
 */
struct _CastetKnotSequence
{
  gfloat *data;
  guint   nb_knots;
  guint   flags;
};

/**
 * castet_knot_sequence_new_from_array:
 * @data: A float array
 * @nb_knots: size of @floats / 2
 *
 * Allocates a new #CastetKnotSequence using given float array and its size.
 * The newly created #CastetKnotSequence has a copy of @array.
 *
 * Return value: the newly allocated #CastetKnotSequence. Use
 *   castet_knot_sequence_free() to free the resources
 *
 * Since: 0.2
 */
CastetKnotSequence *
castet_knot_sequence_new_with_array (gfloat *data,
                                     guint   nb_knots)
{
    CastetKnotSequence *seq;

    seq = g_slice_new (CastetKnotSequence);
    seq->data = g_memdup (data, sizeof (gfloat) * nb_knots * 2);
    seq->nb_knots = nb_knots;
    seq->flags = CASTET_KNOT_SEQUENCE_FLAG_NONE;
    seq->flags |= CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA;

    return seq;
}

/**
 * castet_knot_sequence_new_from_static_array:
 * @data: A float array
 * @nb_knots: size of @floats / 2
 *
 * Allocates a new #CastetKnotSequence using given float array and its size.
 * The newly created #CastetKnotSequence does not own the array.
 *
 * Return value: the newly allocated #CastetKnotSequence. Use
 *   castet_knot_sequence_free() to free the resources
 *
 * Since: 0.2
 */
CastetKnotSequence *
castet_knot_sequence_new_from_static_array (gfloat *data,
                                            guint   nb_knots)
{
    CastetKnotSequence *seq;

    seq = g_slice_new (CastetKnotSequence);
    seq->data = data;
    seq->nb_knots = nb_knots;
    seq->flags = CASTET_KNOT_SEQUENCE_FLAG_NONE;

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
    gchar *cur = (gchar *)string;
    guint nb_commas, nb_floats = 0;

    nb_commas = str_count_commas (string);

    seq = g_slice_new (CastetKnotSequence);
    seq->data = g_malloc (sizeof (gfloat) * nb_commas * 2);
    seq->nb_knots = 0;
    seq->flags = CASTET_KNOT_SEQUENCE_FLAG_NONE;
    seq->flags |= CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA;

    while (str_parse_float_list (&cur, 2, seq->data + nb_floats) &&
           nb_floats < nb_commas * 2)
    {
        str_skip_space_and_char (&cur, ',');
        nb_floats += 2;
    }
    seq->nb_knots = nb_floats / 2;

    return seq;
}

/**
 * castet_knot_sequence_copy:
 * @seq: a #CastetKnotSequence
 *
 * Copies @seq
 *
 * Return value: a newly allocated copy of #CastetKnotSequence. Use
 *   castet_knot_sequence_free() to free the allocated resources
 *
 * Since: 0.2
 */
CastetKnotSequence *
castet_knot_sequence_copy (const CastetKnotSequence *seq)
{
    CastetKnotSequence *new_seq;

    if (G_LIKELY (seq != NULL)) {
        new_seq = g_slice_dup (CastetKnotSequence, seq);
        new_seq->nb_knots = seq->nb_knots;
        if (seq->flags & CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA)
            new_seq->data = g_memdup (seq->data,
                                      sizeof(gfloat) * seq->nb_knots * 2);
        else
            new_seq->data = seq->data;
        new_seq->flags = seq->flags;
        return new_seq;
    }

    return NULL;
}

/**
 * castet_knot_sequence_deep_copy:
 * @seq a #CastetKnotSequence
 *
 * Copies @seq
 *
 * Return value: a newly allocated copy of #CastetKnotSequence. Use
 *   castet_knot_sequence_free() to free the allocated resources
 *
 * Since: 0.2
 */
CastetKnotSequence *
castet_knot_sequence_deep_copy (const CastetKnotSequence *seq)
{
    CastetKnotSequence *new_seq;

    if (G_LIKELY (seq != NULL)) {
        new_seq = g_slice_dup (CastetKnotSequence, seq);
        new_seq->nb_knots = seq->nb_knots;
        new_seq->data = g_memdup (seq->data,
                                  sizeof(gfloat) * seq->nb_knots * 2);
        new_seq->flags = CASTET_KNOT_SEQUENCE_FLAG_NONE;
        new_seq->flags |= CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA;
        return new_seq;
    }

    return NULL;
}

/**
 * castet_knot_sequence_free:
 * @box: a #CastetKnotSequence
 *
 * Frees a #CastetKnotSequence allocated using castet_knot_sequence_new()
 * or castet_knot_sequence_copy()
 *
 * Since: 0.2
 */
void
castet_knot_sequence_free (CastetKnotSequence *seq)
{
    if (G_LIKELY (seq != NULL)) {
        if (seq->flags & CASTET_KNOT_SEQUENCE_FLAG_OWN_DATA)
            g_free (seq->data);
        g_slice_free (CastetKnotSequence, seq);
    }
}

GType
castet_knot_sequence_get_type (void)
{
    static GType our_type = 0;

    if (G_UNLIKELY (our_type == 0))
        our_type =
            g_boxed_type_register_static (ISS("CastetKnotSequence"),
                                          (GBoxedCopyFunc)castet_knot_sequence_copy,
                                          (GBoxedFreeFunc)castet_knot_sequence_free);
    return our_type;
}

const gfloat *
castet_knot_sequence_get_array (const CastetKnotSequence *seq)
{
    return seq->data;
}

const guint
castet_knot_sequence_get_size (const CastetKnotSequence *seq)
{
    return seq->nb_knots;
}
