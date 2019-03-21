
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

@property (nonatomic, assign) BOOL needPlay;

+ (id)shared;

- (void) resume;
- (void) destory;

// 将note音放入播放数组等待播放，用于同时播放多个音
- (void) pushNoteToQueueOn:(int)midiNoteNumber;

@end
