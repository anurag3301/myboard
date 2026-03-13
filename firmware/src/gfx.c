#include "gfx.h"

#include <string.h>

static size_t gfx_bitmap_required_size(const GFX_Bitmap *bitmap)
{
    size_t row_bytes = ((size_t)bitmap->width + 7U) / 8U;
    return row_bytes * (size_t)bitmap->height;
}

size_t GFX_CalcBufferSize(uint16_t width, uint16_t height)
{
    uint16_t pages = (uint16_t)((height + 7U) / 8U);
    return (size_t)width * (size_t)pages;
}

GFX_Framebuffer GFX_Init(uint16_t width,
                         uint16_t height,
                         uint8_t *buffer,
                         size_t buffer_size,
                         GFX_FlushCallback flush_cb,
                         void *flush_context)
{
    GFX_Framebuffer fb;
    size_t required_size = GFX_CalcBufferSize(width, height);

    fb.width = width;
    fb.height = height;
    fb.pages = (uint16_t)((height + 7U) / 8U);
    fb.stride = width;
    fb.buffer = buffer;
    fb.buffer_size = buffer_size;
    fb.flush_cb = flush_cb;
    fb.flush_context = flush_context;

    if ((buffer == NULL) || (buffer_size < required_size) || (width == 0U) || (height == 0U))
    {
        fb.buffer = NULL;
        fb.buffer_size = 0U;
    }

    return fb;
}

uint8_t GFX_IsReady(const GFX_Framebuffer *fb)
{
    if (fb == NULL)
    {
        return 0U;
    }

    if ((fb->buffer == NULL) || (fb->width == 0U) || (fb->height == 0U))
    {
        return 0U;
    }

    return (fb->buffer_size >= GFX_CalcBufferSize(fb->width, fb->height)) ? 1U : 0U;
}

void GFX_Clear(GFX_Framebuffer *fb, uint8_t color)
{
    if (GFX_IsReady(fb) == 0U)
    {
        return;
    }

    memset(fb->buffer, (color != 0U) ? 0xFF : 0x00, GFX_CalcBufferSize(fb->width, fb->height));
}

void GFX_DrawPixel(GFX_Framebuffer *fb, int16_t x, int16_t y, uint8_t color)
{
    if (GFX_IsReady(fb) == 0U)
    {
        return;
    }

    if ((x < 0) || (y < 0) || (x >= (int16_t)fb->width) || (y >= (int16_t)fb->height))
    {
        return;
    }

    uint16_t page = (uint16_t)y / 8U;
    uint16_t index = (uint16_t)x + (uint16_t)(page * fb->stride);
    uint8_t mask = (uint8_t)(1U << ((uint16_t)y % 8U));

    if (color != 0U)
    {
        fb->buffer[index] |= mask;
    }
    else
    {
        fb->buffer[index] &= (uint8_t)(~mask);
    }
}

void GFX_DrawRectBorder(GFX_Framebuffer *fb,
                        int16_t x,
                        int16_t y,
                        uint16_t width,
                        uint16_t height,
                        uint8_t thickness,
                        uint8_t color)
{
    if ((GFX_IsReady(fb) == 0U) || (width == 0U) || (height == 0U) || (thickness == 0U))
    {
        return;
    }

    for (uint8_t t = 0U; t < thickness; t++)
    {
        int16_t left = (int16_t)(x + t);
        int16_t top = (int16_t)(y + t);
        int16_t right = (int16_t)(x + (int16_t)width - 1 - (int16_t)t);
        int16_t bottom = (int16_t)(y + (int16_t)height - 1 - (int16_t)t);

        if ((right < left) || (bottom < top))
        {
            break;
        }

        for (int16_t px = left; px <= right; px++)
        {
            GFX_DrawPixel(fb, px, top, color);
            GFX_DrawPixel(fb, px, bottom, color);
        }

        for (int16_t py = top; py <= bottom; py++)
        {
            GFX_DrawPixel(fb, left, py, color);
            GFX_DrawPixel(fb, right, py, color);
        }
    }
}

void GFX_DrawBitmap(GFX_Framebuffer *fb, const GFX_Bitmap *bitmap, int16_t x, int16_t y)
{
    if ((GFX_IsReady(fb) == 0U) || (bitmap == NULL) || (bitmap->data == NULL))
    {
        return;
    }

    if ((bitmap->width == 0U) || (bitmap->height == 0U))
    {
        return;
    }

    size_t required_size = gfx_bitmap_required_size(bitmap);
    if (bitmap->size < required_size)
    {
        return;
    }

    size_t row_bytes = ((size_t)bitmap->width + 7U) / 8U;

    for (uint16_t by = 0U; by < bitmap->height; by++)
    {
        for (uint16_t bx = 0U; bx < bitmap->width; bx++)
        {
            size_t byte_index = (size_t)by * row_bytes + ((size_t)bx / 8U);
            uint8_t bit_mask = (uint8_t)(0x80U >> (bx % 8U));
            uint8_t pixel_on = (bitmap->data[byte_index] & bit_mask) ? 1U : 0U;

            GFX_DrawPixel(fb, (int16_t)(x + (int16_t)bx), (int16_t)(y + (int16_t)by), pixel_on);
        }
    }
}

int32_t GFX_Present(const GFX_Framebuffer *fb)
{
    if ((GFX_IsReady(fb) == 0U) || (fb->flush_cb == NULL))
    {
        return -1;
    }

    return fb->flush_cb(fb->flush_context,
                        fb->buffer,
                        GFX_CalcBufferSize(fb->width, fb->height),
                        fb->width,
                        fb->height);
}
