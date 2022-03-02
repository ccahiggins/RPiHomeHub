class LightsOff: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn) {
		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
		string param;
		mg_printf(conn, "<html><head>");

		mg_printf(conn, "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1\">");
		mg_printf(conn, "<link rel=\"stylesheet\" href=\"/styles.css\" type=\"text/css\" media=\"screen\" />");
	
		mg_printf(conn, "</head>");
        mg_printf(conn, "<body>");
		
		while (radioInUse)
		{
			//std::cout << "Hub handler waiting" << std::endl;
			usleep(1000);
		} 
		radioInUse=true;

		
		payload_lights payload = { 0};
		RF24NetworkHeader header(/*to node*/ 013);
		bool ok = network.write(header,&payload,sizeof(payload));
		radioInUse=false;
		if (ok)
		{
			mg_printf(conn, "Lights turned off");
		}
		else
		{
			mg_printf(conn, "Can't connect");
		}
		
		while (radioInUse)
		{
			//std::cout << "Hub handler waiting" << std::endl;
			usleep(1000);
		} 
		radioInUse=true;

		
		RF24NetworkHeader header2(/*to node*/ 013, 'p');
		ok = network.write(header2,&payload,sizeof(payload));
		radioInUse=false;
		if (ok)
		{
			mg_printf(conn, "Lights turned off - P");
		}
		else
		{
			mg_printf(conn, "Can't connect - P");
		}
		
				while (radioInUse)
		{
			//std::cout << "Hub handler waiting" << std::endl;
			usleep(1000);
		} 
		radioInUse=true;

		
		RF24NetworkHeader header3(/*to node*/ 013, 5);
		ok = network.write(header3,&payload,sizeof(payload));
		radioInUse=false;
		if (ok)
		{
			mg_printf(conn, "Lights turned off - 5");
		}
		else
		{
			mg_printf(conn, "Can't connect - 5");
		}
		mg_printf(conn, "</body></html>\r\n");
		return true;
	}
};