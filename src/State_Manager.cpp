#include "State_Manager.h"
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

using namespace cocos2d;

Scene* State_Manager::createScene() {
	//auto release objects
	auto scene = Scene::create();
    auto layer = State_Manager::create();
	scene->addChild(layer);

    return scene;
}

bool State_Manager::init() {
	if (!Layer::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	label = LabelBMFont::create("", "fonts/lucida.fnt");
	label->setString("abcdefghijklmnopqrstuvwxyz");
	label->setPosition(200, 200);
	addChild(label, 1);

	scheduleUpdate();
	init_time = clock();

	WSAData wsa_data;
	SOCKET conn_sock = INVALID_SOCKET;
	struct addrinfo* result, hints;
	int err;
	char* ip = "104.236.253.123";
	char* port = "4222";

	if ((err = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		CCLOG("WSA Startup failed! Err: %d", err);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if ((err = getaddrinfo(ip, port, &hints, &result)) != 0) {
		CCLOG("getaddrinfo failed (ip: %s, port: %s). Err %d", ip, port, err);
	}

	while (result != NULL) {
		if ((conn_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
			CCLOG("connection socket failed with error: %d", err);
			break;
		}
		if ((err = connect(conn_sock, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
			closesocket(conn_sock);
			conn_sock = INVALID_SOCKET;
			continue;
		}
		result = result->ai_next;
	}
	freeaddrinfo(result);

	if (conn_sock == INVALID_SOCKET) {
		CCLOG("could not connect to server. err: %d", err);
	}

	char* buffer = "ayy lmao";
	send(conn_sock, buffer, strlen(buffer), 0);

    return true;
}

void State_Manager::update(float dt) {
	time_since_startup = (clock() - init_time) / 1000.0f;
	label->setPosition(Vec2((cos(time_since_startup) * 40.0f) + 250, 200));

	for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setRotation(s->getRotation() + cos(s->getPosition().x));
	}

	for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setPosition((n * 25), s->getPosition().y);
	}
}

void State_Manager::menu_close(Ref* r) {
	Director::getInstance()->end();
}
