#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../test.h"
#include "cmdfx/sound/sound.h"

void test_sound_basic_api() {
    // Test basic API calls with non-existent file (should fail gracefully)
    int result;

    printf("Testing basic sound API...\n");

    // Test play with invalid file
    result = Sound_play(NULL);
    ASSERT(result != 0, "Sound_play with NULL should fail");

    result = Sound_play("nonexistent.wav");
    ASSERT(result != 0, "Sound_play with non-existent file should fail");

    // Test volume functions
    double volume;
    result = Sound_getVolumeAll(&volume);
    ASSERT(result == 0, "Sound_getVolumeAll should succeed initially");
    ASSERT(volume == 1.0, "Initial global volume should be 1.0");

    result = Sound_setVolumeAll(0.5);
    ASSERT(result == 0, "Sound_setVolumeAll should succeed");

    result = Sound_getVolumeAll(&volume);
    ASSERT(result == 0, "Sound_getVolumeAll should succeed after setting");
    ASSERT(volume == 0.5, "Global volume should be 0.5 after setting");

    // Test invalid volume values
    result = Sound_setVolumeAll(-0.1);
    ASSERT(result != 0, "Sound_setVolumeAll with negative volume should fail");

    result = Sound_setVolumeAll(1.1);
    ASSERT(result != 0, "Sound_setVolumeAll with volume > 1.0 should fail");

    // Reset volume
    result = Sound_setVolumeAll(1.0);
    ASSERT(result == 0, "Sound_setVolumeAll reset should succeed");

    printf("Basic sound API tests passed!\n");
}

void test_sound_volume_management() {
    printf("Testing sound volume management...\n");

    int result;
    double volume;

    // Test individual sound volume with non-existent file
    result = Sound_getVolume("nonexistent.wav", &volume);
    ASSERT(result != 0, "Sound_getVolume with non-existent file should fail");

    result = Sound_setVolume("nonexistent.wav", 0.5);
    ASSERT(result != 0, "Sound_setVolume with non-existent file should fail");

    // Test global volume edge cases
    result = Sound_setVolumeAll(0.0);
    ASSERT(result == 0, "Sound_setVolumeAll with 0.0 should succeed");

    result = Sound_getVolumeAll(&volume);
    ASSERT(result == 0 && volume == 0.0, "Global volume should be 0.0");

    result = Sound_setVolumeAll(1.0);
    ASSERT(result == 0, "Sound_setVolumeAll with 1.0 should succeed");

    result = Sound_getVolumeAll(&volume);
    ASSERT(result == 0 && volume == 1.0, "Global volume should be 1.0");

    printf("Sound volume management tests passed!\n");
}

void test_sound_control_functions() {
    printf("Testing sound control functions...\n");

    int result;

    // Test control functions with non-existent file
    result = Sound_pause("nonexistent.wav");
    ASSERT(result != 0, "Sound_pause with non-existent file should fail");

    result = Sound_resume("nonexistent.wav");
    ASSERT(result != 0, "Sound_resume with non-existent file should fail");

    result = Sound_stop("nonexistent.wav");
    ASSERT(result != 0, "Sound_stop with non-existent file should fail");

    // Test global control functions (should succeed even with no sounds)
    result = Sound_pauseAll();
    ASSERT(result == 0, "Sound_pauseAll should succeed with no sounds");

    result = Sound_resumeAll();
    ASSERT(result == 0, "Sound_resumeAll should succeed with no sounds");

    result = Sound_stopAll();
    ASSERT(result == 0, "Sound_stopAll should succeed with no sounds");

    printf("Sound control function tests passed!\n");
}

void test_sound_loop_parameters() {
    printf("Testing sound loop parameters...\n");

    int result;

    // Test playLooped with invalid parameters
    result = Sound_playLooped(NULL, 1);
    ASSERT(result != 0, "Sound_playLooped with NULL file should fail");

    result = Sound_playLooped("nonexistent.wav", 0);
    ASSERT(result != 0, "Sound_playLooped with 0 loops should fail");

    result = Sound_playLooped("nonexistent.wav", -2);
    ASSERT(
        result != 0, "Sound_playLooped with invalid negative loops should fail"
    );

    // Valid loop counts (should fail due to non-existent file, but parameters
    // are valid)
    result = Sound_playLooped("nonexistent.wav", 1);
    ASSERT(result != 0, "Sound_playLooped with non-existent file should fail");

    result = Sound_playLooped("nonexistent.wav", 5);
    ASSERT(result != 0, "Sound_playLooped with non-existent file should fail");

    result = Sound_playLooped("nonexistent.wav", -1); // Infinite loop
    ASSERT(result != 0, "Sound_playLooped with non-existent file should fail");

    printf("Sound loop parameter tests passed!\n");
}

int main() {
    printf("Running Sound Engine Tests...\n");
    printf("============================\n");

    test_sound_basic_api();
    test_sound_volume_management();
    test_sound_control_functions();
    test_sound_loop_parameters();

    printf("\n============================\n");
    printf("All sound tests passed! ✓\n");

    return 0;
}
