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

#include "dax-internals.h"
#include "dax-utils.h"
#include "dax-types.h"

/*
 * DaxDuration
 */

DaxDurationType
dax_duration_get_unit_type (const DaxDuration *dur)
{
    g_return_val_if_fail (dur, DAX_DURATION_MS);

    return dur->unit_type;
}

gfloat
dax_duration_get_unit_value (const DaxDuration *dur)
{
    g_return_val_if_fail (dur, 0.0f);

    return dur->value;
}

DaxDuration *
dax_duration_copy (const DaxDuration *dur)
{
    if (dur == NULL)
        return NULL;

    return g_slice_dup (DaxDuration, dur);
}

void
dax_duration_free (DaxDuration *dur)
{
    if (dur == NULL)
        return;

    g_slice_free (DaxDuration, dur);
}

void
dax_duration_from_s (DaxDuration *dur,
                        gfloat          seconds)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = DAX_DURATION_S;
    dur->value = seconds;
    dur->ms = seconds * 1000;
    dur->ms_set = TRUE;
}

void
dax_duration_from_ms (DaxDuration *dur,
                         gfloat          ms)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = DAX_DURATION_MS;
    dur->value = ms;
    dur->ms = ms;
    dur->ms_set = TRUE;
}

gfloat
dax_duration_to_ms (DaxDuration *dur)
{
    g_return_val_if_fail (dur != NULL, 0.0);

    if (dur->ms_set)
        return dur->ms;

    switch (dur->unit_type) {
    case DAX_DURATION_S:
        dur->ms = dur->value * 1000;
        break;
    case DAX_DURATION_MS:
        dur->ms = dur->value;
        break;
    default:
        g_assert_not_reached ();
    }

    dur->ms_set = TRUE;

    return dur->ms;
}

gboolean
dax_duration_from_string (DaxDuration *dur,
                             const gchar    *string)
{
    DaxDurationType unit_type;
    gfloat value;
    gchar *str = (gchar *)string;

    g_return_val_if_fail (dur != NULL, FALSE);
    g_return_val_if_fail (str != NULL, FALSE);

    while (g_ascii_isspace (*str))
        str++;

    if (_dax_utils_parse_simple_float (&str, &value) == FALSE)
        return FALSE;

    while (g_ascii_isspace (*str))
        str++;

    /* default to seconds (SVG Tiny 1.2 clock-value p. 226) */
    if (*str == '\0')
        unit_type = DAX_DURATION_S;
    else if (*str == 's') {
        unit_type = DAX_DURATION_S;
        str++;
    } else if (strncmp (str, "ms", 2) == 0) {
        unit_type = DAX_DURATION_MS;
        str += 2;
    } else
        return FALSE;

    /* ensure the unit is only followed by white space */
    while (g_ascii_isspace (*str))
        str++;
    if (*str != '\0')
        return FALSE;

    dur->unit_type = unit_type;
    dur->value = value;
    dur->ms_set = FALSE;

    return TRUE;
}

static const gchar *
dax_duration_type_name (DaxDurationType unit_type)
{
    switch (unit_type) {
    case DAX_DURATION_MS:
        return "ms";
    case DAX_DURATION_S:
        return "s";
    default:
        g_assert_not_reached ();
        return "<invalid>";
    }
}

gchar *
dax_duration_to_string (const DaxDuration *dur)
{
    g_return_val_if_fail (dur != NULL, NULL);

    return g_strdup_printf ("%.02f%s",
                            dur->value,
                            dax_duration_type_name (dur->unit_type));
}

static void
_transform_duration_string (const GValue *src,
                            GValue       *dest)
{
    gchar *string = dax_duration_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_duration (const GValue *src,
                            GValue       *dest)
{
    DaxDuration duration = { DAX_DURATION_MS, 0.0f };

    dax_duration_from_string (&duration, g_value_get_string (src));

    dest->data[0].v_pointer = dax_duration_copy (&duration);
}

GType
dax_duration_get_type (void)
{
    static volatile gsize dax_duration_type__volatile = 0;

    if (g_once_init_enter (&dax_duration_type__volatile)) {
        GType dax_duration_type =
            g_boxed_type_register_static (I_("DaxDuration"),
                                          (GBoxedCopyFunc)dax_duration_copy,
                                          (GBoxedFreeFunc)dax_duration_free);

        g_value_register_transform_func (dax_duration_type, G_TYPE_STRING,
                                         _transform_duration_string);
        g_value_register_transform_func (G_TYPE_STRING, dax_duration_type,
                                         _transform_string_duration);
        g_once_init_leave (&dax_duration_type__volatile,
                           dax_duration_type);
    }

    return dax_duration_type__volatile;
}

/*
 * DaxRepeatCount
 */

DaxRepeatCount *
dax_repeat_count_copy (const DaxRepeatCount *count)
{
    if (count == NULL)
        return NULL;

    return g_slice_dup (DaxRepeatCount, count);
}

void
dax_repeat_count_free (DaxRepeatCount *count)
{
    if (count == NULL)
        return;

    g_slice_free (DaxRepeatCount, count);
}

void
dax_repeat_count_from_number (DaxRepeatCount *count,
                                 gfloat             nb)
{
    g_return_if_fail (count != NULL);

    count->value = nb;
}

gboolean
dax_repeat_count_is_indefinite (const DaxRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value == 0.0f;
}

gfloat
dax_repeat_count_get_value (const DaxRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value;
}

gboolean
dax_repeat_count_from_string (DaxRepeatCount *count,
                                 const gchar       *string)
{
    gfloat value;
    gchar *str = (gchar *)string;

    g_return_val_if_fail (count != NULL, FALSE);
    g_return_val_if_fail (count != NULL, FALSE);

    while (g_ascii_isspace (*str))
        str++;

    if (strncmp (str, "indefinite", 10) == 0) {
        value = 0.0f;
        str += 10;
    } else if (_dax_utils_parse_simple_float (&str, &value)) {
        /* value and str are updated by parse_simple_float () */
    } else
        return FALSE;

    /* ensure the unit is only followed by white space */
    while (g_ascii_isspace (*str))
        str++;
    if (*str != '\0')
        return FALSE;

    count->value = value;

    return TRUE;
}

gchar *
dax_repeat_count_to_string (const DaxRepeatCount *count)
{
    static char indefinite[] = "indefinite";

    g_return_val_if_fail (count != NULL, indefinite);

    if (count->value == 0.0f)
        return g_strdup (indefinite);

    return g_strdup_printf ("%.02f", count->value);
}

static void
_transform_repeat_count_string (const GValue *src,
                                GValue       *dest)
{
    gchar *string = dax_repeat_count_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_repeat_count (const GValue *src,
                                GValue       *dest)
{
    DaxRepeatCount repeat_count = { 0.0f };

    dax_repeat_count_from_string (&repeat_count, g_value_get_string (src));

    dest->data[0].v_pointer = dax_repeat_count_copy (&repeat_count);
}

GType
dax_repeat_count_get_type (void)
{
    static volatile gsize dax_repeat_count_type__volatile = 0;

    if (g_once_init_enter (&dax_repeat_count_type__volatile)) {
        GType dax_repeat_count_type =
            g_boxed_type_register_static (I_("DaxRepeatCount"),
                                          (GBoxedCopyFunc)dax_repeat_count_copy,
                                          (GBoxedFreeFunc)dax_repeat_count_free);

        g_value_register_transform_func (dax_repeat_count_type,
                                         G_TYPE_STRING,
                                         _transform_repeat_count_string);
        g_value_register_transform_func (G_TYPE_STRING,
                                         dax_repeat_count_type,
                                         _transform_string_repeat_count);
        g_once_init_leave (&dax_repeat_count_type__volatile,
                           dax_repeat_count_type);
    }

    return dax_repeat_count_type__volatile;
}
