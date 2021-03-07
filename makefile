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

all: $(BIN)start_hub
	
$(BIN)start_hub: $(OUT)HubMain.o $(OUT)HubClass.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandler.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)IftttHandler.o $(OUT)RequestValidator.o $(OUT)AuthHandler.o $(OUT)LoginHandler.o $(OUT)HomeHandler.o $(OUT)Thermostat.o $(OUT)ThermostatHandler.o $(OUT)Subject.o $(OUT)Emailer.o $(OUT)EmailerHandler.o $(OUT)TimerEvent.o $(OUT)BoilerTimerEvent.o $(OUT)ThermostatTimerEvent.o $(OUT)Sensors.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(CIVET) $(SQLITE) $(RF24CMB) $(RF24) $(LIB)$(CIVLIBA) $(CRYPTO) $(UUID)
	
$(OUT)HubMain.o: $(SRC)HubMain.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24)
	
$(OUT)HubClass.o: $(SRC)HubClass.cpp
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

$(OUT)Thermostat.o: $(SRC)Thermostat.cpp $(INC)Thermostat.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OUT)ThermostatHandler.o: $(SRC)ThermostatHandler.cpp $(INC)ThermostatHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

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
	
$(OUT)VoltageHandler.o: $(SRC)VoltageHandler.cpp $(INC)VoltageHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)RequestValidator.o: $(SRC)RequestValidator.cpp $(INC)RequestValidator.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)
	
$(OUT)IftttHandler.o: $(SRC)IftttHandler.cpp $(INC)IftttHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(JSON) $(SQLITE)
	
$(OUT)AuthHandler.o: $(SRC)AuthHandler.cpp $(INC)AuthHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)LoginHandler.o: $(SRC)LoginHandler.cpp $(INC)LoginHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)
	
$(OUT)HomeHandler.o: $(SRC)HomeHandler.cpp $(INC)HomeHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)Subject.o: $(SRC)Subject.cpp $(INC)Subject.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)

$(OUT)Emailer.o: $(SRC)Emailer.cpp $(INC)Emailer.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)

$(OUT)EmailerHandler.o: $(SRC)EmailerHandler.cpp $(INC)EmailerHandler.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)TimerEvent.o: $(SRC)TimerEvent.cpp $(INC)TimerEvent.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)

$(OUT)BoilerTimerEvent.o: $(SRC)BoilerTimerEvent.cpp $(INC)BoilerTimerEvent.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)

$(OUT)ThermostatTimerEvent.o: $(SRC)ThermostatTimerEvent.cpp $(INC)ThermostatTimerEvent.hpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)
	
$(OUT)Sensors.o: $(SRC)Sensors.cpp $(INC)Sensors.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)
	
clean:
ifeq ($(OS),Windows_NT)
	del $(OUT)*.o
else
	rm $(OUT)*.o
endif