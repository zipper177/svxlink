/**
@file	 AsyncAudioDevice.h
@brief   Handle audio devices
@author  Tobias Blomberg
@date	 2004-03-20

A_detailed_description_for_this_file

\verbatim
<A brief description of the program or library this file belongs to>
Copyright (C) 2004  Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

/** @example Template_demo.cpp
An example of how to use the Template class
*/


#ifndef ASYNC_AUDIO_DEVICE_INCLUDED
#define ASYNC_AUDIO_DEVICE_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <sigc++/signal_system.h>

#include <string>
#include <map>
#include <list>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

namespace Async
{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class AudioIO;
class FdWatch;


/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	A_brief_class_description
@author Tobias Blomberg
@date   2004-03-20

A_detailed_class_description

\include Template_demo.cpp
*/
class AudioDevice : public SigC::Object
{
  public:
    /**
     * @brief The different modes to open a device in
     */
    typedef enum
    {
      MODE_NONE,  ///< No mode. The same as close
      MODE_RD,	  ///< Read
      MODE_WR,	  ///< Write
      MODE_RDWR   ///< Both read and write
    } Mode;
  
    static AudioDevice *registerAudioIO(const std::string& dev_name,
      	    AudioIO *audio_io);
    static void unregisterAudioIO(AudioIO *audio_io);
    
    /**
     * @brief 	Open the audio device
     * @param 	mode The mode to open the audio device in (See AudioIO::Mode)
     * @return	Returns \em true on success or else \em false
     */
    bool open(const Mode& mode);
    
    /**
     * @brief 	Close the audio device
     */
    void close(void);
    
    /**
     * @brief 	Tell the audio device handler that there are audio to be
     *	      	written in the buffer
     */
    void audioToWriteAvailable(void);

    /**
     * @brief 	Find out how many samples there are in the output buffer
     * @return	Returns the number of samples in the output buffer on
     *          success or -1 on failure.
     *
     * This function can be used to find out how many samples there are
     * in the output buffer at the moment. This can for example be used
     * to find out how long it will take before the output buffer has
     * been flushed.
     */
    int samplesToWrite(void) const;
    
    
    /**
     * @brief 	A signal that is emitted when a block of audio has been
     *	      	received from the audio device
     * @param 	buf   A buffer containing the read samples
     * @param 	count The number of samples in the buffer
     */
    SigC::Signal2<int, short *, int> audioRead;

    /**
     * @brief 	A signal that is emitted when the write buffer is full
     * @param 	is_full Set to \em true if the buffer is full or \em false
     *	      	      	if the buffer full condition has been cleared
     */
    SigC::Signal1<void, bool> writeBufferFull;
    
            
    
  protected:
    /**
     * @brief 	Default constuctor
     */
    AudioDevice(const std::string& dev_name);
  
    /**
     * @brief 	Destructor
     */
    ~AudioDevice(void);
  
    
  private:
    static const int  RATE = 8000;
    static const int  CHANNELS = 1;
    //static const int  SIZE = 16;
    //static const int  FRAG_COUNT = 32;    // 32 frags ~ one second
    //static const int  FRAG_SIZE_LOG2 = 8; // 256 bytes/frag
    static const int  BUF_FRAG_COUNT = 4;
    static std::map<std::string, AudioDevice*>  devices;
    
    std::string       	dev_name;
    int       	      	use_count;
    std::list<AudioIO*> aios;
    Mode      	      	current_mode;
    int       	      	fd;
    FdWatch	      	*read_watch;
    FdWatch	      	*write_watch;
    char      	      	*read_buf;
    
    void audioReadHandler(FdWatch *watch);
    void writeSpaceAvailable(FdWatch *watch);
    
};  /* class AudioDevice */


} /* namespace */

#endif /* ASYNC_AUDIO_DEVICE_INCLUDED */



/*
 * This file has not been truncated
 */

