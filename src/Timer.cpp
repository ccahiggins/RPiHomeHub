#include "Timer.hpp"
//static const int BOILER_WATER=0;
//static const int BOILER_HEATING=1;

//number of seconds before triggering next event
int secondsToNextEvent;
int counter;
int idCounter;

//position of next event in timers vector
unsigned nextEventPos;


//vector containing list of timer schedule
std::vector<Timer::TimerEvent> timers;


Timer::Timer() {

	idCounter=0;
}

void Timer::writeToFile(std::string message) {
	
	std::ofstream myfile;
	myfile.open ("timer.txt", std::ios_base::app);
	
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	int hour=now->tm_hour;
	if (hour < 10) {
		myfile << "0" << hour << ":" ;
	}
	else {
		myfile << hour << ":";
	}
	int minute=now->tm_min;
	if (minute < 10) {
		myfile << "0" << minute << ":" ;
	}
	else {
		myfile << minute << ":";
	}
	int second = now->tm_sec;
	if (second < 10) {
		myfile << "0" << second << ": ";
	}
	else {
		myfile << second << ": ";
	}

	myfile << message << std::endl;
	myfile.close();
}

//Add a new timerEvent to the list
bool Timer::addTimerEvent(Timer::TimerEvent event)
{
	bool ok=true;
	if (event.startHour < 0 || event.startHour > 24)
	{
		//cout << "startHour: " << event.startHour;
		ok=false;
	}
	if (event.startMinute < 0 || event.startMinute > 59)
	{
		//cout << ", startmin: " << event.startMinute;
		ok=false;
	}
	if (event.duration <= 0)
	{
		//cout << ", duration: " << event.duration;
		ok=false;
	}
	if (event.boilerItem < 0 || event.boilerItem > 1)
	{
		//cout << ", boilerItem: " << event.boilerItem;
		ok=false;
	}
	
	if (ok)
	{
		event.id=idCounter++;
		timers.push_back(event);
		saveTimers();
		
		getNextTimerEvent();
		saveTimers();
		return true;
	}
	else
	{
		saveTimers();
		return false;
	}
}

bool Timer::enableTimerEvent(int id)
{
	for (unsigned x=0; x<timers.size(); x++)
	{
		if (timers[x].id==id)
		{
			if (timers[x].enabled==false)
			{
				timers[x].enabled=true;
				getNextTimerEvent();
				saveTimers();
				return true;
			}
		}
	}
	saveTimers();
	return false;
}

bool Timer::disableTimerEvent(int id)
{
	for (unsigned x=0; x<timers.size(); x++)
	{
		if (timers[x].id==id)
		{
			if (timers[x].enabled)
			{
				timers[x].enabled=false;
				getNextTimerEvent();
				saveTimers();
				return true;
			}
		}
	}
	saveTimers();
	return false;
}

std::vector<Timer::TimerEvent>& Timer::getTimers()
{	
	return timers;
}

bool Timer::addTimerEvent(int startHour, int startMinute, int duration, int boilerItem, bool enabled, bool oneTime) {

	if (startHour == 24)
		startHour=0;
	bool ok=true;
	if (startHour < 0 || startHour > 24)
	{
		//cout << "startHour: " << startHour;
		ok=false;
	}
	if (startMinute < 0 || startMinute > 59)
	{
		//cout << ", startmin: " << startMinute;
		ok=false;
	}
	if (duration <= 0)
	{
		//cout << ", duration: " << duration;
		ok=false;
	}
	if (boilerItem < 0 || boilerItem > 1)
	{
		//cout << ", boilerItem: " << boilerItem;
		ok=false;
	}
	
	for (unsigned x=0; x<timers.size(); x++)
	{
		if (startHour == timers[x].startHour &&
			startMinute == timers[x].startMinute &&
			boilerItem == timers[x].boilerItem)
			{
				ok = false;
			}
	}
	
	if (ok)
	{
		TimerEvent event = {
			startHour,
			startMinute,
			duration,
			boilerItem,
			enabled,
			oneTime,
			idCounter++
		};
		
		
		if (ok)
		{
			timers.push_back(event);
			saveTimers();
			getNextTimerEvent();
			saveTimers();
			return true;
		}
	}
	saveTimers();
	return false;
}

//Remove timerEvent from the list at position
/* bool Timer::removeTimerEvent(unsigned position)
{
	//cout << "pos: "<< position << endl;
	//cout << "size: "<< timers.size() << endl;
	if ((position > 0) && (position < timers.size()))
	{
		timers.erase(timers.begin() + position);
		getNextTimerEvent();
		return true;
	}
	else
		return false;
} */

bool Timer::removeTimerEvent(int id)
{
	for (unsigned x=0; x<timers.size(); x++)
	{
		if (timers[x].id==id)
		{
			timers.erase(timers.begin() + x);
			getNextTimerEvent();
			saveTimers();
			return true;
		}
	}
	saveTimers();
	return false;
}

/*
bool updateTimerEvent()
{
	
} */

//Get number of minutes between two times
//						current time           time to compare
//int minsBetweenTimes(int hour1, int minute1, int hour2, int minute2);

//Get number of seconds between now and next event time
int Timer::getSecondsToNextEvent(TimerEvent& event)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	
	int hour = now->tm_hour;
	int min =now->tm_min;
	int sec = now->tm_sec;
	
	int minsToNext = minsBetweenTimes(hour, min, event.startHour, event.startMinute);
		
	//cout << "Minutes to next event is " << minsToNext << endl;
	
	int secondsToNext = (minsToNext * 60) - (sec-1);
	
	//stringstream ss;
	//ss << "Now:" << hour << ":" << min << ":" <<sec << endl;
	//ss << event.startHour << ":"<< event.startMinute << endl;
	//ss << "secondstonext:" << secondsToNext << endl;
	//logfile(ss.str());


	return secondsToNext;
}
//void getNextTimerEvent();

//Check next event
//Trigger next event when event time matches current time
Timer::TimerEvent* Timer::checkTimer(int interval)
{
	
	if (secondsToNextEvent > 0)
	{
		//cout << "SecondsToNesecondsToNextEvent: " << secondsToNextEvent << endl;
		//cout << "nextEventPos: " << nextEventPos << endl;
		//stringstream ss;
		//ss << "Counter:" << counter << ", secondstonext:" << secondsToNextEvent << endl;
		//logfile(ss.str());
		
		counter += interval;
	//	if (counter >= (secondsToNextEvent - 250))
		{
			
			time_t t = time(0);   // get time now
			struct tm * now = localtime( & t );
							
		//	cout << "tm_min:" << now->tm_min << ", timer.startmin:" << timers[nextEventPos].startMinute << endl;
			int nowMins = now->tm_min;
			int nowHour = now->tm_hour;
			//ss << "now: " << nowHour << ":" << nowMins << ", starttime:" << timers[nextEventPos].startHour << ":" << timers[nextEventPos].startMinute << ", secondstonext: " << secondsToNextEvent << endl;
			//if (nowMins == 0)
			//	nowMins=60;
			if (nowMins == timers[nextEventPos].startMinute && nowHour==timers[nextEventPos].startHour)
			{
				secondsToNextEvent=0;
				counter=0;
				
				
				//cout << "Event triggered at "
				//<< now->tm_hour << ":"
				//<< now->tm_min << ":"
				//<< now->tm_sec
				//<< endl;
				
				//cout << "Hour:" << timers[nextEventPos].startHour << ", Minute: " << timers[nextEventPos].startMinute
				//	 << ", Duration: " << timers[nextEventPos].duration << ", BoilerItem: " << timers[nextEventPos].boilerItem
				//	 << ", Enabled: " << timers[nextEventPos].enabled
				//<< ", OneTime: " << timers[nextEventPos].oneTime << endl;
					 
				if (timers[nextEventPos].oneTime)
					timers[nextEventPos].enabled = false;
					 
				//getNextTimerEvent();
				
				//timerEvent *ptr=&timers[nextEventPos];
				
				//return ptr;
				//cout << "retutning this event:" <<  timers[nextEventPos].startHour << ":" << timers[nextEventPos].startMinute << ", duration:" << timers[nextEventPos].duration << ", boilerItem:" << timers[nextEventPos].boilerItem << endl;
				//timers[nextEventPos].enabled=false;
				return &timers[nextEventPos];
			}
			/* else
			{
				cout << "Secs+30 SecsToNext: " << secondsToNextEvent << " EventMins: " << timers[nextEventPos].startMinute << " Mins now: " << now->tm_min << endl;
				secondsToNextEvent+=30;
			} */
		}
		
	}
	return NULL;
}

//Get number of minutes between two times
//						current time           time to compare
int Timer::minsBetweenTimes(int hour1, int minute1, int hour2, int minute2)
{

	//stringstream ss;
	//ss<<hour1<<";"<<minute1<<" , "<< hour2 << ";" << minute2<<endl ;
	int mins1=(hour1*60) + minute1;
	int mins2=(hour2*60) + minute2;
	
	if (mins2>mins1)
	{
		//cout << "Greater than, mins2: " << mins2 << ", mins1: " << mins1  
		//<< "Mins2-mins1: " << mins2-mins1 << endl;
		//ss << mins2-mins1;
		//logfile(ss.str());
		return mins2-mins1;
	}
	else
	{
		//cout << "Less than, mins2: " << mins2 << ", mins1: " << mins1  
		//<< ", Mins2-mins1: " << mins2-mins1 << endl;
		//cout << "mins2: " << mins2 << ", 60*24: " << 60*24 << ", mins2+60*24: " << mins2+(60*24) << endl;
		//cout << "(mins2+(60*24))-mins1: " << (mins2+(60*24))-mins1 << endl;
		//ss << (mins2+(60*24))-mins1;
		//logfile(ss.str());
		return ((mins2+(60*24))-mins1);
	}
}

//Get the position of the next event in the list
//Set the second to next event so timer can run
 void Timer::getNextTimerEvent() {
	std::ostringstream oss;
	
	secondsToNextEvent=0;
	counter=0;
		
	if (timers.size() > 0)
	{		
			
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
		int hour=now->tm_hour;
		int minute=now->tm_min;
		
		int minMinutes = 25*60; //25hrsx60mins
			
		int position=0;
		
		bool isEvent=false;
		
		int y = 0;
		
		for (unsigned x = 0; x < timers.size(); x++)
		{
			TimerEvent& event = timers[x];
			//cout << "Timer to check: " << timers[x].startHour << ":" << timers[x].startMinute << ", enabled: " << timers[x].enabled << endl;
			oss << "Check for timer: " << timers[x].startHour << ":" << timers[x].startMinute << ", enabled: " << timers[x].enabled << std::endl;
			if (event.enabled)
			{
				isEvent=true;
				int timeBetween = minsBetweenTimes(hour, minute, event.startHour, event.startMinute);
				if (y > 0)
					//cout << ",";
				//cout << timers[x].startHour << ":" << timers[x].startMinute;
				if (x < timers.size())
				if ((timeBetween < minMinutes ) && timeBetween > 0)
				{
					minMinutes = timeBetween;
					position = x;
				}
				y++;
			}
		}
		if (isEvent)
		{
			//cout << endl;
			nextEventPos = position;
			secondsToNextEvent = getSecondsToNextEvent(timers[nextEventPos]);
			//cout << "Next timer event: " << timers[nextEventPos].startHour << ":" << timers[nextEventPos].startMinute << endl << endl;
			//writeToFile("Next timer event: "
			
			oss << "Next timer event: " << timers[nextEventPos].startHour << ":" << timers[nextEventPos].startMinute;
			//writeToFile(oss.str());
		}
		//else{
		//	writeToFile("No timers to get");
		//}
	}
}

void Timer::saveTimers() {
	
	std::ofstream f;
	f.open("timers.txt");
	
	for (unsigned x=0; x<timers.size(); x++)
	{
		f << timers[x].startHour << std::endl;
		f << timers[x].startMinute << std::endl;
		f << timers[x].duration << std::endl;
		f << timers[x].boilerItem << std::endl;
		f << timers[x].enabled << std::endl;
		f << timers[x].oneTime << std::endl;
		f << timers[x].id << std::endl;
	}
	
	f.close();
}

void Timer::loadTimers() {

	std::cout << "\033[32mLoading timers\033[0m" << std::endl;
	
	std::ifstream f;
	f.open("timers.txt");
	
	int x;
	std::vector<int> timerData;
	
	while (f >>  x) {
		timerData.push_back(x);
		if (timerData.size() == 7) {
			bool enabled = timerData[4];
			bool oneTime = timerData[5];
			TimerEvent t = {
				timerData[0],
				timerData[1],
				timerData[2],
				timerData[3],
				enabled,
				oneTime,
				idCounter++
			};
			timers.push_back(t);
			timerData.clear();
		}
	}
	if (timerData.size() > 0) {
		std::cout << "Error in timers file" << std::endl;
	}
	getNextTimerEvent();
	f.close();
}
