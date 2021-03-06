// Client Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "include/rang.hpp"
#include "include/json.hpp"
#include "include/hab.hpp"

using json = nlohmann::json;
void openConfig(std::string);

std::ifstream confRead("config.json");
json config;

int main()
{
	confRead >> config;
	confRead.close();
	sf::TcpSocket socket;
	std::cout << rang::fgB::cyan << "== OCESS Phobos EECOM Test System ==" << std::endl;
	std::string conIP, inPort;
	int conPort = -1;
	std::stringstream convert(inPort);
	std::cout << rang::fg::cyan << "Server Host:  [" << config["ip"] << "] " << rang::fgB::cyan;
	std::getline(std::cin, conIP);
	std::cout << rang::fg::cyan << "Server Port:  [" << config["port"] << "] " << rang::fgB::cyan;
	std::getline(std::cin, inPort);
	if (conIP.empty())
		conIP = config["ip"];
	if (!(convert >> conPort))
		conPort = config["port"];
	std::cout << rang::fgB::gray << "Connecting to server" << std::endl;
	sf::Socket::Status status = socket.connect(conIP, conPort);
	if (status != sf::Socket::Done)
	{
		// error...
		return 1;
	}
	config["ip"] = conIP;
	config["port"] = conPort;
	std::ofstream confWrite("config.json");
	confWrite << config;
	confWrite.close();
	std::cout << "Connected! Sending packet..." << std::endl;
	sf::Uint32 x = 24;
	std::string s = "hello";
	double d = 5.89;
	// Group the variables to send into a packet
	sf::Packet packet;
	packet << "phobos" << x << s << d;
	// Send it over the network (socket is a valid sf::TcpSocket)
	socket.send(packet);
	std::cout << "Sent!" << std::endl;

	Habitat hab;
	sf::Image icon;
	icon.loadFromFile("img/transparent.png");
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("Montserrat-Regular.ttf"))
	{
		std::cerr << "Font not found" << std::endl;
		return 1;
	}
	text.setFont(font);

	// set the string to display
	text.setString("OCESS Habitat");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

							   // set the color
	text.setFillColor(sf::Color::White);


	sf::RenderWindow window(sf::VideoMode(800, 600), "Phobos EECOM Development Build");
	window.setIcon(750, 750, icon.getPixelsPtr());
	// run the program as long as the window is open
	int habPos[2] = { 10, 45 };
	text.setPosition(habPos[0] - 4, 8);
	sf::RectangleShape outWalls(sf::Vector2f(175, 250));
	outWalls.setFillColor(sf::Color(0, 0, 0));
	outWalls.setOutlineThickness(3);
	outWalls.setOutlineColor(sf::Color(255, 255, 255));
	outWalls.setPosition(habPos[0], habPos[1]);
	sf::RectangleShape vWalls(sf::Vector2f(3, 250));
	vWalls.setPosition(55, 10);
	sf::RectangleShape hWalls(sf::Vector2f(130, 3));
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		
		window.draw(outWalls);
		vWalls.setSize(sf::Vector2f(3, 250));
		vWalls.setPosition(habPos[0] + 45, habPos[1]);
		window.draw(vWalls);
		vWalls.setSize(sf::Vector2f(3, 125));
		vWalls.setPosition(habPos[0] + 140, habPos[1] + 55);
		window.draw(vWalls);
		hWalls.setSize(sf::Vector2f(130, 3));
		hWalls.setPosition(habPos[0] + 45, habPos[1] + 55);
		window.draw(hWalls);
		hWalls.setPosition(habPos[0] + 45, habPos[1] + 180);
		window.draw(hWalls);
		hWalls.setSize(sf::Vector2f(35, 3));
		hWalls.setPosition(habPos[0] + 140, habPos[1] + 130);
		window.draw(hWalls);

		window.draw(text);

		window.display();
	}

	return 0;
}