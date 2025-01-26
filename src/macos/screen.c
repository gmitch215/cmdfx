#include <stdio.h>

#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>

#include "cmdfx/screen.h"

// Screen API

void Screen_getSize(int* width, int* height) {
    CGDirectDisplayID display = CGMainDisplayID();
    *width = CGDisplayPixelsWide(display);
    *height = CGDisplayPixelsHigh(display);
}

int Screen_getRGB(int x, int y) {
    CGImageRef image = CGDisplayCreateImage(CGMainDisplayID());
    if (!image) {
        fprintf(stderr, "Failed to capture screen.\n");
        return -1;
    }

    CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(image));
    const uint8_t* pixels = CFDataGetBytePtr(data);

    size_t width = CGImageGetWidth(image);
    size_t height = CGImageGetHeight(image);
    size_t bytesPerRow = CGImageGetBytesPerRow(image);
    size_t bytesPerPixel = 4;

    if (x < 0 || y < 0 || x >= width || y >= height) {
        fprintf(stderr, "Coordinates out of bounds.\n");
        CFRelease(data);
        CGImageRelease(image);
        return -1;
    }

    size_t offset = y * bytesPerRow + x * bytesPerPixel;
    int red = pixels[offset];
    int green = pixels[offset + 1];
    int blue = pixels[offset + 2];

    CFRelease(data);
    CGImageRelease(image);

    return (red << 16) | (green << 8) | blue;
}