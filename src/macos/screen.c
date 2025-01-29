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
    CGDirectDisplayID display = CGMainDisplayID();
    
    if (__builtin_available(macOS 15, *)) {
        // macOS 15+ - Use CGDisplayStream
        __block int result = -1;
        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
        
        CGDisplayStreamRef stream = CGDisplayStreamCreateWithDispatchQueue(
            display,
            1, 1, // Smallest resolution to fetch a single pixel
            kCVPixelFormatType_32BGRA,
            NULL,
            dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
            ^(CGDisplayStreamFrameStatus status, uint64_t time, IOSurfaceRef frameSurface, CGDisplayStreamUpdateRef updateRef) {
                if (status == kCGDisplayStreamFrameStatusFrameComplete && frameSurface) {
                    size_t bytesPerRow;
                    void* baseAddress = IOSurfaceLock(frameSurface, kIOSurfaceLockReadOnly, &bytesPerRow);
                    if (baseAddress) {
                        uint8_t* pixels = (uint8_t*)baseAddress + (y * bytesPerRow) + (x * 4);
                        int red = pixels[2];
                        int green = pixels[1];
                        int blue = pixels[0];
                        result = (red << 16) | (green << 8) | blue;
                        IOSurfaceUnlock(frameSurface, kIOSurfaceLockReadOnly, &bytesPerRow);
                    }
                }
                dispatch_semaphore_signal(semaphore);
            }
        );

        if (!stream) {
            fprintf(stderr, "Failed to create display stream.\n");
            return -1;
        }

        CGDisplayStreamStart(stream);
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        CGDisplayStreamStop(stream);
        CFRelease(stream);

        return result;
    } else {
        // macOS 13-14 - Use CGDisplayCreateImage
        CGImageRef image = CGDisplayCreateImage(display);
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
}