/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _ANDROID_H_
#define _ANDROID_H_

#if defined(__ANDROID__)

#include "backends/platform/android3d/portdefs.h"
#include "common/fs.h"
#include "common/archive.h"
#include "common/mutex.h"
#include "common/ustr.h"
#include "audio/mixer_intern.h"
#include "graphics/palette.h"
#include "graphics/surface.h"
#include "graphics/opengl/system_headers.h"
#include "backends/graphics3d/opengl/framebuffer.h"
#include "backends/modular-backend.h"
#include "backends/plugins/posix/posix-provider.h"
#include "backends/fs/posix/posix-fs-factory.h"

#include "backends/platform/android3d/events.h"
#include "backends/platform/android3d/texture.h"
#include "backends/platform/android3d/graphics.h"
#include "backends/platform/android3d/touchcontrols.h"

#include <pthread.h>

#include <android/log.h>

// toggles start
//#define ANDROID_DEBUG_ENTER
//#define ANDROID_DEBUG_GL
//#define ANDROID_DEBUG_GL_CALLS
// toggles end

extern const char *android_log_tag;

#define _ANDROID_LOG(prio, fmt, args...) __android_log_print(prio, android_log_tag, fmt, ## args)
#define LOGD(fmt, args...) _ANDROID_LOG(ANDROID_LOG_DEBUG, fmt, ##args)
#define LOGI(fmt, args...) _ANDROID_LOG(ANDROID_LOG_INFO, fmt, ##args)
#define LOGW(fmt, args...) _ANDROID_LOG(ANDROID_LOG_WARN, fmt, ##args)
#define LOGE(fmt, args...) _ANDROID_LOG(ANDROID_LOG_ERROR, fmt, ##args)

#ifdef ANDROID_DEBUG_ENTER
#define ENTER(fmt, args...) LOGD("%s(" fmt ")", __FUNCTION__, ##args)
#else
#define ENTER(fmt, args...) do {  } while (false)
#endif

#ifdef ANDROID_DEBUG_GL
extern void checkGlError(const char *expr, const char *file, int line);

#ifdef ANDROID_DEBUG_GL_CALLS
#define GLCALLLOG(x, before) \
	do { \
		if (before) \
			LOGD("calling '%s' (%s:%d)", x, __FILE__, __LINE__); \
		else \
			LOGD("returned from '%s' (%s:%d)", x, __FILE__, __LINE__); \
	} while (false)
#else
#define GLCALLLOG(x, before) do {  } while (false)
#endif

#define GLCALL(x) \
	do { \
		GLCALLLOG(#x, true); \
		(x); \
		GLCALLLOG(#x, false); \
		checkGlError(#x, __FILE__, __LINE__); \
	} while (false)

#define GLTHREADCHECK \
	do { \
		assert(pthread_self() == _main_thread); \
	} while (false)

#else
#define GLCALL(x) do { (x); } while (false)
#define GLTHREADCHECK do {  } while (false)
#endif

class OSystem_Android : public ModularGraphicsBackend, Common::EventSource {
private:
	// passed from the dark side
	int _audio_sample_rate;
	int _audio_buffer_size;

	int _screen_changeid;

	pthread_t _main_thread;

	bool _timer_thread_exit;
	pthread_t _timer_thread;
	static void *timerThreadFunc(void *arg);

	bool _audio_thread_exit;
	pthread_t _audio_thread;
	static void *audioThreadFunc(void *arg);

	bool _virtkeybd_on;

	Audio::MixerImpl *_mixer;
	timeval _startTime;

	Common::String getSystemProperty(const char *name) const;

	void setupKeymapper();

public:
	OSystem_Android(int audio_sample_rate, int audio_buffer_size);
	virtual ~OSystem_Android();

	virtual void initBackend();

	virtual bool hasFeature(OSystem::Feature f);
	virtual void setFeatureState(OSystem::Feature f, bool enable);
	virtual bool getFeatureState(OSystem::Feature f);

public:
	void pushEvent(int type, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);

private:
	Common::Queue<Common::Event> _event_queue;
	Common::Event _queuedEvent;
	uint32 _queuedEventTime;
	Common::Mutex *_event_queue_lock;

	Common::Point _touch_pt_down, _touch_pt_scroll, _touch_pt_dt;
	int _eventScaleX;
	int _eventScaleY;
	bool _touchpad_mode;
	int _touchpad_scale;
	int _trackball_scale;
	int _dpad_scale;
	int _fingersDown;

	void clipMouse(Common::Point &p);
	void scaleMouse(Common::Point &p, int x, int y, bool touchpadMode = false);
	void disableCursorPalette();

	TouchControls _touchControls;

public:

	virtual void pushEvent(const Common::Event &event);
	virtual void pushKeyPressEvent(Common::Event &event);
	virtual bool pollEvent(Common::Event &event);
	virtual Common::KeymapperDefaultBindings *getKeymapperDefaultBindings();

	virtual uint32 getMillis(bool skipRecord = false);
	virtual void delayMillis(uint msecs);
	virtual Common::MutexInternal *createMutex();

	virtual void quit();

	virtual void setWindowCaption(const Common::U32String &caption);
	virtual void showVirtualKeyboard(bool enable);

	virtual Audio::Mixer *getMixer();
	virtual void getTimeAndDate(TimeDate &td, bool skipRecord = false) const;
	virtual void logMessage(LogMessageType::Type type, const char *message);
	virtual void addSysArchivesToSearchSet(Common::SearchSet &s,
											int priority = 0);
	virtual bool openUrl(const Common::String &url);
	virtual bool hasTextInClipboard();
	virtual Common::U32String getTextFromClipboard();
	virtual bool setTextInClipboard(const Common::U32String &text);
	virtual bool isConnectionLimited();
	virtual Common::String getSystemLanguage() const;

	void updateEventScale(const GLESBaseTexture *tex);
	TouchControls* getTouchControls() { return &_touchControls; }
};

#endif
#endif
