#include "UI_Clock.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "Brofiler\Brofiler.h"


void Clock::setStartValue(int new_start_value)
{
	start_value = new_start_value;
}

void Clock::setAlarm(int alarm)
{
	alarms.push_back(alarm);
}

void Clock::restartChrono()
{
	switch (this->type)
	{
	case TIMER:
		time = start_value;
		break;
	case STOPWATCH:
		time = 0;
		break;
	}
}

void Clock::BlitElement()
{
	BROFILER_CATEGORY("Clock Blit", Profiler::Color::PowderBlue);

	time_elapsed = counter.ReadSec();

	switch (type)
	{
	case STOPWATCH:
		if (time != time_elapsed)
		{
			time = time_elapsed;

			if (callback != nullptr) //If has callback send event
			{
				for (int i = 0; i < alarms.size(); i++)
				{
					if (time == (int)alarms[i])
						callback->OnUIEvent(this, STOPWATCH_ALARM);
				}
			}
			std::string secs = std::to_string(time);
			//p2SString secs("%04d", time);

			if (last_secs != secs)
				text->setText(secs);

			section = text->section;
			last_secs = std::to_string(time);
		}
		break;
	case TIMER:
		if (start_value - time_elapsed != time && time != 0)
		{
			time = start_value - time_elapsed;

			if (time == 0 && callback != nullptr) //If has callback send event
				callback->OnUIEvent(this, TIMER_ZERO);

			//p2SString secs("%d", time);
			std::string secs = std::to_string(time);

			if (last_secs != secs)
				text->setText(secs);

			section = text->section;
			last_secs = std::to_string(time);
		}
		break;
	}

	text->BlitElement();
}