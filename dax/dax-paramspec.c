/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include <clutter/clutter.h>

#include "dax-internals.h"
#include "dax-utils.h"
#include "dax-paramspec.h"

/*
 * Custom GParamSpecs
 *
 * Dax needs custom GParamSpec to add a few features needed for SVG, namely:
 *   * differenciation between properties and attributes,
 *   * handling of inheritance,
 *   * specify if the parameter is animatable.
 */

/*
 * DaxParamSpecEnum
 */

static void
dax_param_enum_init (GParamSpec *pspec)
{
#if 0
    DaxParamSpecEnum *espec = DAX_PARAM_SPEC_ENUM (pspec);
#endif
}

static void
dax_param_enum_class_init (GParamSpecClass *klass)
{
    klass->value_type = G_TYPE_ENUM;

    ((DaxParamSpecClass *)klass)->from_string = NULL;
    ((DaxParamSpecClass *)klass)->to_string = NULL;
}

GType
dax_param_enum_get_type (void)
{
    static volatile gsize dax_param_enum_type__volatile = 0;

    if (g_once_init_enter (&dax_param_enum_type__volatile)) {
        GType type;

        static const GTypeInfo info = {
            sizeof (DaxParamSpecClass),
            NULL,
            NULL,
            (GClassInitFunc) dax_param_enum_class_init,
            NULL,
            NULL,
            sizeof (DaxParamSpecEnum),
            0,
            (GInstanceInitFunc) dax_param_enum_init
        };

        type = g_type_register_static (G_TYPE_PARAM_ENUM,
                                       "DaxParamSpecEnum",
                                       &info,
                                       0);

        g_once_init_leave (&dax_param_enum_type__volatile, type);
    }

    return dax_param_enum_type__volatile;
}

/**
 * dax_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @enum_type: a #GType derived from %G_TYPE_ENUM
 * @default_value: default value for the property specified
 * @g_flags: flags for the underlying GParamSpec property
 * @dax_flags: flags specific to Dax
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #DaxParamSpecEnum instance specifying a %G_TYPE_ENUM
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
dax_param_spec_enum (const gchar   *name,
                     const gchar   *nick,
                     const gchar   *blurb,
                     GType          enum_type,
                     gint           default_value,
                     GParamFlags    g_flags,
                     DaxParamFlags  dax_flags,
                     const char    *namespace_uri)
{
    DaxParamSpecEnum *dax_enum_spec;
    GParamSpecEnum *g_enum_spec;
    GEnumClass *enum_class;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref (enum_type);

    g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL,
                          NULL);

    dax_enum_spec = g_param_spec_internal (DAX_TYPE_PARAM_ENUM,
                                           name,
                                           nick,
                                           blurb,
                                           g_flags);

    G_PARAM_SPEC (dax_enum_spec)->value_type = enum_type;

    g_enum_spec = G_PARAM_SPEC_ENUM (dax_enum_spec);
    g_enum_spec->enum_class = enum_class;
    g_enum_spec->default_value = default_value;

    dax_enum_spec->flags = dax_flags;
    dax_enum_spec->namespace_uri = namespace_uri;

    return G_PARAM_SPEC (dax_enum_spec);
}

/*
 * DaxParamSpecString
 */

static void
dax_param_string_init (GParamSpec *pspec)
{
#if 0
    DaxParamSpecString *espec = DAX_PARAM_SPEC_STRING (pspec);
#endif
}

static void
dax_param_string_class_init (GParamSpecClass *klass)
{
    klass->value_type = G_TYPE_STRING;

    ((DaxParamSpecClass *)klass)->from_string = NULL;
    ((DaxParamSpecClass *)klass)->to_string = NULL;
}

GType
dax_param_string_get_type (void)
{
    static volatile gsize dax_param_string_type__volatile = 0;

    if (g_once_init_enter (&dax_param_string_type__volatile)) {
        GType type;

        static const GTypeInfo info = {
            sizeof (DaxParamSpecClass),
            NULL,
            NULL,
            (GClassInitFunc) dax_param_string_class_init,
            NULL,
            NULL,
            sizeof (DaxParamSpecString),
            0,
            (GInstanceInitFunc) dax_param_string_init
        };

        type = g_type_register_static (G_TYPE_PARAM_STRING,
                                       "DaxParamSpecString",
                                       &info,
                                       0);

        g_once_init_leave (&dax_param_string_type__volatile, type);
    }

    return dax_param_string_type__volatile;
}

/**
 * dax_param_spec_string:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @string_type: a #GType derived from %G_TYPE_STRING
 * @default_value: default value for the property specified
 * @g_flags: flags for the underlying GParamSpec property
 * @dax_flags: flags specific to Dax
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #DaxParamSpecString instance specifying a %G_TYPE_STRING
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
dax_param_spec_string (const gchar   *name,
                       const gchar   *nick,
                       const gchar   *blurb,
                       const gchar   *default_value,
                       GParamFlags    g_flags,
                       DaxParamFlags  dax_flags,
                       const char    *namespace_uri)
{
    DaxParamSpecString *dax_string_spec;

    dax_string_spec = g_param_spec_internal (DAX_TYPE_PARAM_STRING,
                                             name,
                                             nick,
                                             blurb,
                                             g_flags);

    G_PARAM_SPEC_STRING (dax_string_spec)->default_value =
        (gchar *) default_value;

    dax_string_spec->flags = dax_flags;
    dax_string_spec->namespace_uri = namespace_uri;

    return G_PARAM_SPEC (dax_string_spec);
}

/*
 * DaxParamSpecBoxed
 */

static void
dax_param_boxed_class_init (GParamSpecClass *klass)
{
    klass->value_type = G_TYPE_BOXED;

    ((DaxParamSpecClass *)klass)->from_string = NULL;
    ((DaxParamSpecClass *)klass)->to_string = NULL;
}

GType
dax_param_boxed_get_type (void)
{
    static volatile gsize dax_param_enum_type__volatile = 0;

    if (g_once_init_enter (&dax_param_enum_type__volatile)) {
        GType type;

        static const GTypeInfo info = {
            sizeof (DaxParamSpecClass),
            NULL,
            NULL,
            (GClassInitFunc) dax_param_boxed_class_init,
            NULL,
            NULL,
            sizeof (DaxParamSpecBoxed),
            0,
            NULL,
        };

        type = g_type_register_static (G_TYPE_PARAM_BOXED,
                                       "DaxParamSpecBoxed",
                                       &info,
                                       0);

        g_once_init_leave (&dax_param_enum_type__volatile, type);
    }

    return dax_param_enum_type__volatile;
}

/**
 * dax_param_spec_boxed:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @boxed_type: a #GType derived from %G_TYPE_ENUM
 * @g_flags: flags for the underlying GParamSpec property
 * @dax_flags: flags specific to Dax
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #DaxParamSpecBoxed instance specifying a %G_TYPE_BOXED
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
dax_param_spec_boxed (const gchar   *name,
                      const gchar   *nick,
                      const gchar   *blurb,
                      GType          boxed_type,
                      GParamFlags    g_flags,
                      DaxParamFlags  dax_flags,
                      const char    *namespace_uri)
{
    DaxParamSpecBoxed *dax_boxed_spec;

    g_return_val_if_fail (G_TYPE_IS_BOXED (boxed_type), NULL);
    g_return_val_if_fail (G_TYPE_IS_VALUE_TYPE (boxed_type), NULL);

    dax_boxed_spec = g_param_spec_internal (DAX_TYPE_PARAM_BOXED,
                                           name,
                                           nick,
                                           blurb,
                                           g_flags);

    G_PARAM_SPEC (dax_boxed_spec)->value_type = boxed_type;

    dax_boxed_spec->flags = dax_flags;
    dax_boxed_spec->namespace_uri = namespace_uri;

    return G_PARAM_SPEC (dax_boxed_spec);
}

/*
 * DaxParamSpecArray
 */

static gint
dax_param_units_array_from_string (GParamSpec *pspec,
                                   const char *str,
                                   GArray     *array)
{
    gchar *units_start, *start, *end;
    gint parsed_elements = 0;
    ClutterUnits units;
    gboolean success;

    /* The idea is to insert '\0' between each unit */
    start = units_start = g_strdup (str);
    end = strchr (start, ',');

    if (end) {
        /* ','-separated list */
        do {
            *end = '\0';
            success = clutter_units_from_string (&units, units_start);
            if (!success) {
                parsed_elements = -1;
                break;
            }
            parsed_elements++;
            g_array_append_val (array, units);
            units_start = end + 1;
            end = strchr (units_start, ',');
        } while (end);
    } else {
        /* white space-separated list */
        guint len;

        len = strlen (start);
        do {
            end = units_start;
            _dax_utils_find_next_space (&end);
            *end = '\0';
            success = clutter_units_from_string (&units, units_start);
            if (!success) {
                parsed_elements = -1;
                break;
            }
            parsed_elements++;
            g_array_append_val (array, units);
            units_start = end + 1;
        } while (units_start < (start + len));
    }

    g_free (start);
    return parsed_elements;
}

static gint
dax_param_float_array_from_string (GParamSpec *pspec,
                                   const char *str,
                                   GArray     *array)
{
    gchar *cur = (gchar *)str;
    gint parsed_elements = 0;
    gboolean success;
    gfloat value;

    while (*cur) {
        _dax_utils_skip_space (&cur);
        success = _dax_utils_parse_float (&cur, &value);
        if (!success) {
            parsed_elements = -1;
            break;
        }
        parsed_elements++;
        g_array_append_val (array, value);
        _dax_utils_skip_space_and_char (&cur, ',');
    }

    return parsed_elements;
}

static gboolean
dax_param_array_from_string (GParamSpec *pspec,
                             const char *string,
                             GValue     *new_value)
{
    DaxParamSpecArray *array_pspec = DAX_PARAM_SPEC_ARRAY (pspec);
    gint expected_elements, parsed_elements = 0;
    guint element_size;
    gboolean success;
    GArray *array;

    if (array_pspec->element_type == G_TYPE_FLOAT)
        element_size = sizeof (float);
    else if (array_pspec->element_type == CLUTTER_TYPE_UNITS)
        element_size = sizeof (ClutterUnits);
    else
        g_assert_not_reached ();

    if (array_pspec->size != DAX_PARAM_SPEC_ARRAY_NOT_SIZED) {
        array = g_array_sized_new (FALSE, FALSE,
                                   element_size,
                                   array_pspec->size);
        expected_elements = array_pspec->size;
    } else {

        /* a <list-of-T> can be separated by ',' (+ surrounding white space)
         * or by white space only */
        expected_elements = _dax_utils_count_commas (string);
        if (expected_elements == 0)
            expected_elements = _dax_utils_count_words (string);
        else
            expected_elements++; /* n commas -> n + 1 elements */

        array = g_array_sized_new (FALSE, FALSE,
                                   element_size,
                                   expected_elements);
    }

    if (array_pspec->element_type == G_TYPE_FLOAT)
        parsed_elements = dax_param_float_array_from_string (pspec,
                                                             string,
                                                             array);
    else if (array_pspec->element_type == CLUTTER_TYPE_UNITS)
        parsed_elements = dax_param_units_array_from_string (pspec,
                                                             string,
                                                             array);
    else
        g_assert_not_reached ();

    if (expected_elements != parsed_elements)
        g_warning ("%s: parsed \"%s\": %d elements found but was expecting %d",
                   pspec->name, string, parsed_elements, expected_elements);

    success = parsed_elements == expected_elements;

    if (success)
        g_value_take_boxed (new_value, array);
    else
        g_array_free (array, TRUE);

    return success;
}

static void
dax_param_array_class_init (GParamSpecClass *klass)
{
    klass->value_type = G_TYPE_ARRAY;

    ((DaxParamSpecClass *)klass)->from_string = dax_param_array_from_string;
    ((DaxParamSpecClass *)klass)->to_string = NULL;
}

GType
dax_param_array_get_type (void)
{
    static volatile gsize dax_param_enum_type__volatile = 0;

    if (g_once_init_enter (&dax_param_enum_type__volatile)) {
        GType type;

        static const GTypeInfo info = {
            sizeof (DaxParamSpecClass),
            NULL,
            NULL,
            (GClassInitFunc) dax_param_array_class_init,
            NULL,
            NULL,
            sizeof (DaxParamSpecArray),
            0,
            NULL,
        };

        type = g_type_register_static (G_TYPE_PARAM_BOXED,
                                       "DaxParamSpecArray",
                                       &info,
                                       0);

        g_once_init_leave (&dax_param_enum_type__volatile, type);
    }

    return dax_param_enum_type__volatile;
}

/**
 * dax_param_spec_array:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @element_type: a #GType derived from %G_TYPE_ENUM of the array elements
 * @size: number of elements of the array or %DAX_PARAM_SPEC_ARRAY_NOT_SIZED
 * @g_flags: flags for the underlying GParamSpec property
 * @dax_flags: flags specific to Dax
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #DaxParamSpecArray instance. The type of the elements the
 * array holds is given by @element_type.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
dax_param_spec_array (const gchar   *name,
                      const gchar   *nick,
                      const gchar   *blurb,
                      GType          element_type,
                      gint           size,
                      GParamFlags    g_flags,
                      DaxParamFlags  dax_flags,
                      const char    *namespace_uri)
{
    DaxParamSpecArray *dax_array_spec;

    dax_array_spec = g_param_spec_internal (DAX_TYPE_PARAM_ARRAY,
                                           name,
                                           nick,
                                           blurb,
                                           g_flags);

    G_PARAM_SPEC (dax_array_spec)->value_type = G_TYPE_ARRAY;

    dax_array_spec->element_type = element_type;
    dax_array_spec->size = size;
    dax_array_spec->flags = dax_flags;
    dax_array_spec->namespace_uri = namespace_uri;

    return G_PARAM_SPEC (dax_array_spec);
}
