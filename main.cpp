// g++ main.cpp -lzip -lSDL -lSDL_mixer -o Tower && ./Tower

#include <zip.h>	// requires libzip
#include <SDL/SDL_mixer.h>

#include <iostream>

using namespace std;

char* PlayMIDI_FromZIP_File(const char* zipfile, const char* midifile)
{
	// Open the ZIP archive
	int err = 0;
	zip *z = zip_open(zipfile, 0, &err);

	// Search for the file of given name
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, midifile, 0, &st);

	// Alloc memory for its uncompressed contents
	char *contents = new char[st.size];

	// Read the compressed file
	zip_file *f = zip_fopen(z, midifile, 0);
	zip_fread(f, contents, st.size);
	if (f != NULL)
	{
		zip_fclose(f);
	}

	// Close the archive
	zip_close(z);

	// Create a memory I/O stream for Mix_Music
	SDL_RWops* rwops = SDL_RWFromMem((unsigned char*)contents, st.size);

	if (rwops != NULL)
	{
		// Load the music from memory
		Mix_Music* music = Mix_LoadMUS_RW(rwops);

		// Close I/O
		SDL_RWclose(rwops);

		// Display and error if there is a problem
		if(!music)
		{
			cout << Mix_GetError() << endl;
		}
		else if (!Mix_PlayingMusic())
		{
			// Play the music
			Mix_PlayMusic(music, -1);
		}
	}
	else
	{
		delete[] contents;
		contents = NULL;
	}

	return contents; // MIDI memory space to be deleted
}

int main()
{
	cout << "  ║                 ║  " << endl;
	cout << "  ║  TOWER OF DOOM  ║  " << endl;
	cout << "  ║                 ║  " << endl;
	cout << "  ╬                 ╬  " << endl;
	cout << "╘═╬═╛             ╘═╬═╛" << endl;
	cout << "  ╬                 ╬  " << endl;

	// Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cerr << "SDL_mixer could not be initialized." << endl;
	}
	else
	{
		char* mem_midi = PlayMIDI_FromZIP_File("data.zip", "07.MID");

		system("sleep 60");

		Mix_HaltMusic();
		if (mem_midi != NULL)
		{
			delete[] mem_midi;
		}
	}
}