#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cmdfx.h"

int main() {
    printf("CmdFX Sound Engine Example\n");
    printf("==========================\n");
    
    // Note: This example requires actual sound files to work properly
    // For demonstration, we'll use placeholder filenames
    
    const char* soundFile1 = "example_sound.wav";
    const char* soundFile2 = "background_music.wav";
    
    printf("1. Testing sound system initialization...\n");
    
    // Set global volume to 80%
    if (Sound_setVolumeAll(0.8) == 0) {
        printf("   ✓ Global volume set to 80%%\n");
    } else {
        printf("   ✗ Failed to set global volume\n");
    }
    
    double globalVolume;
    if (Sound_getVolumeAll(&globalVolume) == 0) {
        printf("   ✓ Current global volume: %.1f%%\n", globalVolume * 100);
    }
    
    printf("\n2. Testing sound playback (will fail without actual files)...\n");
    
    // Try to play a sound file
    int result = Sound_play(soundFile1);
    if (result == 0) {
        printf("   ✓ Successfully started playing %s\n", soundFile1);
        
        // Play for 2 seconds, then pause
        sleep(2);
        
        printf("   Pausing sound...\n");
        if (Sound_pause(soundFile1) == 0) {
            printf("   ✓ Sound paused\n");
        }
        
        sleep(1);
        
        printf("   Resuming sound...\n");
        if (Sound_resume(soundFile1) == 0) {
            printf("   ✓ Sound resumed\n");
        }
        
        sleep(2);
        
        printf("   Stopping sound...\n");
        if (Sound_stop(soundFile1) == 0) {
            printf("   ✓ Sound stopped\n");
        }
    } else {
        printf("   ✗ Failed to play %s (error code: %d)\n", soundFile1, result);
        printf("   This is expected if the sound file doesn't exist.\n");
    }
    
    printf("\n3. Testing looped playback...\n");
    
    // Try to play a sound in a loop
    result = Sound_playLooped(soundFile2, 3); // Play 3 times
    if (result == 0) {
        printf("   ✓ Successfully started looped playback of %s\n", soundFile2);
        
        // Let it play for a bit
        sleep(5);
        
        printf("   Stopping looped playback...\n");
        if (Sound_stop(soundFile2) == 0) {
            printf("   ✓ Looped playback stopped\n");
        }
    } else {
        printf("   ✗ Failed to play %s in loop (error code: %d)\n", soundFile2, result);
        printf("   This is expected if the sound file doesn't exist.\n");
    }
    
    printf("\n4. Testing volume control...\n");
    
    // Set individual sound volume (this will fail if sound isn't loaded)
    result = Sound_setVolume(soundFile1, 0.5);
    if (result == 0) {
        printf("   ✓ Set %s volume to 50%%\n", soundFile1);
        
        double volume;
        if (Sound_getVolume(soundFile1, &volume) == 0) {
            printf("   ✓ Current volume for %s: %.1f%%\n", soundFile1, volume * 100);
        }
    } else {
        printf("   ✗ Failed to set volume for %s (sound not loaded)\n", soundFile1);
    }
    
    printf("\n5. Testing global controls...\n");
    
    // Test global pause/resume (safe to call even with no sounds)
    printf("   Testing global pause...\n");
    if (Sound_pauseAll() == 0) {
        printf("   ✓ Global pause executed\n");
    }
    
    printf("   Testing global resume...\n");
    if (Sound_resumeAll() == 0) {
        printf("   ✓ Global resume executed\n");
    }
    
    printf("   Testing global stop...\n");
    if (Sound_stopAll() == 0) {
        printf("   ✓ Global stop executed\n");
    }
    
    printf("\n6. Volume edge cases...\n");
    
    // Test volume boundaries
    if (Sound_setVolumeAll(0.0) == 0) {
        printf("   ✓ Set global volume to 0%% (mute)\n");
    }
    
    if (Sound_setVolumeAll(1.0) == 0) {
        printf("   ✓ Set global volume to 100%% (max)\n");
    }
    
    // Test invalid volumes
    if (Sound_setVolumeAll(-0.1) != 0) {
        printf("   ✓ Correctly rejected negative volume\n");
    }
    
    if (Sound_setVolumeAll(1.1) != 0) {
        printf("   ✓ Correctly rejected volume > 100%%\n");
    }
    
    printf("\n============================\n");
    printf("Sound engine example completed!\n");
    printf("\nTo test with actual sound files:\n");
    printf("1. Add .wav files named 'example_sound.wav' and 'background_music.wav'\n");
    printf("2. Make sure they are in the same directory as this executable\n");
    printf("3. Run the example again to hear the actual sound playback\n");
    
    return 0;
}
