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
	OUT=out/
endif

CXXFLAGS_TEST =  -Wall -lpthread -ldl -lrt
CXXFLAGS = -Ofast -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -mtune=cortex-a7 -Wall  -lpthread -ldl -lrt
all: hub

#hub:$(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandler.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
	
#hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)JsonBoilerHandler.o $(OUT)JsonBoilerStatusHandler.o $(OUT)TestHandler.o
	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)JsonBoilerHandler.o $(OUT)JsonBoilerStatusHandler.o $(OUT)TestHandler.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a

#hub: $(OUT)HubNew.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempsensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimersHandler.o $(OUT)AddTimerHandler.o $(OUT)DisableTimerHandler.o $(OUT)EnableTimerHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)DeleteTimerHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
	
	
$(OUT)HubNew.o: HubNew.cpp
	$(CC) -c HubNew.cpp -o $(OUT)HubNew.o $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24NETWORK)

$(OUT)TempsensorController.o: TempsensorController.cpp
	$(CC) -c TempsensorController.cpp -o $(OUT)TempsensorController.o $(CXXFLAGS)

$(OUT)HubHandler.o: HubHandler.cpp
	$(CC) -c HubHandler.cpp -o $(OUT)HubHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)
	

$(OUT)BoilerHandler.o: BoilerHandler.cpp
	$(CC) -c BoilerHandler.cpp -o $(OUT)BoilerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ReadHtml.o: ReadHtml.cpp
	$(CC) -c ReadHtml.cpp -o $(OUT)ReadHtml.o $(CXXFLAGS)

$(OUT)Boiler.o: Boiler.cpp
	$(CC) -c Boiler.cpp -o $(OUT)Boiler.o $(CXXFLAGS)

$(OUT)RadioController.o: RadioController.cpp
	$(CC) -c RadioController.cpp -o $(OUT)RadioController.o $(CXXFLAGS) $(RF24NETWORK)

$(OUT)Timer.o: Timer.cpp
	$(CC) -c Timer.cpp -o $(OUT)Timer.o $(CXXFLAGS)

$(OUT)TimersHandler.o: TimersHandler.cpp
	$(CC) -c TimersHandler.cpp -o $(OUT)TimersHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)AddTimerHandler.o: AddTimerHandler.cpp
	$(CC) -c AddTimerHandler.cpp -o $(OUT)AddTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)DisableTimerHandler.o: DisableTimerHandler.cpp
	$(CC) -c DisableTimerHandler.cpp -o $(OUT)DisableTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)EnableTimerHandler.o: EnableTimerHandler.cpp
	$(CC) -c EnableTimerHandler.cpp -o $(OUT)EnableTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST) $(BOOST)

#$(OUT)ChartHandler.o: ChartHandler.cpp
	#$(CC) -c ChartHandler.cpp -o $(OUT)ChartHandler.o $(CXXFLAGS) $(CIVET) 
	#$(BOOST)

$(OUT)ChartHandler2.o: ChartHandler2.cpp
	$(CC) -c ChartHandler2.cpp -o $(OUT)ChartHandler2.o $(CXXFLAGS) $(CIVET)	
	
$(OUT)ChartHandlerTest.o: ChartHandlerTest.cpp
	$(CC) -c ChartHandlerTest.cpp -o $(OUT)ChartHandlerTest.o $(CXXFLAGS) $(CIVET)
	
$(OUT)ChartHandlerTest2.o: ChartHandlerTest2.cpp
	$(CC) -c ChartHandlerTest2.cpp -o $(OUT)ChartHandlerTest2.o $(CXXFLAGS) $(CIVET)

$(OUT)ChartCreator.o: ChartCreator.cpp
	$(CC) -c ChartCreator.cpp -o $(OUT)ChartCreator.o $(CXXFLAGS) $(CIVET)
	
$(OUT)ChartCreatorJson.o: ChartCreatorJson.cpp
	$(CC) -c ChartCreatorJson.cpp -o $(OUT)ChartCreatorJson.o $(CXXFLAGS) $(CIVET)	
	
$(OUT)DeleteTimerHandler.o: DeleteTimerHandler.cpp
	$(CC) -c DeleteTimerHandler.cpp -o $(OUT)DeleteTimerHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)VoltageHandler.o: VoltageHandler.cpp
	$(CC) -c VoltageHandler.cpp -o $(OUT)VoltageHandler.o $(CXXFLAGS) $(CIVET) $(BOOST)

#$(OUT)JsonHubHandler.o: JsonHubHandler.cpp
#	$(CC) -c JsonHubHandler.cpp -o $(OUT)JsonHubHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
#
$(OUT)JsonBoilerHandler.o: JsonBoilerHandler.cpp
	$(CC) -c JsonBoilerHandler.cpp -o $(OUT)JsonBoilerHandler.o $(CXXFLAGS) $(CIVET) $(JSON)

$(OUT)JsonBoilerStatusHandler.o: JsonBoilerStatusHandler.cpp
	$(CC) -c JsonBoilerStatusHandler.cpp -o $(OUT)JsonBoilerStatusHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
	
$(OUT)TestHandler.o: TestHandler.cpp
	$(CC) -c TestHandler.cpp -o $(OUT)TestHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
	
clean:
ifeq ($(OS),Windows_NT)
	del *.o
else
	rm out/*.o
endif
