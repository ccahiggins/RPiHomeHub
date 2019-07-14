ifeq ($(OS),Windows_NT)
	CC=C:\SysGCC\Raspberry\bin\arm-linux-gnueabihf-g++.exe -std=c++11
	BOOST=-I..\boost_1_65_1
	CIVET=-I..\civetweb\include
	JSON=-IP:..\json-2.1.1\src
	RF24NETWORK=-lrf24network
else
	CC=g++ -std=c++11
	BOOST=-I ../boost_1_65_1
	CIVET=-I../civetweb/include
	JSON=-I../json-2.1.1/src/
	RF24=-lrf24network -lrf24-bcm -I../rf24libs/RF24 -I../rf24libs/RF24Network -L/usr/local/lib
endif

INC=include/
BIN=bin/
LIB=lib/
OUT=build/
SRC=src/
CXXFLAGS_OLD=-Wall -lpthread -ldl -lrt
CXXFLAGS=-Ofast -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -mtune=cortex-a7 -Wall -lpthread -ldl -lrt -Iinclude
SQLITE=-lsqlite3
RF24CMB=-lrf24-bcm
CRYPTO=-lcrypto
UUID=-luuid
CIVLIBA=libcivetweb.a

all: $(BIN)hub
	
$(BIN)hub: $(OUT)Hub.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandler.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)JsonBoilerHandler.o $(OUT)JsonBoilerStatusHandler.o $(OUT)IftttHandler.o $(OUT)RequestValidator.o $(OUT)AuthHandler.o $(OUT)LoginHandler.o $(OUT)HomeHandler.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(CIVET) $(SQLITE) $(RF24CMB) $(RF24) $(LIB)$(CIVLIBA) $(CRYPTO) $(UUID)
	
$(OUT)Hub.o: $(SRC)Hub.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24)
	
$(OUT)TempSensorController.o: $(SRC)TempSensorController.cpp $(INC)TempSensorController.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OUT)HubHandler.o: $(SRC)HubHandler.cpp $(INC)HubHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)BoilerHandler.o: $(SRC)BoilerHandler.cpp $(INC)BoilerHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ReadHtml.o: $(SRC)ReadHtml.cpp $(INC)ReadHtml.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OUT)Boiler.o: $(SRC)Boiler.cpp $(INC)Boiler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OUT)RadioController.o: $(SRC)RadioController.cpp $(INC)RadioController.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(RF24)

$(OUT)Timer.o: $(SRC)Timer.cpp $(INC)Timer.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OUT)TimerHandler.o: $(SRC)TimerHandler.cpp $(INC)TimerHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)TimerAddHandler.o: $(SRC)TimerAddHandler.cpp $(INC)TimerAddHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)TimerDeleteHandler.o: $(SRC)TimerDeleteHandler.cpp $(INC)TimerDeleteHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)TimerEnableHandler.o: $(SRC)TimerEnableHandler.cpp $(INC)TimerEnableHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST) 

$(OUT)TimerDisableHandler.o: $(SRC)TimerDisableHandler.cpp $(INC)TimerDisableHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ChartHandler.o: $(SRC)ChartHandler.cpp $(INC)ChartHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ChartCreator.o: $(SRC)ChartCreator.cpp $(INC)ChartCreator.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET)
	
$(OUT)ChartCreatorJson.o: $(SRC)ChartCreatorJson.cpp $(INC)ChartCreatorJson.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET)	
	
$(OUT)VoltageHandler.o: $(SRC)VoltageHandler.cpp $(INC)VoltageHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)JsonBoilerHandler.o: $(SRC)JsonBoilerHandler.cpp $(INC)JsonBoilerHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON)

$(OUT)JsonBoilerStatusHandler.o: $(SRC)JsonBoilerStatusHandler.cpp $(INC)JsonBoilerStatusHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON)
	
$(OUT)RequestValidator.o: $(SRC)RequestValidator.cpp $(INC)RequestValidator.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)
	
$(OUT)IftttHandler.o: $(SRC)IftttHandler.cpp $(INC)IftttHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(JSON) $(SQLITE)
	
#$(OUT)TestHandler.o: $(SRC)TestHandler.cpp $(INC)TestHandler.hpp
#	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET)
	
$(OUT)AuthHandler.o: $(SRC)AuthHandler.cpp $(INC)AuthHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)LoginHandler.o: $(SRC)LoginHandler.cpp $(INC)LoginHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)HomeHandler.o: $(SRC)HomeHandler.cpp $(INC)HomeHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)
	
clean:
ifeq ($(OS),Windows_NT)
	del $(OUT)*.o
else
	rm $(OUT)*.o
endif


#$(OUT)JsonHubHandler.o: JsonHubHandler.cpp
#	$(CC) -c JsonHubHandler.cpp -o $(OUT)JsonHubHandler.o $(CXXFLAGS) $(CIVET) $(JSON)
#


#$(OUT)ChartHandlerTest2.o: $(SRC)ChartHandlerTest2.cpp $(INC)ChartHandlerTest2.hpp
#	$(CC) -c $(SRC)ChartHandlerTest2.cpp -o $(OUT)ChartHandlerTest2.o $(CXXFLAGS) $(CIVET)


#hub:$(OUT)HubNew.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandler.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandler.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
	
	
#hub: $(OUT)HubNew.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandlerTest.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandlerTest.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a

#hub: $(OUT)HubNew.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)JsonHubHandler.o $(OUT)JsonBoilerHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o
#	$(CC) -o hub $(OUT)HubNew.o $(OUT)RadioController.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)Boiler.o $(OUT)ReadHtml.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandlerTest.o $(OUT)ChartHandler2.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)ChartCreatorJson.o $(CXXFLAGS) $(CIVET) -lsqlite3 -lrf24-bcm  $(RF24NETWORK) libcivetweb.a
