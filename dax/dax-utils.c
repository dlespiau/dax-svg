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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "dax-utils.h"

void
_dax_utils_skip_space (gchar **str)
{
	while (g_ascii_isspace (**str))
		(*str)++;
}

void
_dax_utils_skip_space_and_char (gchar **str,
                                gchar   skip_me)
{
	while (g_ascii_isspace (**str) || **str == skip_me)
		(*str)++;
}

void
_dax_utils_find_next_space (gchar **str)
{
    while ((**str) && !g_ascii_isspace (**str))
        (*str)++;
}

guint
_dax_utils_count_commas (const gchar *str)
{
    guint nb_commas = 0;

    while (strchr (str, ',')) {
        nb_commas++;
        str++;
    }

    return nb_commas;
}

guint
_dax_utils_count_words (const gchar *str)
{
    gboolean seen_space = TRUE;
    guint nb_words = 0;

    while (*str) {
        if (g_ascii_isspace (*str))
            seen_space = TRUE;
        else if (seen_space) {
            nb_words++;
            seen_space = FALSE;
        }
        str++;
    }

    return nb_words;
}

gboolean
_dax_utils_parse_simple_float (gchar  **string,
                               gfloat  *number)
{
    gchar *str = *string;
    gfloat value;

    if (!g_ascii_isdigit (*str))
        return FALSE;

    /* integer part */
    value = (gfloat) strtoul (str, (char **)&str, 10);

    if ((*str) == '.') {
        gfloat divisor = 0.1;

        /* 5. is not a valid number */
        if (!g_ascii_isdigit (*++str))
            return FALSE;

        while (g_ascii_isdigit (*str)) {
            value += (*str - '0') * divisor;
            divisor *= 0.1;
            str++;
        }
    }

    *string = str;
    *number = value;

    return TRUE;
}

gboolean
_dax_utils_parse_float (char   **string,
                        gfloat  *x)
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

	c = *string;

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

	*string = end;

	return TRUE;
}

/* That's a bit restrictive as a definition of an URI but that's the two
 * schemes we want to support anyway */
gboolean
_dax_utils_is_iri (const gchar *str)
{
    return g_str_has_prefix (str, "http://") ||
           g_str_has_prefix (str, "file://");
}

static void
dump_path2d_node (ClutterPath2DNode *node)
{
  switch (node->type)
    {
    case CLUTTER_PATH_MOVE_TO:
      g_print ("M %.2f,%.2f %.2f,%.2f\n", node->points[0].x, node->points[0].y,
                                          node->points[1].x, node->points[1].y);
      break;
    case CLUTTER_PATH_REL_MOVE_TO:
      g_print ("m %.2f,%.2f %.2f,%.2f\n", node->points[0].x, node->points[0].y,
                                          node->points[1].x, node->points[1].y);
      break;
    case CLUTTER_PATH_LINE_TO:
      g_print ("L %.2f,%.2f %.2f,%.2f\n", node->points[0].x, node->points[0].y,
                                          node->points[1].x, node->points[1].y);
      break;
    case CLUTTER_PATH_REL_LINE_TO:
      g_print ("l %.2f,%.2f %.2f,%.2f\n", node->points[0].x, node->points[0].y,
                                          node->points[1].x, node->points[1].y);
      break;
    case CLUTTER_PATH_CURVE_TO:
      g_print ("C %.2f,%.2f %.2f,%.2f %.2f,%.2f\n",
               node->points[0].x, node->points[0].y,
               node->points[1].x, node->points[1].y,
               node->points[2].x, node->points[2].y);
      break;
    case CLUTTER_PATH_REL_CURVE_TO:
      g_print ("c %.2f,%.2f %.2f,%.2f %.2f,%.2f\n",
               node->points[0].x, node->points[0].y,
               node->points[1].x, node->points[1].y,
               node->points[2].x, node->points[2].y);
      break;
    case CLUTTER_PATH_CLOSE:
      g_print ("z\n");
      break;
    default:
      g_assert_not_reached ();
    }

}

void
_dax_utils_dump_path_2d (ClutterPath2D *path)
{
  GSList *nodes, *e;

  g_message ("path %p", path);

  nodes = clutter_path_2d_get_nodes (path);
  for (e = nodes; e; e = g_slist_next (e))
    {
      ClutterPath2DNode *node = e->data;

      dump_path2d_node (node);
    }

  g_slist_free (nodes);
}
