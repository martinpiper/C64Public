// --------------------------------------------------------------------------
// Advanced Linux Sound Architecture (ALSA) specific audio driver interface.
// --------------------------------------------------------------------------
/***************************************************************************
 *  $Log: audiodrv.cpp,v $
 *  Revision 1.10  2006/10/16 17:44:24  s_a_white
 *  Recover from a ctrl-z, bg which causes a buffer underrun during playback
 *
 *  Revision 1.9  2005/10/17 08:30:55  s_a_white
 *  Gentoo bug #98769.  goto open_error crosses initialization of
 *  `unsigned int rate'
 *
 *  Revision 1.8  2005/07/18 19:46:43  s_a_white
 *  Switch from obsolete alsa interface (patch by shd).
 *
 *  Revision 1.7  2002/03/04 19:07:48  s_a_white
 *  Fix C++ use of nothrow.
 *
 *  Revision 1.6  2002/01/10 19:04:00  s_a_white
 *  Interface changes for audio drivers.
 *
 *  Revision 1.5  2001/12/11 19:38:13  s_a_white
 *  More GCC3 Fixes.
 *
 *  Revision 1.4  2001/01/29 01:17:30  jpaana
 *  Use int_least8_t instead of ubyte_sidt which is obsolete now
 *
 *  Revision 1.3  2001/01/23 21:23:23  s_a_white
 *  Replaced SID_HAVE_EXCEPTIONS with HAVE_EXCEPTIONS in new
 *  drivers.
 *
 *  Revision 1.2  2001/01/18 18:35:41  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#include "audiodrv.h"
#ifdef   HAVE_ALSA

#include <errno.h>
#include <stdio.h>
#ifdef HAVE_EXCEPTIONS
#   include <new>
#endif

Audio_ALSA::Audio_ALSA()
{
    // Reset everything.
    outOfOrder();
}

Audio_ALSA::~Audio_ALSA ()
{
    close ();
}

void Audio_ALSA::outOfOrder ()
{
    // Reset everything.
    _errorString = "None";
    _audioHandle = NULL;
}

float *Audio_ALSA::open (AudioConfig &cfg, const char *)
{
    AudioConfig tmpCfg;

    if (_audioHandle != NULL)
    {
        _errorString = "ERROR: Device already in use";
        return NULL;
     }

    snd_pcm_uframes_t    buffer_frames;
    snd_pcm_hw_params_t *hw_params = 0;

    if (snd_pcm_open (&_audioHandle, "default", SND_PCM_STREAM_PLAYBACK, 0))
    {
        _errorString = "ERROR: Could not open audio device.";
        goto open_error;
    }

    // May later be replaced with driver defaults.
    tmpCfg = cfg;

    if (snd_pcm_hw_params_malloc (&hw_params))
    {
        _errorString = "ERROR: could not malloc hwparams.";
        goto open_error;
    }

    if (snd_pcm_hw_params_any (_audioHandle, hw_params))
    {
        _errorString = "ERROR: could not initialize hw params";
        goto open_error;
    }

    if (snd_pcm_hw_params_set_access (_audioHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED))
    {
        _errorString = "ERROR: could not set access type";
        goto open_error;
    }
    
    if (snd_pcm_hw_params_set_format (_audioHandle, hw_params, SND_PCM_FORMAT_FLOAT))
    {
        _errorString = "ERROR: could not set sample format";
        goto open_error;
    }

    if (snd_pcm_hw_params_set_channels (_audioHandle, hw_params, tmpCfg.channels))
    {
        _errorString = "ERROR: could not set channel count";
        goto open_error;
    }

    {   // Gentoo bug #98769, comment 4
        unsigned int rate = tmpCfg.frequency;
        if (snd_pcm_hw_params_set_rate_near (_audioHandle, hw_params, &rate, 0))
        {
            _errorString = "ERROR: could not set sample rate";
            goto open_error;
        }
    }

    _alsa_to_frames_divisor = tmpCfg.channels;
    buffer_frames = 4096;
    snd_pcm_hw_params_set_period_size_near(_audioHandle, hw_params, &buffer_frames, 0);

    if (snd_pcm_hw_params (_audioHandle, hw_params))
    {
        _errorString = "ERROR: could not set hw parameters";
        goto open_error;
    }

    snd_pcm_hw_params_free (hw_params);
    hw_params = 0;

    if (snd_pcm_prepare (_audioHandle))
    {
        _errorString = "ERROR: could not prepare audio interface for use";
        goto open_error;
    }

    tmpCfg.bufSize = buffer_frames * _alsa_to_frames_divisor;					
#ifdef HAVE_EXCEPTIONS
    _sampleBuffer = new(std::nothrow) float[tmpCfg.bufSize];
#else
    _sampleBuffer = new float[tmpCfg.bufSize];
#endif

    if (!_sampleBuffer)
    {
        _errorString = "AUDIO: Unable to allocate memory for sample buffers.";
        goto open_error;
    }

    // Setup internal Config
    _settings = tmpCfg;
    // Update the users settings
    getConfig (cfg);
    return _sampleBuffer;

open_error:
    if (hw_params)
        snd_pcm_hw_params_free (hw_params);
    if (_audioHandle != NULL)
        close ();
    perror ("ALSA");
    return NULL;
}

// Close an opened audio device, free any allocated buffers and
// reset any variables that reflect the current state.
void Audio_ALSA::close ()
{
    if (_audioHandle != NULL )
    {
        snd_pcm_close(_audioHandle);
        delete[] _sampleBuffer;
        outOfOrder ();
    }
}

float *Audio_ALSA::reset ()
{
    return _sampleBuffer;   
}

float *Audio_ALSA::write ()
{
    if (_audioHandle == NULL)
    {
        _errorString = "ERROR: Device not open.";
        return NULL;
    }

    if (snd_pcm_writei  (_audioHandle, _sampleBuffer, _settings.bufSize / _alsa_to_frames_divisor) == -EPIPE)
        snd_pcm_prepare (_audioHandle); // Underrun
    return _sampleBuffer;
}

#endif // HAVE_ALSA
