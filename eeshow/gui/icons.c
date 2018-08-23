/*
 * gui/icons.c - Icons
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdint.h>
#include <string.h>

#include <cairo/cairo.h>

#include "gui/icons.h"


cairo_surface_t *icon_delta;
cairo_surface_t *icon_diff;


struct read_ctx {
	uint8_t *data;
	unsigned left;
};


static uint8_t data_delta[] = {
#include "icons/delta.hex"
};

static uint8_t data_diff[] = {
#include "icons/diff.hex"
};


static cairo_status_t read_data(void *user, unsigned char *data,
    unsigned length)
{
	struct read_ctx *read_ctx = user;

	if (!read_ctx->left)
		return CAIRO_STATUS_READ_ERROR;
	if (length > read_ctx->left)
		length = read_ctx->left;
	memcpy(data, read_ctx->data, length);
	read_ctx->data += length;
	read_ctx->left -= length;
	return CAIRO_STATUS_SUCCESS;
}


static cairo_surface_t *get_icon(uint8_t *data, unsigned size)
{
	struct read_ctx read_ctx = {
		.data	= data,
		.left	= size,
	};


	return cairo_image_surface_create_from_png_stream(read_data, &read_ctx);
}


void icons_init(void)
{
	icon_delta = get_icon(data_delta, sizeof(data_delta));
	icon_diff = get_icon(data_diff, sizeof(data_diff));

}
