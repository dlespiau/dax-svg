/*
 * Castet - Load and draw SVG
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

/*
 * Custom GParamSpec
 *
 * Castet needs custom GParamSpec to add a few features needed for SVG, namely:
 *   * differenciation between properties and attributes,
 *   * handling of inheritance.
 *   * specify if the parameter is animatable
 */

/*
 * CastetParamSpecEnum is derived From glib's gobject/gparamspecs.c
 *
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 * LGPL v2.1 (it's my option to choose 2.1)
 */
static void
param_enum_init (GParamSpec *pspec)
{
    GParamSpecEnum *espec = G_PARAM_SPEC_ENUM (pspec);

    espec->enum_class = NULL;
    espec->default_value = 0;
}

static void
param_enum_finalize (GParamSpec *pspec)
{
    GParamSpecEnum *espec = G_PARAM_SPEC_ENUM (pspec);
    GParamSpecClass *parent_class = g_type_class_peek (g_type_parent (G_TYPE_PARAM_ENUM));

    if (espec->enum_class) {
        g_type_class_unref (espec->enum_class);
        espec->enum_class = NULL;
    }

    parent_class->finalize (pspec);
}

static void
param_enum_set_default (GParamSpec *pspec,
                        GValue     *value)
{
    value->data[0].v_long = G_PARAM_SPEC_ENUM (pspec)->default_value;
}

static gboolean
param_enum_validate (GParamSpec *pspec,
                     GValue     *value)
{
    GParamSpecEnum *espec = G_PARAM_SPEC_ENUM (pspec);
    glong oval = value->data[0].v_long;

    if (!espec->enum_class ||
        !g_enum_get_value (espec->enum_class, value->data[0].v_long))
        value->data[0].v_long = espec->default_value;

    return value->data[0].v_long != oval;
}

static gint
param_long_values_cmp (GParamSpec   *pspec,
                       const GValue *value1,
                       const GValue *value2)
{
    if (value1->data[0].v_long < value2->data[0].v_long)
        return -1;
    else
        return value1->data[0].v_long > value2->data[0].v_long;
}

GType
castet_param_enum_get_type (void)
{
    static volatile gsize castet_param_enum_type__volatile = 0;

    if (g_once_init_enter (&castet_param_enum_type__volatile)) {
        GType type;
        static const GParamSpecTypeInfo pspec_info = {
            sizeof (GParamSpecEnum),  /* instance_size */
            16,                       /* n_preallocs */
            param_enum_init,          /* instance_init */
            G_TYPE_ENUM,              /* value_type */
            param_enum_finalize,      /* finalize */
            param_enum_set_default,   /* value_set_default */
            param_enum_validate,      /* value_validate */
            param_long_values_cmp,    /* values_cmp */
        };
        type = g_param_type_register_static (ISS("CastetParamEnum"),
                                             &pspec_info);
        g_once_init_leave (&castet_param_enum_type__volatile, type);
    }

    return castet_param_enum_type__volatile;
}

/**
 * castet_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @enum_type: a #GType derived from %G_TYPE_ENUM
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GParamSpecEnum instance specifying a %G_TYPE_ENUM
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */
GParamSpec*
castet_param_spec_enum (const gchar      *name,
                        const gchar      *nick,
                        const gchar      *blurb,
                        GType             enum_type,
                        gint              default_value,
                        GParamFlags       g_flags,
                        CastetParamFlags  castet_flags)
{
    CastetParamSpecEnum *castet_enum_spec;
    GEnumClass *enum_class;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref (enum_type);

    g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL,
                          NULL);

    castet_enum_spec = g_param_spec_internal (G_TYPE_PARAM_ENUM,
                                              name,
                                              nick,
                                              blurb,
                                              g_flags);

    castet_enum_spec->enum_class = enum_class;
    castet_enum_spec->default_value = default_value;
    castet_enum_spec->flags = castet_flags;
    G_PARAM_SPEC (castet_enum_spec)->value_type = enum_type;

    return G_PARAM_SPEC (castet_enum_spec);
}
