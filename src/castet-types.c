/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#include "castet-internals.h"
#include "castet-utils.h"
#include "castet-types.h"

/*
 * CastetDuration
 */

CastetDurationType
castet_duration_get_unit_type (const CastetDuration *dur)
{
    g_return_val_if_fail (dur, CASTET_DURATION_MS);

    return dur->unit_type;
}

gfloat
castet_duration_get_unit_value (const CastetDuration *dur)
{
    g_return_val_if_fail (dur, 0.0f);

    return dur->value;
}

CastetDuration *
castet_duration_copy (const CastetDuration *dur)
{
    if (dur == NULL)
        return NULL;

    return g_slice_dup (CastetDuration, dur);
}

void
castet_duration_free (CastetDuration *dur)
{
    if (dur == NULL)
        return;

    g_slice_free (CastetDuration, dur);
}

void
castet_duration_from_s (CastetDuration *dur,
                        gfloat          seconds)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = CASTET_DURATION_S;
    dur->value = seconds;
    dur->ms = seconds * 1000;
    dur->ms_set = TRUE;
}

void
castet_duration_from_ms (CastetDuration *dur,
                         gfloat          ms)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = CASTET_DURATION_MS;
    dur->value = ms;
    dur->ms = ms;
    dur->ms_set = TRUE;
}

gfloat
castet_duration_to_ms (CastetDuration *dur)
{
    g_return_val_if_fail (dur != NULL, 0.0);

    if (dur->ms_set)
        return dur->ms;

    switch (dur->unit_type) {
    case CASTET_DURATION_S:
        dur->ms = dur->value * 1000;
        break;
    case CASTET_DURATION_MS:
        dur->ms = dur->value;
        break;
    }

    dur->ms_set = TRUE;

    return dur->ms;
}

gboolean
castet_duration_from_string (CastetDuration *dur,
                             const gchar    *string)
{
    CastetDurationType unit_type;
    gfloat value;
    gchar *str = (gchar *)string;

    g_return_val_if_fail (dur != NULL, FALSE);
    g_return_val_if_fail (str != NULL, FALSE);

    while (g_ascii_isspace (*str))
        str++;

    if (_castet_utils_parse_simple_float (&str, &value) == FALSE)
        return FALSE;

    while (g_ascii_isspace (*str))
        str++;

    /* default to seconds (SVG Tiny 1.2 clock-value p. 226) */
    if (*str == '\0')
        unit_type = CASTET_DURATION_S;
    else if (*str == 's') {
        unit_type = CASTET_DURATION_S;
        str++;
    } else if (strncmp (str, "ms", 2) == 0) {
        unit_type = CASTET_DURATION_MS;
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
castet_duration_type_name (CastetDurationType unit_type)
{
    switch (unit_type) {
    case CASTET_DURATION_MS:
        return "ms";
    case CASTET_DURATION_S:
        return "s";
    default:
        g_assert_not_reached ();
        return "<invalid>";
    }
}

gchar *
castet_duration_to_string (const CastetDuration *dur)
{
    g_return_val_if_fail (dur != NULL, NULL);

    return g_strdup_printf ("%.02f%s",
                            dur->value,
                            castet_duration_type_name (dur->unit_type));
}

static void
_transform_duration_string (const GValue *src,
                            GValue       *dest)
{
    gchar *string = castet_duration_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_duration (const GValue *src,
                            GValue       *dest)
{
    CastetDuration duration = { CASTET_DURATION_MS, 0.0f };

    castet_duration_from_string (&duration, g_value_get_string (src));

    dest->data[0].v_pointer = castet_duration_copy (&duration);
}

GType
castet_duration_get_type (void)
{
    static volatile gsize castet_duration_type__volatile = 0;

    if (g_once_init_enter (&castet_duration_type__volatile)) {
        GType castet_duration_type =
            g_boxed_type_register_static (ISS("CastetDuration"),
                                          (GBoxedCopyFunc)castet_duration_copy,
                                          (GBoxedFreeFunc)castet_duration_free);

        g_value_register_transform_func (castet_duration_type, G_TYPE_STRING,
                                         _transform_duration_string);
        g_value_register_transform_func (G_TYPE_STRING, castet_duration_type,
                                         _transform_string_duration);
        g_once_init_leave (&castet_duration_type__volatile,
                           castet_duration_type);
    }

    return castet_duration_type__volatile;
}

/*
 * CastetRepeatCount
 */

CastetRepeatCount *
castet_repeat_count_copy (const CastetRepeatCount *count)
{
    if (count == NULL)
        return NULL;

    return g_slice_dup (CastetRepeatCount, count);
}

void
castet_repeat_count_free (CastetRepeatCount *count)
{
    if (count == NULL)
        return;

    g_slice_free (CastetRepeatCount, count);
}

void
castet_repeat_count_from_number (CastetRepeatCount *count,
                                 gfloat             nb)
{
    g_return_if_fail (count != NULL);

    count->value = nb;
}

gboolean
castet_repeat_count_is_indefinite (const CastetRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value == 0.0f;
}

gfloat
castet_repeat_count_get_value (const CastetRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value;
}

gboolean
castet_repeat_count_from_string (CastetRepeatCount *count,
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
    } else if (_castet_utils_parse_simple_float (&str, &value)) {
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
castet_repeat_count_to_string (const CastetRepeatCount *count)
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
    gchar *string = castet_repeat_count_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_repeat_count (const GValue *src,
                                GValue       *dest)
{
    CastetRepeatCount repeat_count = { 0.0f };

    castet_repeat_count_from_string (&repeat_count, g_value_get_string (src));

    dest->data[0].v_pointer = castet_repeat_count_copy (&repeat_count);
}

GType
castet_repeat_count_get_type (void)
{
    static volatile gsize castet_repeat_count_type__volatile = 0;

    if (g_once_init_enter (&castet_repeat_count_type__volatile)) {
        GType castet_repeat_count_type =
            g_boxed_type_register_static (ISS("CastetRepeatCount"),
                                          (GBoxedCopyFunc)castet_repeat_count_copy,
                                          (GBoxedFreeFunc)castet_repeat_count_free);

        g_value_register_transform_func (castet_repeat_count_type,
                                         G_TYPE_STRING,
                                         _transform_repeat_count_string);
        g_value_register_transform_func (G_TYPE_STRING,
                                         castet_repeat_count_type,
                                         _transform_string_repeat_count);
        g_once_init_leave (&castet_repeat_count_type__volatile,
                           castet_repeat_count_type);
    }

    return castet_repeat_count_type__volatile;
}
