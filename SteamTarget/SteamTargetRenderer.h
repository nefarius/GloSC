/*
Copyright 2016 Peter Repukat - FlatspotSoftware

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once

#include <Windows.h>
#include <dwmapi.h>

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <thread>

#include <QApplication>
#include <QTimer>
#include <QProcess>
#include <QBuffer>
#include <QDatastream>
#include <QSharedmemory>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <psapi.h>

#include "VirtualControllerThread.h"
#include <atomic>

class SteamTargetRenderer : public QApplication
{
	Q_OBJECT

public:
	SteamTargetRenderer(int& argc, char** argv);
	~SteamTargetRenderer();

	void run();




private:
	void stop();
	void getSteamOverlay();
	void RunSfWindowLoop();
	static void makeSfWindowTransparent(sf::RenderWindow& window);
	void drawDebugEdges();
	void hookBindings() const;

	void loadLogo();

	static LRESULT WINAPI HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

	static void unhookBindings();

	static BOOL WINAPI ConsoleCtrlCallback(_In_ DWORD dwCtrlType);

	std::atomic<bool> bRunLoop = true;

	bool bUseDesktopConfig = false;
	bool bDrawDebugEdges = false;
	bool bDrawOverlay = true;
	bool bVsync = false;
	int iRefreshRate = 30;
	sf::CircleShape sfCshape;
	sf::RenderWindow sfWindow;

	std::thread renderThread;

	HWND consoleHwnd;

	HMODULE hmodGameOverlayRenderer = nullptr;
#ifdef _AMD64_
	WCHAR* overlayModuleName = L"GameOverlayRenderer64.dll";
#else
	WCHAR* overlayModuleName = L"GameOverlayRenderer.dll";
#endif
	static std::atomic<bool> overlayOpen;
	static HHOOK hook;

	HWND hwForeGroundWindow = nullptr;
	bool bNeedFocusSwitch = false;

	VirtualControllerThread controllerThread;

	bool bEnableControllers = true;

	static std::atomic<bool> bHookSteam;

	QTimer updateTimer;

	std::unique_ptr<sf::Texture> spriteTexture;
	sf::Sprite backgroundSprite;

	const QString LaunchGame = "LaunchGame";
	const QString LaunchedProcessFinished = "LaunchedProcessFinished";
	const QString IsSteamHooked = "IsSteamHooked";
	const QStringList defaultSharedMemData = QStringList()
		<< LaunchGame
		<< ""
		<< ""
		<< LaunchedProcessFinished
		<< "0"
		<< IsSteamHooked
		<< "-1";

private slots:
	void launchApp();
	void checkSharedMem();
};

