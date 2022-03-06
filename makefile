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
	CPR=-I../cpr/include -lcurl
	#CPR_CPP=../cpr/cpr/auth.cpp ../cpr/cpr/bearer.cpp ../cpr/cpr/cookies.cpp ../cpr/cpr/cprtypes.cpp ../cpr/cpr/curl_container.cpp ../cpr/cpr/error.cpp ../cpr/cpr/multipart.cpp ../cpr/cpr/parameters.cpp ../cpr/cpr/payload.cpp ../cpr/cpr/proxies.cpp ../cpr/cpr/session.cpp ../cpr/cpr/timeout.cpp ../cpr/cpr/unix_socket.cpp ../cpr/cpr/util.cpp
	#CPR_CPP= ../cpr/cpr/ ../cpr/cpr/ ../cpr/cpr/ ../cpr/cpr/ 
	CPR_CPP=../cpr/cpr/
	CPR_H=../cpr/include/cpr/
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
SQLLL=-I../sqlite-amalgamation-3380000/

all: $(BIN)start_hub
	
$(BIN)start_hub: $(OUT)HubMain.o $(OUT)HubClass.o $(OUT)TempSensorController.o $(OUT)HubHandler.o $(OUT)BoilerHandler.o $(OUT)ReadHtml.o $(OUT)Boiler.o $(OUT)RadioController.o $(OUT)Timer.o $(OUT)TimerHandler.o $(OUT)TimerAddHandler.o $(OUT)TimerDisableHandler.o $(OUT)TimerEnableHandler.o $(OUT)ChartHandler.o $(OUT)TimerDeleteHandler.o $(OUT)VoltageHandler.o $(OUT)ChartCreator.o $(OUT)IftttHandler.o $(OUT)RequestValidator.o $(OUT)AuthHandler.o $(OUT)LoginHandler.o $(OUT)HomeHandler.o $(OUT)Thermostat.o $(OUT)ThermostatHandler.o $(OUT)Subject.o $(OUT)Emailer.o $(OUT)EmailerHandler.o $(OUT)TimerEvent.o $(OUT)BoilerTimerEvent.o $(OUT)ThermostatTimerEvent.o $(OUT)Sensors.o $(OUT)DatabaseController.o $(OUT)Sonoff.o $(OUT)SonoffHandler.o $(OUT)CprResponse.o $(OUT)CprCurlholder.o $(OUT)CprAuth.o $(OUT)CprSession.o $(OUT)CprBearer.o $(OUT)CprCookies.o $(OUT)CprCprtypes.o $(OUT)CprError.o $(OUT)CprMultipart.o $(OUT)CprParameters.o $(OUT)CprPayload.o $(OUT)CprProxies.o $(OUT)CprTimeout.o $(OUT)CprUnixSocket.o $(OUT)CprUtil.o $(OUT)CprCurlContainer.o $(OUT)ChartJsonHandler.o $(OUT)ChartCreatorJson.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(CIVET) $(SQLITE) $(RF24CMB) $(RF24) $(LIB)$(CIVLIBA) $(CRYPTO) $(UUID) $(CPR)
	
$(OUT)HubMain.o: $(SRC)HubMain.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24) $(CPR)
	
$(OUT)HubClass.o: $(SRC)HubClass.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(JSON) $(BOOST) $(RF24) $(CPR)

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
	$(CC) -o $@ -c $<  $(CXXFLAGS) $(CIVET) $(JSON) $(SQLITE) $(CPR)
	
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
	
$(OUT)DatabaseController.o: $(SRC)DatabaseController.cpp $(INC)DatabaseController.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(SQLLL)
	
$(OUT)Sonoff.o: $(SRC)Sonoff.cpp $(INC)Sonoff.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)SonoffHandler.o: $(SRC)SonoffHandler.cpp $(INC)SonoffHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST) $(CPR)
	
$(OUT)CprResponse.o: $(CPR_CPP)response.cpp $(CPR_H)response.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)

$(OUT)CprCurlholder.o: $(CPR_CPP)curlholder.cpp $(CPR_H)curlholder.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprAuth.o: $(CPR_CPP)auth.cpp $(CPR_H)auth.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprSession.o: $(CPR_CPP)session.cpp $(CPR_H)session.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)

$(OUT)CprBearer.o: $(CPR_CPP)bearer.cpp $(CPR_H)bearer.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprCookies.o: $(CPR_CPP)cookies.cpp $(CPR_H)cookies.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprCprtypes.o: $(CPR_CPP)cprtypes.cpp $(CPR_H)cprtypes.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprError.o: $(CPR_CPP)error.cpp $(CPR_H)error.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)

$(OUT)CprMultipart.o: $(CPR_CPP)multipart.cpp $(CPR_H)multipart.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprParameters.o: $(CPR_CPP)parameters.cpp $(CPR_H)parameters.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprPayload.o: $(CPR_CPP)payload.cpp $(CPR_H)payload.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprProxies.o: $(CPR_CPP)proxies.cpp $(CPR_H)proxies.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprTimeout.o: $(CPR_CPP)timeout.cpp $(CPR_H)timeout.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprUnixSocket.o: $(CPR_CPP)unix_socket.cpp $(CPR_H)unix_socket.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)
	
$(OUT)CprUtil.o: $(CPR_CPP)util.cpp $(CPR_H)util.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)

$(OUT)CprCurlContainer.o: $(CPR_CPP)curl_container.cpp $(CPR_H)curl_container.h
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CPR)

$(OUT)ChartJsonHandler.o: $(SRC)ChartJsonHandler.cpp $(INC)ChartJsonHandler.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET) $(BOOST)

$(OUT)ChartCreatorJson.o: $(SRC)ChartCreatorJson.cpp $(INC)ChartCreatorJson.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(CIVET)

clean:
ifeq ($(OS),Windows_NT)
	del $(OUT)*.o
else
	rm $(OUT)*.o
endif