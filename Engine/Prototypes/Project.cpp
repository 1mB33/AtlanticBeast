#include "ProjectHeader.hpp"

#include <iostream>

void Application::Initialize()
{
    ::std::cout << "Initialize Application" << ::std::endl;
}

void Application::Update( ComponentBridge &bridge, float fDelta )
{
    ::std::cout << "I'm alive: Application" << ::std::endl;
    ::std::cout << "Quering component: ";
    bridge.QueryComponent<Gamemode>().DoGamemodeThing();
}

void Application::Destroy()
{
    ::std::cout << "Destroy Application" << ::std::endl;
}

void Renderer::Initialize()
{
    ::std::cout << "Initialize Renderer" << ::std::endl;
}

void Renderer::Update( ComponentBridge &bridge, float fDelta )
{
    ::std::cout << "I'm alive: Renderer" << ::std::endl;
}

void Renderer::Destroy()
{
    ::std::cout << "Destroy Renderer" << ::std::endl;
}

void Gamemode::Initialize()
{
    ::std::cout << "Initialize Gamemode" << ::std::endl;
}

void Gamemode::Update( ComponentBridge &bridge, float fDelta )
{
    ::std::cout << "I'm alive: Gamemode" << ::std::endl;
}

void Gamemode::Destroy()
{
    ::std::cout << "Destroy Gamemode" << ::std::endl;
}

void Gamemode::DoGamemodeThing()
{
    ::std::cout << "Doing something Gamemode " << ++i << ::std::endl;
}
