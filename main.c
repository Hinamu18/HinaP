#include "lib.h"
#include <stdbool.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


char *the_dir_path = NULL;
float volume = 0.5f;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
  if (pDecoder == NULL) return;

  ma_uint64 framesRead = 0;
  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, &framesRead);

  float* samples = (float*)pOutput;
  ma_uint64 totalSamples = framesRead * pDevice->playback.channels;

  // volume to all samples
  for (ma_uint64 i = 0; i < totalSamples; i++) {
    samples[i] *= volume;
  }

  (void)pInput;
}

int main(int argc, char *argv[])
{
  // add lang
  setlocale(LC_ALL, "");

  if (argc < 2) {
    printf("Usage: %s <FILE OR DIR>\n", argv[0]);
    return -1;
  }

  char* final_audio_path = NULL;
  char full_path_buffer[1024]; 

  struct stat st;
  if (stat(argv[1], &st) != 0) {
    printf("Error: %s not found.\n", argv[1]);
    return -1;
  }

  if(S_ISREG(st.st_mode)) {
    // direct 
    final_audio_path = argv[1];
  }else if(S_ISDIR(st.st_mode)) {
    // directory
    the_dir_path = argv[1];
    list_files();

    char* selected_file = draw_files_menu();

    if (selected_file == NULL) {
      printf("Exiting...\n");
      return 0;
    }

    snprintf(full_path_buffer, sizeof(full_path_buffer), "%s%s", the_dir_path, selected_file);
    final_audio_path = full_path_buffer;
  }

  ma_decoder decoder;
  if (ma_decoder_init_file(final_audio_path, NULL, &decoder) != MA_SUCCESS) {
    printf("Could not load audio file: %s\n", final_audio_path);
    return -1;
  }

  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format   = ma_format_f32;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate        = decoder.outputSampleRate;
  deviceConfig.dataCallback      = data_callback;
  deviceConfig.pUserData         = &decoder;

  ma_device device;
  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    ma_decoder_uninit(&decoder);
    return -1;
  }

  if (ma_device_start(&device) != MA_SUCCESS) {
    printf("Failed to start playback device.\n");
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    return -1;
  }

  int audio = audio_ui(final_audio_path);
  while (true){
    if (audio == -1) {
      break;
    }else if(audio == 1){
      ma_decoder_init_file(NULL, NULL, &decoder); 
      the_dir_path = argv[1];
      list_files();

      char* selected_file = draw_files_menu();

      if (selected_file == NULL) {
        break;
      }

      snprintf(full_path_buffer, sizeof(full_path_buffer), "%s%s", the_dir_path, selected_file);
      final_audio_path = full_path_buffer;
      
      if (ma_decoder_init_file(final_audio_path, NULL, &decoder) != MA_SUCCESS) {
        printf("Could not load audio file: %s\n", final_audio_path);
        return -1;
      }
      
      audio = audio_ui(final_audio_path);
    }
  }

  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);

  return 0;
}
