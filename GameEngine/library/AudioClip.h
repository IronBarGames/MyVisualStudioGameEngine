#ifndef FCPP_AUDCLIP_H
#define FCPP_AUDCLIP_H

#include "LibGFX.h"

/// <summary>
/// handles audio of file types .mp3, .ogg, and .wav
/// </summary>
class AudioClip
{
private:
	Mix_Music* clip{};
	Mix_Chunk* wClip{};
	string name{};
	int chan{};

public:
	bool loop{ false };

	AudioClip(string file, int chan)
	{
		this->chan = chan;
		name = file;
		if (strfind(name, ".wav"))
		{
			wClip = Mix_LoadWAV(file.c_str());
		}
		else clip = Mix_LoadMUS(file.c_str());		
	}

	explicit AudioClip(const char* file)
	{
		name = file;
		if (strfind(name, ".wav"))
		{
			wClip = Mix_LoadWAV(file);
		}
		else clip = Mix_LoadMUS(file);
	}

	void Channel(int chan)
	{
		this->chan = chan;
	}

	AudioClip() {}

	inline void SetVolume(int vol)
	{
		Mix_Volume(chan, vol);
	}

	explicit AudioClip(int chan)
	{
		this->chan = chan;
	}

	void open(string file)
	{
		name = file;
		if (strfind(name, ".wav"))
		{
			wClip = Mix_LoadWAV(file.c_str());
		}
		else clip = Mix_LoadMUS(file.c_str());
	}

	void new_track(string file)
	{
		if (strfind(file, ".wav"))
		{
			Mix_FreeChunk(wClip);
			wClip = Mix_LoadWAV(file.c_str());
		}
		else
		{
			Mix_FreeMusic(clip);
			clip = Mix_LoadMUS(file.c_str());
		}
			name = file;
	}

	bool isPlaying()
	{
		if (strfind(name, ".wav"))
		{
			return (Mix_Playing(chan) && !Mix_Paused(chan));
		}
		else
		{
			return (Mix_PlayingMusic() && !Mix_PausedMusic());
		}
	}

	const string Name()
	{
		return name;
	}

	void Pause()
	{
		if (strfind(name, ".wav"))
		{
			Mix_Pause(chan);
		}
		else
		{
			Mix_PauseMusic();
		}
	}

	void Play(bool seq = true)
	{
		if (strfind(name, ".wav"))
		{
			if (seq)
			{
				if (!Mix_Playing(chan) || Mix_Paused(chan))
				{
					if (loop) { if (!Mix_Paused(chan)) Mix_PlayChannel(chan, wClip, -1); else if (Mix_Paused(chan)) Mix_Resume(chan); }
					else if (!loop) { if (!Mix_Paused(chan)) Mix_PlayChannel(chan, wClip, 0); else if (Mix_Paused(chan)) Mix_Resume(chan); }
				}
			}
			else
			{
				if (loop) { if (!Mix_Paused(chan)) Mix_PlayChannel(chan, wClip, -1); else if (Mix_Paused(chan)) Mix_Resume(chan); }
				else if (!loop) { if (!Mix_Paused(chan)) Mix_PlayChannel(chan, wClip, 0); else if (Mix_Paused(chan)) Mix_Resume(chan); }
			}
		}
		else
		{
			if (seq)
			{
				if (!Mix_PlayingMusic() || Mix_PausedMusic())
				{
					if (loop) { if (!Mix_PausedMusic()) Mix_PlayMusic(clip, -1); else if (Mix_PausedMusic()) Mix_ResumeMusic(); }
					else if (!loop) { if (!Mix_PausedMusic()) Mix_PlayMusic(clip, 0); else if (Mix_PausedMusic()) Mix_ResumeMusic(); }
				}
			}
			else
			{
				if (loop) { if (!Mix_PausedMusic()) Mix_PlayMusic(clip, -1); else if (Mix_PausedMusic()) Mix_ResumeMusic(); }
				else if (!loop) { if (!Mix_PausedMusic()) Mix_PlayMusic(clip, 0); else if (Mix_PausedMusic()) Mix_ResumeMusic(); }
			}
		}
	}

	void Toggle()
	{
		bool t{ isPlaying() };
		if (t) Pause();
		else if (!t) Play();
	}

	void Close()
	{
		if (strfind(name, ".wav"))
		{
			Mix_FreeChunk(wClip);
		}
		else
		{
			Mix_FreeMusic(clip);
		}
	}
};

typedef std::vector<AudioClip>AudioClips;

void ClearAudioClips(AudioClips& aud)
{
	while (aud.size() > 0)
	{
		aud.back().Close();
		aud.pop_back();
		aud.shrink_to_fit();
	}
}

#endif