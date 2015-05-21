#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sqlite3.h>
#include <signal.h>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <sstream>
#include <sys/time.h>
#include <iomanip>
#include <ctime>
#include <vector>
using namespace std;


class Boiler
{
	//#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>


	struct payload_pin
	{
		uint16_t pinNumber;
		uint16_t pinState;
		uint16_t duration;
	};

	const static int waterPin = 4, heaterPin = 5;
	RF24Network& network;
	
	bool switchPin(int pin, int state)
	{
		RF24NetworkHeader header(03, 'p');
		payload_pin payload;
		payload.pinNumber = pin;
		payload.pinState=state;
		payload.duration=0;
		// payload.option = PIN_SET;
		
		bool ok = network.write(header,&payload,sizeof(payload));
		if (ok)
		{
			//std::cout << "true" << std::endl;
			return true;
		}
		else
		{ 
			//std::cout << "false" << std::endl;
			return false;
		}
	}
	
	bool switchPin(int pin, int state, int duration)
	{
		//std::cout << "turning on for " << duration << " minutes" << std::endl;
		RF24NetworkHeader header(03, 'p');
		payload_pin payload;
		payload.pinNumber = pin;
		payload.pinState=state;
		payload.duration=duration;
		// payload.option = PIN_SET;
		
		bool ok = network.write(header,&payload,sizeof(payload));
		//radioInUse=false;
		if (ok)
		{
			//std::cout << "true" << std::endl;
			return true;
		}
		else
		{ 
			//std::cout << "false" << std::endl;
			return false;
		}
	}
	
public:
	//Boiler();
	Boiler(RF24Network* network):network(*network){};
	
	
	bool TurnHeatingOn()
	{
		std::cout << "Turning heating on" << std::endl;
		if (switchPin(5, 0))
		return true;
		else
		return false;
	}
	
	bool TurnWaterOn()
	{
		std::cout << "Turning water on" << std::endl;
		if (switchPin(4, 0))
		return true;
		else
		return false;
	}
	
	bool TurnWaterOn(int duration)
	{
		std::cout << "Turning water on for " << duration << " minutes" << std::endl;
		if (switchPin(4, 0, duration))
		return true;
		else
		return false;
	}
	
	bool TurnWaterOff()
	{
		std::cout << "Turning water off " << std::endl;
		if (switchPin(4, 1))
		return true;
		else
		return false;
	}
	
	bool TurnHeatingOn(int duration)
	{
		std::cout << "Turning heating on for " << duration << " minutes" << std::endl;
		if (switchPin(5, 0, duration))
		return true;
		else
		return false;
	}
	
	bool TurnHeatingOff()
	{
		std::cout << "Turning heating off " << std::endl;
		if (switchPin(5, 1))
		return true;
		else
		return false;
	}
	
	int WaterStatus()
	{
		std::cout << "Getting status of water " << std::endl;
		RF24NetworkHeader header(03, 'p');
		payload_pin payload;
		payload.pinNumber = 4;
		payload.pinState=2;
		payload.duration=0;
		
		bool ok = network.write(header,&payload,sizeof(payload));
		if (ok)
		{
			//std::cout << "In receive status loop" << std::endl;
			for (int x=0; x<10; x++)
			{
				//std::cout << x << std::endl;
				network.update();
				while (network.available())
				{
					//std::cout << "In network (status) loop" << std::endl;
					RF24NetworkHeader header2;
					network.peek(header2);
					
					//std::cout << header2.type << std::endl;
					
					if (header2.type=='p')
					{
						payload_pin payload2;
						network.read(header2, &payload2, sizeof(payload2));
						//std::cout << "It's the status P bit" << payload2.pinState << std::endl;
						//std::cout << "Water payload duration " << payload2.duration << std::endl;
						if (payload2.duration > 0)
							return payload2.duration;
						if (payload2.pinState==0)
							return 0;
						if (payload2.pinState==1)
							return -1;
					}
					else
					{
						payload_pin payload2;
						network.read(header2, &payload2, sizeof(payload2));
					}
				}
				usleep(100000);
			}
		}
		else
		{ 
			//std::cout << "false" << std::endl;
			return 2;
		}
		return 2;
	}
	
int HeatingStatus()
	{
		std::cout << "Getting status of heating " << std::endl;
		RF24NetworkHeader header(03, 'p');
		payload_pin payload;
		payload.pinNumber = 5;
		payload.pinState=2;
		payload.duration=0;
		
		bool ok = network.write(header,&payload,sizeof(payload));
		if (ok)
		{
			//std::cout << "In receive status loop" << std::endl;
			for (int x=0; x<10; x++)
			{
				//std::cout << x << std::endl;
				network.update();
				while (network.available())
				{
					//std::cout << "In network (status) loop" << std::endl;
					RF24NetworkHeader header2;
					network.peek(header2);
					
					//std::cout << header2.type << std::endl;
					
					if (header2.type=='p')
					{
						payload_pin payload2;
						network.read(header2, &payload2, sizeof(payload2));
						//std::cout << "It's the status P bit" << payload2.pinState << std::endl;
						//std::cout << "Heating payload duration " << payload2.duration << std::endl;
						if (payload2.duration > 0)
							return payload2.duration;
						if (payload2.pinState==0)
							return 0;
						if (payload2.pinState==1)
							return -1;
					}
					else
					{
						payload_pin payload2;
						network.read(header2, &payload2, sizeof(payload2));
					}
				}
				usleep(100000);
			}
		}
		else
		{ 
			//std::cout << "false" << std::endl;
			return -2;
		}
		return -2;
	}
	
};








RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  

RF24Network network(radio);
Boiler boiler(&network);
// Address of our node in Octal format
const uint16_t this_node = 00;










#define PIN_GET 1;
#define PIN_SET 2;
//#define HIGH 1;
//#define LOW 0;
#define ERROR 2;

#define BOILER_HEATING 4;
#define BOILER 03;




struct payload_pin
{
	uint16_t pinNumber;
	uint16_t pinState;
	uint16_t option;
};

struct payload_temp {
	float temp;
	uint16_t voltage;
	uint16_t deviceNum;
};


struct payload_time{
	uint16_t hours;
	uint16_t minutes;
	uint16_t seconds;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "CivetServer.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DOCUMENT_ROOT "."
#define PORT "8081"
#define HUB_URI "/hub"
#define BOILER_URI "/boiler"
#define EXIT_URI "/exit"
bool exitNow = false;
bool sendPayload();

volatile bool radioInUse = false;


std::string printGraph();

class HubHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn) {
mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
		mg_printf(conn, "<html><head>");
		std::string hello=printGraph();
		mg_printf(conn, "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1\">");
		mg_printf(conn, "<link rel=\"stylesheet\" href=\"/styles.css\" type=\"text/css\" media=\"screen\" />");
		mg_printf(conn, hello.c_str());
		mg_printf(conn, "</head>");
        mg_printf(conn, "<body>");
		
		mg_printf(conn, "<div  class = \"enjoy-css\">Welcome to the Raspberry Pi Home Hub</div>\r\n");
		//mg_printf(conn, "<div class=\"ggg\">Adjust the heating controls</div>\r\n");
		
		while (radioInUse)
		{
			//std::cout << "Hub handler waiting" << std::endl;
			usleep(1000);
		} 
		radioInUse=true;
		int waterStatus = boiler.WaterStatus();
		//cout << waterStatus << endl;
		radioInUse=false;
		
		if (waterStatus == -2)
		mg_printf(conn, "<p>Cannot connect to boiler :(</p>\r\n");
		else
		{
			if (waterStatus==0)
			{
				mg_printf(conn, "<div class = \"heading\">Water override is <em>ON</em></div>\r\n");
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=wateroff\">Turn the water off</a></div>\r\n");
			}
			else if (waterStatus==-1)
			{
				mg_printf(conn, "<div class = \"heading\">Water override is <b>OFF</b></div>\r\n");
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=wateron\">Turn the water on</a> "
				"<a href=\"boiler?param=wateron15\">15</a> "
				"<a href=\"boiler?param=wateron30\">30</a> "
				"<a href=\"boiler?param=wateron45\">45</a> "
				"<a href=\"boiler?param=wateron60\">60</a></div>\r\n");
			}
			else if (waterStatus > 0)
			{
				mg_printf(conn, "<div class = \"heading\">Water override is <b>ON</b> for %i minutes</div>\r\n", waterStatus);
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=wateroff\">Turn the water off</a></div>\r\n");
			}
			
		}
		
		while (radioInUse)
		{
			//std::cout << "Hub handler waiting" << std::endl;
			usleep(1000);
		} 
		radioInUse=true;
		int heatingStatus = boiler.HeatingStatus();
		//cout << heatingStatus << endl;
		radioInUse=false;
		
		if (heatingStatus == -2)
			mg_printf(conn, "<p>Cannot connect to boiler :(</p>\r\n");
		else
		{
			if (heatingStatus==0)
			{
				mg_printf(conn, "<div class = \"heading\">Heating override is <b>ON</b></div>\r\n");
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=heatingoff\">Turn the heating off</a></div>\r\n");
			}
			else if (heatingStatus==-1)
			{
				mg_printf(conn, "<div class = \"heading\">Heating override is <b>OFF</b></div>\r\n");
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=heatingon\">Turn the heating on</a> "
				"<a href=\"boiler?param=heatingon15\">15</a> "
				"<a href=\"boiler?param=heatingon30\">30</a> "
				"<a href=\"boiler?param=heatingon45\">45</a> "
				"<a href=\"boiler?param=heatingon60\">60</a></div>\r\n");
			}
			else if (heatingStatus > 0)
			{
				mg_printf(conn, "<div class = \"heading\">Heating override is <b>ON</b> for %i minutes</div>\r\n", heatingStatus);
				mg_printf(conn, "<div class=\"hhh\"><a href=\"boiler?param=heatingoff\">Turn the heating off</a></div>\r\n");
			}
			
		}
		
		// mg_printf(conn, "<p>To see a page from the A handler with a parameter <a href=\"A?param=1\">click here</a></p>\r\n");
		// mg_printf(conn, "<p>To see a page from the A/B handler <a href=\"A/B\">click here</a></p>\r\n");
		// mg_printf(conn, "<p>To see a page from the *.foo handler <a href=\"xy.foo\">click here</a></p>\r\n");
		// mg_printf(conn, "<p>To exit <a href=\"%s\">click here</a></p>\r\n", EXIT_URI);
		mg_printf(conn, "<div id=\"chart_container\"><div id=\"linechart_material\" style=\"width:100%%; height:100%%\"</div></div>\n");
		mg_printf(conn, "</body></html>\r\n");
		return true;
	}
};

// class ExitHandler: public CivetHandler
// {
// public:
	// bool handleGet(CivetServer *server, struct mg_connection *conn) {
		// mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
		// mg_printf(conn, "Bye!\n");
		// exitNow = true;
		// return true;
	// }
// };

class BoilerHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn) {
		string s = "";
		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
		mg_printf(conn, "<html><head>");
		mg_printf(conn, "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1\">");
		mg_printf(conn, "<link rel=\"stylesheet\" href=\"/styles.css\" type=\"text/css\" media=\"screen\" />");
		mg_printf(conn, "</head>");
        mg_printf(conn, "<body>");
		mg_printf(conn, "<div  class = \"enjoy-css\">Boiler controller</div>");
		
		if (CivetServer::getParam(conn, "param", s))
		{
			if (s.find("water") != string::npos)
			{
				if (s.find("off") != string::npos)
				{
					while (radioInUse)
					{
						usleep(1000);
					} 
					radioInUse=true;
					if (boiler.TurnWaterOff())
					{
						radioInUse=false;
						mg_printf(conn, "<div class = \"heading\">Water turned off</div>\r\n");
						mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
					}
					else
					{
						radioInUse=false;
						mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
						mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
					}
				}
				else if (s.find("on") != string::npos)
				{
					if (std::string::npos != s.find_first_of("0123456789"))
					{
						string durationString = s.substr(7, 10);
						int duration = atoi(durationString.c_str());
						
						while (radioInUse)
						{
							usleep(1000);
						} 
						radioInUse=true;
						if (boiler.TurnWaterOn(duration))
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Water turned on for %i minutes</div>\r\n", duration);
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						else
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						
					}
					else
					{
						while (radioInUse)
						{
							usleep(1000);
						} 
						radioInUse=true;
						if (boiler.TurnWaterOn())
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Water turned on</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						else
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
					}
				}
			}

			else if (s.find("heating") != string::npos)
			{
				if (s.find("off") != string::npos)
				{
					while (radioInUse)
					{
						usleep(1000);
					} 
					radioInUse=true;
					if (boiler.TurnHeatingOff())
					{
						radioInUse=false;
						mg_printf(conn, "<div class = \"heading\">Heating turned off</div>\r\n");
						mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
					}
					else
					{
						radioInUse=false;
						mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
						mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
					}
				}
				else if (s.find("on") != string::npos)
				{
					if (std::string::npos != s.find_first_of("0123456789"))
					{
						string durationString = s.substr(9, 10);
						int duration = atoi(durationString.c_str());
						
						while (radioInUse)
						{
							usleep(1000);
						} 
						radioInUse=true;
						if (boiler.TurnHeatingOn(duration))
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Heating turned on for %i minutes</div>\r\n", duration);
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						else
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						
					}
					else
					{
						while (radioInUse)
						{
							usleep(1000);
						} 
						radioInUse=true;
						if (boiler.TurnHeatingOn())
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Heating turned on</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
						else
						{
							radioInUse=false;
							mg_printf(conn, "<div class = \"heading\">Unable to communicate with boiler</div>\r\n");
							mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
						}
					}
				}
			}
		}
		else 
		{
			mg_printf(conn, "<div class = \"heading\">param not set</p>");
			mg_printf(conn, "<div class=\"hhh\"><a href=\"hub\">Go back to hub</a></div>\r\n");
		}
		mg_printf(conn, "</body></html>\n");
		return true;
	}
};

// class ABHandler: public CivetHandler
// {
// public:
	// bool handleGet(CivetServer *server, struct mg_connection *conn) {
		
		// cout << "Sending payload to 03" << endl;
		// bool sent = sendPayload();
		// if (sent)
		// {
			// mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
			// mg_printf(conn, "<html><body>");
			// mg_printf(conn, "<h2>Payload sent</h2>");
			// mg_printf(conn, "</body></html>\n");
			// cout << "Sending successful" << endl;
		// }
		// else
		// {
			// mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
			// mg_printf(conn, "<html><body>");
			// mg_printf(conn, "<h2>Unable to send payload</h2>");
			// mg_printf(conn, "</body></html>\n");
			// cout << "Sending failed" << endl;
		// }
		
		// return true;
	// }
// };

// class FooHandler: public CivetHandler
// {
// public:
	// bool handleGet(CivetServer *server, struct mg_connection *conn) {
		// /* Handler may access the request info using mg_get_request_info */
		// struct mg_request_info * req_info = mg_get_request_info(conn);

		// mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
		// mg_printf(conn, "<html><body>");
		// mg_printf(conn, "<h2>This is the Foo handler!!!</h2>");
		// mg_printf(conn, "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>",
		// req_info->request_method, req_info->uri, req_info->http_version);
		// mg_printf(conn, "</body></html>\n");
		// return true;
	// }
// };


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void printCurrentTime()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	cout << (now->tm_hour) << ":"
	<< (now->tm_min);
}



using std::string;
using std::stringstream;

int8_t volatile keepRunning = 1;

// Pointer to Sqlite3 DB - used to access DB when open
sqlite3 *db = NULL;
// Path to DB file - same dir as this program's executable
const char *dbPath = "/var/www/sqlTemplog.db";

// DB Statement handle - used to run SQL statements
sqlite3_stmt *stmt = NULL;



int8_t recordTemp(int devID, double tempC) {
	const char *sql = "INSERT INTO temps( id, timestamp, temp) VALUES(?,datetime('now', 'localtime'), ?)";
	if(!sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)== SQLITE_OK)
	{
		fprintf(stderr, "MEhhdfsd: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_int(stmt, 1, devID);
	sqlite3_bind_double(stmt, 2, tempC);
	sqlite3_step(stmt);  // Run SQL INSERT
	sqlite3_reset(stmt); // Clear statement handle for next use
	return 0;
}	



void intHandler() {
	printf("\nStopping...\n");
	keepRunning = 0;
}






///////////////////////////////////////////////////////////////////////


bool sendPayload()
{
	//network.update();
	bool waiting = false;
	while (radioInUse)
	{
		if (!waiting)
		{
			waiting=true;
			cout << "SendPayload() waiting" << endl;
		}
		usleep(1000);
	} 
	radioInUse=true;
	cout << "Not waiting" << endl;
	payload_temp payload = { 6.4, 22, 4 };
	RF24NetworkHeader header(/*to node*/ 02);
	bool ok = network.write(header,&payload,sizeof(payload));
	radioInUse=false;
	if (ok)
	{
		return true;
	}
	else
	{ 
		return false;
	}
}


///////////////////////////////////////////////////////////////////////


void saveTempData(payload_temp &payload);
void saveDataToFile(ofstream &dataFile, float &battVolts, payload_temp &payload);



int main(int argc, char** argv) 
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	const char * options[] = { "document_root", DOCUMENT_ROOT,
		"listening_ports", PORT, "global_auth_file" , "./.htpasswd", 0
	};
	
	// const char * options[] = { "document_root", DOCUMENT_ROOT,
                               // "listening_ports", PORT, 0
                             // };

	CivetServer server(options);

	//HubHandler h_hub;
	//server.addHandler(HUB_URI, h_hub);
	server.addHandler(HUB_URI, new HubHandler());
	// server.addHandler(EXIT_URI, new ExitHandler());
	//BoilerHandler h_boiler;
	//server.addHandler(BOILER_URI, h_boiler);
	server.addHandler(BOILER_URI, new BoilerHandler());
	//server.addHandler("/boiler", h_boiler);
	// server.addHandler("/a/b", new ABHandler());
	// server.addHandler("**.foo$", new FooHandler());

	printf("Browse files at http://localhost:%s/\n", PORT);
	printf("Run example at http://localhost:%s%s\n", PORT, HUB_URI);
	printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	int rc = sqlite3_open(dbPath, &db);
	// If rc is not 0, there was an error
	if(rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	
	radio.begin();
	delay(5);
	network.begin( 90, this_node);
	radio.printDetails();
	
	while(keepRunning)
	{
		//std::cout << "In main loop" << std::endl;
		while (radioInUse)
		{
			//std::cout << "In waiting (main) loop" << std::endl;
			usleep(100000);
			//sleep(1);
		} 
		radioInUse=true;
		network.update();
		while ( network.available() ) 
		{			
			//std::cout << "In network (main) loop" << std::endl;
			RF24NetworkHeader header;
			network.peek (header);
			
			if (header.type == 'p')
			{
				payload_temp payload;
				network.read(header,&payload,sizeof(payload));
				//std::cout << "In header p loop" << std::endl;
			}
			else
			{
				payload_temp payload;
				network.read(header,&payload,sizeof(payload));
				

				//Save data to database/files
				saveTempData(payload);
			}
		}
		radioInUse=false;
		//sleep(1);
		usleep(100000);
	}
	sqlite3_close(db);
	return 0;
}


void saveTempData(payload_temp &payload)
{
	//Save temperature to database
	recordTemp(payload.deviceNum, payload.temp);
	
	//ofstream dataFile;
	//
	float battVolts=0;
	
	//Open correct file for saving data
	if (payload.deviceNum==1)
	{
		//f = fopen( "/home/pi/temp1.log", "a");
	}
	if (payload.deviceNum==2)
	{
		ofstream dataFile ("/home/pi/temp2.log", ios::out | ios::app);
		battVolts = payload.voltage * 0.001;
		saveDataToFile(dataFile, battVolts, payload);
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	if (payload.deviceNum==3)
	{
		//f = fopen( "/home/pi/temp3.log", "a");
	}
	if (payload.deviceNum==4)
	{
		//f = fopen( "/home/pi/temp4.log", "a");
	}
	if (payload.deviceNum==5)
	{
		//f = fopen( "/home/pi/temp5.log", "a");
	}
	if (payload.deviceNum==6)
	{
		//f = fopen( "/home/pi/temp6.log", "a");
	}
	
	//Write data to screen
	//	std::cout << "Sensor "<< payload.deviceNum << "temp: " << payload.temp;


}

void saveDataToFile(ofstream &dataFile, float &battVolts, payload_temp &payload)
{


	stringstream s; 
	s << fixed <<  setprecision(3) << payload.temp;

	cout << "Sensor " << payload.deviceNum 
	<< " temp: " << s.str() << " ";

	printCurrentTime();
	
	if (battVolts > 0)
	cout << " at " << battVolts << "V" << endl;
	else
	cout << endl;;
	
	
	//Work out current time
	timeval curTime;
	gettimeofday(&curTime, NULL);

	char TimeString[128];

	strftime(TimeString, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

	//char deviceNum[1];
	//sprintf(deviceNum, "%i", payload.deviceNum);
	//fprintf(f, deviceNum);
	//fprintf(f, ",");
	
	if (dataFile.is_open())
	{
		
		dataFile << payload.deviceNum << ","
		<< TimeString << ","
		<< payload.temp;

		//char temp[4];
		//sprintf(temp, "%f", payload.temp);
		//fprintf(f, TimeString);
		//fprintf(f, ",");
		//fprintf(f, temp);

		//dataFile 
		

		if (payload.voltage>0)
		{
			//fprintf(f, ",");
			//char volts[4];
			//sprintf(volts, "%f", battVolts);
			//fprintf(f, volts);
			dataFile << "," << battVolts;
		}
		dataFile << endl;
		//fprintf(f, "\r\n");
		
		dataFile.close();
		//fclose(f);
	}
	else cout << "Unable to open file" << endl;
}



static int callback(void *ptr, int argc, char* argv[], char* cols[])
{
	typedef std::vector<std::vector<std::string> > table_type;
    table_type* table = static_cast<table_type*>(ptr);
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++)
        row.push_back(argv[i] ? argv[i] : "(NULL)");
    table->push_back(row);
    return 0;
}
   
   
std::string printGraph()
{
sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char *sql;
	std::vector<std::vector<std::string> > table;
	
	rc = sqlite3_open("/var/www/sqlTemplog.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      //fprintf(stderr, "Opened database successfully\n");
   }
   
   
   sql = "SELECT timestamp, temp FROM temps WHERE timestamp>datetime('now', 'localtime', '-24 hours') AND id=2";

  
   rc = sqlite3_exec(db, sql, callback, &table, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   
   std::string somestuff;
   
   somestuff.append("<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n"
    "<script type=\"text/javascript\">\n"
      "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});\n"
      "google.setOnLoadCallback(drawChart);\n"
      "function drawChart() {\n"
        "var data = google.visualization.arrayToDataTable([\n"
          "['Time', 'Temperature'],\n");
   
   for (unsigned int i=0; i<table.size(); i++)
   {
		somestuff.append("['");
		somestuff.append(table[i][0].substr (11, 5));
		somestuff.append("',");
		somestuff.append(table[i][1]);
		if (i==table.size()-1)
			somestuff.append("]\n");
		else
			somestuff.append("],\n");
   }
   somestuff.append("]);\n"

       " var options = {\n"
       "   title: 'Temperature',\n curveType: 'function'\n"
       " };\n"

        "var chart = new google.visualization.LineChart(document.getElementById('linechart_material'));\n"
        "chart.draw(data, options);\n"
      "}\n"
    "</script>\n");
   return somestuff;
   
}
