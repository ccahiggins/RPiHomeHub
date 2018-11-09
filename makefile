ifeq ($(OS),Windows_NT)
	CC = C:\SysGCC\Raspberry\bin\arm-linux-gnueabihf-g++.exe -std=c++11
	BOOST = -I..\boost_1_65_1
	CIVET = -I..\civetweb\include
	JSON = -IP:..\json-2.1.1\src
	RF24NETWORK = -lrf24network
else
	CC = g++ -std=c++11
	BOOST = -I ../boost_1_65_1
	CIVET = -I../civetweb/include
	JSON = -I../json-2.1.1/src/
	RF24NETWORK = -lrf24network -lrf24-bcm -I../rf24libs/RF24 -I../rf24libs/RF24Network -L/usr/local/lib
endif

BIN=bin/
LIB=lib/
OUT=build/
SRC=src/
CXXFLAGS_TEST =  -Wall -lpthread -ldl -lrt
CXXFLAGS = -Ofast -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -mtune=cortex-a7 -Wall -lpthread -ldl -lrt -Iinclude
all: hub

#hub:$(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
	
#hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)ChartHandler.o $(OUT)JsonBoilerHandler.o $(OUT)JsonBoilerStatusHandler.o
	$(CC) -o $(BIN)hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)ChartHandler.o $(OUT)JsonBoilerHandler.o $(OUT)JsonBoilerStatusHandler.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) $(LIB)libcivetweb.a

#hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
	
	
$(OUT)HubNew.o: $(SRC)HubNew.cpp
	$(CC) -c $(SRC)HubNew.cpp -o $(OUT)HubNew.o $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24NETWORK)

$(OUT)TempsensorController.o: $(SRC)TempsensorController.cpp
	$(CC) -c $(SRC)TempsensorController.cpp -o $(OUT)TempsensorController.o $(CXXFLAGS)

$(OUT)HubHandler.o: $(SRC)HubHandler.cpp
	$(CC) -c $(SRC)HubHandler.cpp -o $(OUT)HubHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)
	

$(OUT)BoilerHandler.o: $(SRC)BoilerHandler.cpp
	$(CC) -c $(SRC)BoilerHandler.cpp -o $(OUT)BoilerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ReadHtml.o: $(SRC)ReadHtml.cpp
	$(CC) -c $(SRC)ReadHtml.cpp -o $(OUT)ReadHtml.o $(CXXFLAGS)

$(OUT)Boiler.o: $(SRC)Boiler.cpp
	$(CC) -c $(SRC)Boiler.cpp -o $(OUT)Boiler.o $(CXXFLAGS)

$(OUT)RadioController.o: $(SRC)RadioController.cpp
	$(CC) -c $(SRC)RadioController.cpp -o $(OUT)RadioController.o $(CXXFLAGS) $(RF24NETWORK)

$(OUT)Timer.o: $(SRC)Timer.cpp
	$(CC) -c $(SRC)Timer.cpp -o $(OUT)Timer.o $(CXXFLAGS)

$(OUT)TimersHandler.o: $(SRC)TimersHandler.cpp
	$(CC) -c $(SRC)TimersHandler.cpp -o $(OUT)TimersHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)AddTimerHandler.o: $(SRC)AddTimerHandler.cpp
	$(CC) -c $(SRC)AddTimerHandler.cpp -o $(OUT)AddTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)DisableTimerHandler.o: $(SRC)DisableTimerHandler.cpp
	$(CC) -c $(SRC)DisableTimerHandler.cpp -o $(OUT)DisableTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)EnableTimerHandler.o: $(SRC)EnableTimerHandler.cpp
	$(CC) -c $(SRC)EnableTimerHandler.cpp -o $(OUT)EnableTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST) $(BOOST)

#$(OUT)ChartHandler.o: ChartHandler.cpp
	#$(CC) -c ChartHandler.cpp -o $(OUT)ChartHandler.o $(CXXFLAGS) $(CIVET) 
	#$(BOOST)

$(OUT)ChartHandler2.o: $(SRC)ChartHandler2.cpp
	$(CC) -c $(SRC)ChartHandler2.cpp -o $(OUT)ChartHandler2.o $(CXXFLAGS) $(CIVET)	
	
$(OUT)ChartHandlerTest.o: $(SRC)ChartHandlerTest.cpp
	$(CC) -c $(SRC)ChartHandlerTest.cpp -o $(OUT)ChartHandlerTest.o $(CXXFLAGS) $(CIVET)
	
$(OUT)ChartHandlerTest2.o: $(SRC)ChartHandlerTest2.cpp
	$(CC) -c $(SRC)ChartHandlerTest2.cpp -o $(OUT)ChartHandlerTest2.o $(CXXFLAGS) $(CIVET)

$(OUT)ChartCreator.o: $(SRC)ChartCreator.cpp
	$(CC) -c $(SRC)ChartCreator.cpp -o $(OUT)ChartCreator.o $(CXXFLAGS) $(CIVET)
	
$(OUT)ChartCreatorJson.o: $(SRC)ChartCreatorJson.cpp
	$(CC) -c $(SRC)ChartCreatorJson.cpp -o $(OUT)ChartCreatorJson.o $(CXXFLAGS) $(CIVET)	
	
$(OUT)DeleteTimerHandler.o: $(SRC)DeleteTimerHandler.cpp
	$(CC) -c $(SRC)DeleteTimerHandler.cpp -o $(OUT)DeleteTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)VoltageHandler.o: $(SRC)VoltageHandler.cpp
	$(CC) -c $(SRC)VoltageHandler.cpp -o $(OUT)VoltageHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

#$(OUT)JsonHubHandler.o: JsonHubHandler.cpp
#	$(CC) -c JsonHubHandler.cpp -o $(OUT)JsonHubHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
#
$(OUT)JsonBoilerHandler.o: $(SRC)JsonBoilerHandler.cpp
	$(CC) -c $(SRC)JsonBoilerHandler.cpp -o $(OUT)JsonBoilerHandler.o $(CXXFLAGS) $(CIVET) $(JSON)

$(OUT)JsonBoilerStatusHandler.o: $(SRC)JsonBoilerStatusHandler.cpp
	$(CC) -c $(SRC)JsonBoilerStatusHandler.cpp -o $(OUT)JsonBoilerStatusHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
	
clean:
ifeq ($(OS),Windows_NT)
	del $(OUT)*.o
else
	rm $(OUT)*.o
endif
