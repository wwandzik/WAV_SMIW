
#include "sndfile.h"
#include <stdlib.h>

#define SAMPLE_SIZE short

void process_sample(SAMPLE_SIZE *sample1, SAMPLE_SIZE *sample2)
{
	*sample1 = *sample1 + 2000000;
	*sample2 = *sample2 + 20;
}



void close_sndfiles(SNDFILE *input, SNDFILE *output)
{
	sf_close(input);
	sf_close(output);
}


void free_buffer(SAMPLE_SIZE *buffer, int channels)
{
	free(buffer);
}


void write_output(SNDFILE *output, SAMPLE_SIZE *buffer, int channels, int frames)
{
	sf_write_short(output, buffer, channels * frames);
}


void process_samples(SAMPLE_SIZE *buffer, int channels, int frames)
{
	if (channels != 2)
	{
		puts("Cannot process samples because number of channels is not equal to 2!");
		return;
	}
	for (int i = 0; i < frames * channels; i += 2)
	{
		process_sample(&(buffer[i]), &(buffer[i + 1]));
	}
}


void read_input(SNDFILE *input, SAMPLE_SIZE *buffer, int channels, int frames)
{
	sf_read_short(input, buffer, channels * frames);
}


void alloc_buffer(SAMPLE_SIZE **buffer, int channels, int frames)
{
	*buffer = malloc(channels * frames * sizeof(int));
}


void open_sndfiles(char *input, SNDFILE **in_file, char *output, SNDFILE **out_file, SF_INFO *input_info)
{
	SF_INFO info;
	info.format = 0;
	*in_file = sf_open(input, SFM_READ, &info);
	*input_info = info;

	puts("File information:");
	printf("Frames:\t\t%d\n", info.frames);
	printf("Sample rate:\t%d\n", info.samplerate);
	printf("Channels:\t%d\n", info.channels);
	printf("Format:\t\t%d\n", info.format);

	SF_INFO info2;
	info2.channels = info.channels;
	info2.format = info.format;
	info2.samplerate = info.samplerate;

	*out_file = sf_open(output, SFM_WRITE, &info2);
}


int main()
{
	SNDFILE *input, *output;
	SF_INFO info;
	SAMPLE_SIZE *buffer;
	
	open_sndfiles("../input.wav", &input, "../output.wav", &output, &info);
	int channels = info.channels, frames = info.frames;

	alloc_buffer(&buffer, channels, frames);
	read_input(input, buffer, channels, frames);
	process_samples(buffer, channels, frames);
	write_output(output, buffer, channels, frames);
	free_buffer(buffer, channels);
	close_sndfiles(input, output);

	return 0;
}
