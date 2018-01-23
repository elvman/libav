/*
 * pixel format descriptor
 * Copyright (c) 2009 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_PIXDESC_H
#define AVUTIL_PIXDESC_H

#include <inttypes.h>

#include "attributes.h"
#include "pixfmt.h"

typedef struct AVComponentDescriptor {
    /**
     * Which of the 4 planes contains the component.
     */
    int plane;

    /**
     * Number of elements between 2 horizontally consecutive pixels.
     * Elements are bits for bitstream formats, bytes otherwise.
     */
    int step;

    /**
     * Number of elements before the component of the first pixel.
     * Elements are bits for bitstream formats, bytes otherwise.
     */
    int offset;

    /**
     * Number of least significant bits that must be shifted away
     * to get the value.
     */
    int shift;

    /**
     * Number of bits in the component.
     */
    int depth;

#if FF_API_PLUS1_MINUS1
    /** deprecated, use step instead */
    attribute_deprecated int step_minus1;

    /** deprecated, use depth instead */
    attribute_deprecated int depth_minus1;

    /** deprecated, use offset instead */
    attribute_deprecated int offset_plus1;
#endif
} AVComponentDescriptor;

/**
 * Descriptor that unambiguously describes how the bits of a pixel are
 * stored in the up to 4 data planes of an image. It also stores the
 * subsampling factors and number of components.
 *
 * @note This is separate of the colorspace (RGB, YCbCr, YPbPr, JPEG-style YUV
 *       and all the YUV variants) AVPixFmtDescriptor just stores how values
 *       are stored not what these values represent.
 */
typedef struct AVPixFmtDescriptor {
    const char *name;
    uint8_t nb_components;  ///< The number of components each pixel has, (1-4)

    /**
     * Amount to shift the luma width right to find the chroma width.
     * For YV12 this is 1 for example.
     * chroma_width = AV_CEIL_RSHIFT(luma_width, log2_chroma_w)
     * The note above is needed to ensure rounding up.
     * This value only refers to the chroma components.
     */
    uint8_t log2_chroma_w;

    /**
     * Amount to shift the luma height right to find the chroma height.
     * For YV12 this is 1 for example.
     * chroma_height= AV_CEIL_RSHIFT(luma_height, log2_chroma_h)
     * The note above is needed to ensure rounding up.
     * This value only refers to the chroma components.
     */
    uint8_t log2_chroma_h;

    /**
     * Combination of AV_PIX_FMT_FLAG_... flags.
     */
    uint64_t flags;

    /**
     * Parameters that describe how pixels are packed. If the format
     * has chroma components, they must be stored in comp[1] and
     * comp[2].
     * If the format is RGB-like, the first component is R, followed
     * by G and B.
     *
     * If the format is YUV-like, the first component is Y, followed
     * by U and V.
     *
     * If present, the Alpha channel is always the last component.
     */
    AVComponentDescriptor comp[4];

    /**
     * Alternative comma-separated names.
     */
    const char *alias;
} AVPixFmtDescriptor;

/**
 * Pixel format is big-endian.
 */
#define AV_PIX_FMT_FLAG_BE           (1 << 0)
/**
 * Pixel format has a palette in data[1], values are indexes in this palette.
 */
#define AV_PIX_FMT_FLAG_PAL          (1 << 1)
/**
 * All values of a component are bit-wise packed end to end.
 */
#define AV_PIX_FMT_FLAG_BITSTREAM    (1 << 2)
/**
 * Pixel format is an HW accelerated format.
 */
#define AV_PIX_FMT_FLAG_HWACCEL      (1 << 3)
/**
 * At least one pixel component is not in the first data plane.
 */
#define AV_PIX_FMT_FLAG_PLANAR       (1 << 4)
/**
 * The pixel format contains RGB-like data (as opposed to YUV/grayscale).
 */
#define AV_PIX_FMT_FLAG_RGB          (1 << 5)
/**
 * The pixel format is "pseudo-paletted". This means that Libav treats it as
 * paletted internally, but the palette is generated by the decoder and is not
 * stored in the file.
 */
#define AV_PIX_FMT_FLAG_PSEUDOPAL    (1 << 6)
/**
 * The pixel format has an alpha channel.
 */
#define AV_PIX_FMT_FLAG_ALPHA        (1 << 7)

/**
 * Read a line from an image, and write the values of the
 * pixel format component c to dst.
 *
 * @param data the array containing the pointers to the planes of the image
 * @param linesize the array containing the linesizes of the image
 * @param desc the pixel format descriptor for the image
 * @param x the horizontal coordinate of the first pixel to read
 * @param y the vertical coordinate of the first pixel to read
 * @param w the width of the line to read, that is the number of
 * values to write to dst
 * @param read_pal_component if not zero and the format is a paletted
 * format writes the values corresponding to the palette
 * component c in data[1] to dst, rather than the palette indexes in
 * data[0]. The behavior is undefined if the format is not paletted.
 */
void av_read_image_line(uint16_t *dst, const uint8_t *data[4],
                        const int linesize[4], const AVPixFmtDescriptor *desc,
                        int x, int y, int c, int w, int read_pal_component);

/**
 * Write the values from src to the pixel format component c of an
 * image line.
 *
 * @param src array containing the values to write
 * @param data the array containing the pointers to the planes of the
 * image to write into. It is supposed to be zeroed.
 * @param linesize the array containing the linesizes of the image
 * @param desc the pixel format descriptor for the image
 * @param x the horizontal coordinate of the first pixel to write
 * @param y the vertical coordinate of the first pixel to write
 * @param w the width of the line to write, that is the number of
 * values to write to the image line
 */
void av_write_image_line(const uint16_t *src, uint8_t *data[4],
                         const int linesize[4], const AVPixFmtDescriptor *desc,
                         int x, int y, int c, int w);

/**
 * Return the pixel format corresponding to name.
 *
 * If there is no pixel format with name name, then looks for a
 * pixel format with the name corresponding to the native endian
 * format of name.
 * For example in a little-endian system, first looks for "gray16",
 * then for "gray16le".
 *
 * Finally if no pixel format has been found, returns PIX_FMT_NONE.
 */
enum AVPixelFormat av_get_pix_fmt(const char *name);

/**
 * Return the short name for a pixel format, NULL in case pix_fmt is
 * unknown.
 *
 * @see av_get_pix_fmt(), av_get_pix_fmt_string()
 */
const char *av_get_pix_fmt_name(enum AVPixelFormat pix_fmt);

/**
 * Print in buf the string corresponding to the pixel format with
 * number pix_fmt, or an header if pix_fmt is negative.
 *
 * @param buf the buffer where to write the string
 * @param buf_size the size of buf
 * @param pix_fmt the number of the pixel format to print the
 * corresponding info string, or a negative value to print the
 * corresponding header.
 */
char *av_get_pix_fmt_string(char *buf, int buf_size,
                            enum AVPixelFormat pix_fmt);

/**
 * Return the number of bits per pixel used by the pixel format
 * described by pixdesc. Note that this is not the same as the number
 * of bits per sample.
 *
 * The returned number of bits refers to the number of bits actually
 * used for storing the pixel information, that is padding bits are
 * not counted.
 */
int av_get_bits_per_pixel(const AVPixFmtDescriptor *pixdesc);

/**
 * @return a pixel format descriptor for provided pixel format or NULL if
 * this pixel format is unknown.
 */
const AVPixFmtDescriptor *av_pix_fmt_desc_get(enum AVPixelFormat pix_fmt);

/**
 * Iterate over all pixel format descriptors known to libavutil.
 *
 * @param prev previous descriptor. NULL to get the first descriptor.
 *
 * @return next descriptor or NULL after the last descriptor
 */
const AVPixFmtDescriptor *av_pix_fmt_desc_next(const AVPixFmtDescriptor *prev);

/**
 * @return an AVPixelFormat id described by desc, or AV_PIX_FMT_NONE if desc
 * is not a valid pointer to a pixel format descriptor.
 */
enum AVPixelFormat av_pix_fmt_desc_get_id(const AVPixFmtDescriptor *desc);

/**
 * Utility function to access log2_chroma_w log2_chroma_h from
 * the pixel format AVPixFmtDescriptor.
 *
 * @param[in]  pix_fmt the pixel format
 * @param[out] h_shift store log2_chroma_w (horizontal/width shift)
 * @param[out] v_shift store log2_chroma_h (vertical/height shift)
 *
 * @return 0 on success, AVERROR(ENOSYS) on invalid or unknown pixel format
 */
int av_pix_fmt_get_chroma_sub_sample(enum AVPixelFormat pix_fmt,
                                     int *h_shift, int *v_shift);

/**
 * @return number of planes in pix_fmt, a negative AVERROR if pix_fmt is not a
 * valid pixel format.
 */
int av_pix_fmt_count_planes(enum AVPixelFormat pix_fmt);

/**
 * Utility function to swap the endianness of a pixel format.
 *
 * @param[in]  pix_fmt the pixel format
 *
 * @return pixel format with swapped endianness if it exists,
 * otherwise AV_PIX_FMT_NONE
 */
enum AVPixelFormat av_pix_fmt_swap_endianness(enum AVPixelFormat pix_fmt);

/**
 * @return the name for provided color range or NULL if unknown.
 */
const char *av_color_range_name(enum AVColorRange range);

/**
 * @return the AVColorRange value for name or an AVError if not found.
 */
int av_color_range_from_name(const char *name);

/**
 * @return the name for provided color primaries or NULL if unknown.
 */
const char *av_color_primaries_name(enum AVColorPrimaries primaries);

/**
 * @return the AVColorPrimaries value for name or an AVError if not found.
 */
int av_color_primaries_from_name(const char *name);

/**
 * @return the name for provided color transfer or NULL if unknown.
 */
const char *av_color_transfer_name(enum AVColorTransferCharacteristic transfer);

/**
 * @return the AVColorTransferCharacteristic value for name or an AVError if not found.
 */
int av_color_transfer_from_name(const char *name);

/**
 * @return the name for provided color space or NULL if unknown.
 */
const char *av_color_space_name(enum AVColorSpace space);

/**
 * @return the AVColorSpace value for name or an AVError if not found.
 */
int av_color_space_from_name(const char *name);

/**
 * @return the name for provided chroma location or NULL if unknown.
 */
const char *av_chroma_location_name(enum AVChromaLocation location);

/**
 * @return the AVChromaLocation value for name or an AVError if not found.
 */
int av_chroma_location_from_name(const char *name);

#endif /* AVUTIL_PIXDESC_H */
