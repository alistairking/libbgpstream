/*
 * This file is part of bgpstream
 *
 * CAIDA, UC San Diego
 * bgpstream-info@caida.org
 *
 * Copyright (C) 2012 The Regents of the University of California.
 * Authors: Alistair King, Chiara Orsini
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __BGPSTREAM_FORMAT_INTERFACE_H
#define __BGPSTREAM_FORMAT_INTERFACE_H

#include "bgpstream.h"
#include "bgpstream_format.h" /*< for bgpstream_format_t */
#include "bgpstream_transport.h"
#include "config.h"

/** @file
 *
 * @brief Header file that exposes the protected interface of the data format
 * plugin API
 *
 * @author Alistair King
 *
 */

/** Convenience macro that defines all the function prototypes for the data
 * format API */
#define BS_FORMAT_GENERATE_PROTOS(name)                                        \
  int bs_format_##name##_create(bgpstream_format_t *format,                    \
                                bgpstream_resource_t *res);                    \
  bgpstream_format_status_t bs_format_##name##_populate_record(                \
    bgpstream_format_t *format, bgpstream_record_t *record);                   \
  int bs_format_##name##_get_next_elem(bgpstream_format_t *format,             \
                                       bgpstream_record_t *record,             \
                                       bgpstream_elem_t **elem);               \
  void bs_format_##name##_destroy_data(bgpstream_format_t *format,             \
                                       void *data);                            \
  void bs_format_##name##_destroy(bgpstream_format_t *format);

#define BS_FORMAT_SET_METHODS(classname, format)                               \
  do {                                                                         \
    (format)->populate_record = bs_format_##classname##_populate_record;       \
    (format)->get_next_elem = bs_format_##classname##_get_next_elem;           \
    (format)->destroy_data = bs_format_##classname##_destroy_data;             \
    (format)->destroy = bs_format_##classname##_destroy;                       \
  } while (0)

/** Structure which represents a data format */
struct bgpstream_format {

  /**
   * @name Format method pointers
   *
   * @{ */

  /**
   * Get the next record from this format instance
   *
   * @param format      pointer to the format object to read from
   * @param[out] record   set to point to a populated record instance
   * @return format status code
   *
   * This function should use the filter manager to only return records that
   * match the given filters.
   */
  bgpstream_format_status_t (*populate_record)(struct bgpstream_format *format,
                                               bgpstream_record_t *record);

  /** Get the next elem from the given record
   *
   * @param format        pointer to the format object to use
   * @param record        pointer to the record to use
   * @param[out] elem     set to point to a borrowed elem structure or NULL if
   *                      there are no more elems
   * @return 1 if a valid elem was returned, 0 if there are no more elems, -1 if
   * an error occurred.
   */
  int (*get_next_elem)(bgpstream_format_t *format,
                       bgpstream_record_t *record,
                       bgpstream_elem_t **elem);

  /** Destroy the given format-specific record data
   *
   * @param format      pointer to the format object to use
   * @param data        pointer to the data to destroy
   */
  void (*destroy_data)(struct bgpstream_format *format,
                       void *data);

  /** Destroy the given format module
   *
   * @param format        pointer to the format instance to destroy
   */
  void (*destroy)(struct bgpstream_format *format);

  /** }@ */

  /**
   * @name Data format state fields
   *
   * @{ */

  /** Pointer to the transport instance to read data from */
  bgpstream_transport_t *transport;

  /** Pointer to the filter manager instance to use to filter records */
  bgpstream_filter_mgr_t *filter_mgr;

  /** An opaque pointer to format-specific state if needed */
  void *state;

  /** }@ */
};

#endif /* __BGPSTREAM_FORMAT_INTERFACE_H */
