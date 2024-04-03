// https://www.man7.org/linux/man-pages/man7/ip.7.html
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>


int main(){
	// This is my sockfd
	int conection_client = socket(AF_INET,SOCK_STREAM,0);
	if (conection_client != -1){
		// Se creo el socket
		sockaddr_in server_addres;
		server_addres.sin_family = AF_INET;
		server_addres.sin_port = htons(8000);
		server_addres.sin_addr.s_addr = htonl(0x7F000001);

		if(connect(conection_client, // sockfd
					(const sockaddr *)&server_addres, // sockaddr
					sizeof(server_addres)) != -1){ // addrlen
			// Se establecio la conexion
			std::string topic;
			topic = "test";
			std::string msg;
			msg = "hola";

			char control[14];
			control[0] = 0x10;// pacakge type
			control[1] = 0x0c;// flags
			control[2] = 0x00;// flags
			control[3] = 0x04;// flags
			control[4] = 'M';// flags
			control[5] = 'Q';// flags
			control[6] = 'T';// flags
			control[7] = 'T';// flags
			control[8] = 0x04;// flags
			control[9] = 0x02;// flags
			control[10] = 0x00;// flags
			control[11] = 0x3c;// flags
			control[12] = 0x00;// flags
			control[13] = 0x00;// flags

			/* if(send(conection_client,msg.c_str(),msg.size(),MSG_CONFIRM) == -1){ */
			if(send(conection_client,control,sizeof(control),MSG_CONFIRM) == -1){
				std::cout << "Error al mandar el mensaje";
				return 1;
			}

			// Aqui mandamos el topic
			char payload[12];
			payload[0] = 0x30;
			payload[1] = 0x0a;
			payload[2] = 0x00;
			payload[3] = 0x04;
			std::memcpy(payload + 4, topic.c_str(), topic.size());
			std::memcpy(payload + 8, msg.c_str(), msg.size());
												//
			if(send(conection_client,payload,sizeof(payload),MSG_CONFIRM) == -1){
				std::cout << "Error al mandar el mensaje";
				return 1;
			}

			char disconect[2];
			disconect[0] = 0xe0;
			disconect[1] = 0x00;
			if(send(conection_client,disconect,sizeof(disconect),MSG_CONFIRM) == -1){
				std::cout << "Error al mandar el mensaje";
				return 1;
			}

			shutdown(conection_client, SHUT_WR);
			// Si no pongo esto me causa un RST en wireshark
			sleep(1);
			close(conection_client);

		}else{
			std::cout << "Error al entablar conexion";
			return 1;
		}
	}else{
		std::cout << "Error al crear el socket";
		return 1;
	}
	return 0;
}
