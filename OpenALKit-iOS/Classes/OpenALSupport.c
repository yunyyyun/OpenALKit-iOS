
#include "OpenALSupport.h"

ALvoid alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq)
{
	static alBufferDataStaticProcPtr proc = NULL;

	if (proc == NULL)
		proc = (alBufferDataStaticProcPtr)alcGetProcAddress(NULL, (const ALCchar *)"alBufferDataStatic");

	if (proc != NULL)
		proc(bid, format, data, size, freq);
}

ALvoid alcMacOSXMixerOutputRateProc(const ALdouble value)
{
	static alcMacOSXMixerOutputRateProcPtr proc = NULL;

	if (proc == NULL)
		proc = (alcMacOSXMixerOutputRateProcPtr)alcGetProcAddress(NULL, (const ALCchar *)"alcMacOSXMixerOutputRate");

	if (proc != NULL)
		proc(value);
}

void *GetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei *outSampleRate)
{
	OSStatus err = noErr;

	ExtAudioFileRef extRef = NULL;
	err = ExtAudioFileOpenURL(inFileURL, &extRef);
	if (err != noErr)
	{
		printf("GetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %d\n", (int)err);
		return NULL;
	}

	AudioStreamBasicDescription theFileFormat;
	UInt32 thePropertySize = sizeof(theFileFormat);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if (err != noErr)
	{
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %d\n", (int)err);
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	if (theFileFormat.mChannelsPerFrame > 2)
	{
		printf("GetOpenALAudioData: Unsupported format, channel count is greater than stereo\n");
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	// Set the output format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	AudioStreamBasicDescription theOutputFormat;
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;

	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if (err != noErr)
	{
		printf("GetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %d\n", (int)err);
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	SInt64 theFileLengthInFrames = 0;
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if (err != noErr)
	{
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %d\n", (int)err);
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	UInt32 dataSize = (UInt32)theFileLengthInFrames * theOutputFormat.mBytesPerFrame;
	void *theData = malloc(dataSize);
	if (theData == NULL)
	{
		printf("GetOpenALAudioData: malloc FAILED\n");
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	AudioBufferList theDataBuffer;
	theDataBuffer.mNumberBuffers = 1;
	theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
	theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
	theDataBuffer.mBuffers[0].mData = theData;

	err = ExtAudioFileRead(extRef, (UInt32 *)&theFileLengthInFrames, &theDataBuffer);
	if (err != noErr)
	{ 
		printf("GetOpenALAudioData: ExtAudioFileRead FAILED, Error = %d\n", (int)err);
		free(theData);
		ExtAudioFileDispose(extRef);
		return NULL;
	}

	*outDataSize = (ALsizei)dataSize;
	*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;

	ExtAudioFileDispose(extRef);
	return theData;
}

void* MyGetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*    outSampleRate)
{
    OSStatus                        err = noErr;
    SInt64                            theFileLengthInFrames = 0;
    AudioStreamBasicDescription        theFileFormat;
    UInt32                            thePropertySize = sizeof(theFileFormat);
    ExtAudioFileRef                    extRef = NULL;
    void*                            theData = NULL;
    AudioStreamBasicDescription        theOutputFormat;
    
    // Open a file with ExtAudioFileOpen()
    err = ExtAudioFileOpenURL(inFileURL, &extRef);
    if(err) {
        printf("MyGetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %d\n", (int)err);
        exit(1);
    }
    
    // Get the audio data format
    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
    if(err) {
        printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %d\n", err);
        exit(1);
    }
    if (theFileFormat.mChannelsPerFrame > 2)  {
        printf("MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
        exit(1);
        
    }
    
    // Set the client format to 16 bit signed integer (native-endian) data
    // Maintain the channel count and sample rate of the original source format
    theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
    theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
    
    theOutputFormat.mFormatID = kAudioFormatLinearPCM;
    theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mFramesPerPacket = 1;
    theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mBitsPerChannel = 16;
    theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    
    // Set the desired client (output) data format
    err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
    if(err) {
        printf("MyGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %d\n", err);
        exit(1);
        
    }
    
    // Get the total frame count
    thePropertySize = sizeof(theFileLengthInFrames);
    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
    if(err) {
        printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %d\n", err);
        exit(1); }
    
    // Read all the data into memory
    UInt32 theFramesToRead = (UInt32)theFileLengthInFrames;
    UInt32 dataSize = theFramesToRead * theOutputFormat.mBytesPerFrame;;
    theData = malloc(dataSize);
    if (theData)
    {
        AudioBufferList        theDataBuffer;
        theDataBuffer.mNumberBuffers = 1;
        theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
        theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
        theDataBuffer.mBuffers[0].mData = theData;
        
        // Read the data into an AudioBufferList
        err = ExtAudioFileRead(extRef, &theFramesToRead, &theDataBuffer);
        if(err == noErr)
        {
            // success
            *outDataSize = (ALsizei)dataSize;
            *outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            *outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
        }
        else
        {
            // failure
            free (theData);
            theData = NULL; // make sure to return NULL
            printf("MyGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %d\n", err);
            exit(1);
        }
    }
    
Exit:
    // Dispose the ExtAudioFileRef, it is no longer needed
    if (extRef) ExtAudioFileDispose(extRef);
    return theData;
}
