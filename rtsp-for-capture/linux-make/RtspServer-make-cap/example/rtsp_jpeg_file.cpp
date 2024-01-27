// RTSP Server

#include "xop/RtspServer.h"
#include "net/Timer.h"
#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include "JPEGSource.h"
#include <opencv2/opencv.hpp>
#include "capture.h"
#include "JPEGSource.h"

void SendFrameThread(xop::RtspServer* rtsp_server, xop::MediaSessionId session_id, CameraCapture* cap);

int main(int argc, char **argv)
{	
	//if(argc != 2) {
	//	printf("Usage: %s test.JPEG \n", argv[0]);
	//	return 0;
	//}
	//JPEGFile JPEG_file;
	//if(!JPEG_file.Open(argv[1])) {
	//	printf("Open %s failed.\n", argv[1]);
	//	return 0;
	//}

	CameraCapture cap;
	if(!cap.open(0)) {
		printf("Open capture failed.\n");
		return 0;
	}


	std::string suffix = "live";
	std::string ip = "192.168.75.128";
	std::string port = "8554";
	std::string rtsp_url = "rtsp://" + ip + ":" + port + "/" + suffix;
	
	std::shared_ptr<xop::EventLoop> event_loop(new xop::EventLoop());
	std::shared_ptr<xop::RtspServer> server = xop::RtspServer::Create(event_loop.get());

	if (!server->Start("0.0.0.0", atoi(port.c_str()))) {
		printf("RTSP Server listen on %s failed.\n", port.c_str());
		return 0;
	}

#ifdef AUTH_CONFIG
	server->SetAuthConfig("-_-", "admin", "12345");
#endif

	xop::MediaSession *session = xop::MediaSession::CreateNew("live"); 
	session->AddSource(xop::channel_0, xop::JPEGSource::CreateNew()); 
	//session->StartMulticast(); 
	session->AddNotifyConnectedCallback([] (xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port){
		printf("RTSP client connect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
	});
   
	session->AddNotifyDisconnectedCallback([](xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port) {
		printf("RTSP client disconnect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
	});

	xop::MediaSessionId session_id = server->AddSession(session);
         
	std::thread t1(SendFrameThread, server.get(), session_id, &cap);
	t1.detach(); 

	std::cout << "Play URL: " << rtsp_url << std::endl;

	while (1) {
		xop::Timer::Sleep(30);
	}

	getchar();
	return 0;
}

void SendFrameThread(xop::RtspServer* rtsp_server, xop::MediaSessionId session_id, CameraCapture* cap)
{       
	int buf_size = 2000000;
	std::unique_ptr<uint8_t> frame_buf(new uint8_t[buf_size]);

	while(1) {
		int frame_size = cap->ReadFrame((char*)frame_buf.get());
		if(frame_size > 0) {
			xop::AVFrame videoFrame = {0};
			videoFrame.type = 0; 
			videoFrame.size = frame_size;
			videoFrame.timestamp = xop::JPEGSource::GetTimestamp();
			videoFrame.buffer.reset(new uint8_t[videoFrame.size]);    
			memcpy(videoFrame.buffer.get(), frame_buf.get(), videoFrame.size);
			rtsp_server->PushFrame(session_id, xop::channel_0, videoFrame);
		}
		else {
			break;
		}
		xop::Timer::Sleep(20); 
	};
}

