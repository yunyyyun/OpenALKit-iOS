
#import <Foundation/Foundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>

#define NUM_SOURCES 32
@interface SoundBankPlayer : NSObject
@property (nonatomic, assign) BOOL loopNotes;

@property (nonatomic, assign) ALfloat pitchRate;

@property (nonatomic, assign) int noteOffset;

@property (nonatomic, strong) NSArray *soundsCategory;

@property (nonatomic, assign) NSInteger categoryIndex;

+ (id)shared;

- (void) resume;
- (void) destory;
- (void) pushNoteToQueueOn:(int)midiNoteNumber;
- (void) doPlayNoteQueue;
- (void)setSoundBank;
- (void)noteOn:(int)midiNoteNumber gain:(float)gain;
- (void)queueNote:(int)midiNoteNumber gain:(float)gain;
- (void)playQueuedNotes;
- (void)noteOff:(int)midiNoteNumber;
- (void)allNotesOff;

@end
