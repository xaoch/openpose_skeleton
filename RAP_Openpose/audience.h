#pragma once
#include <windows.h>
#include <iostream>

namespace audience {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	bool audience_showing = false;
	bool video_recording = false;
	STARTUPINFO si_record;
	PROCESS_INFORMATION pi_record;

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		//Get process ID of the window
		DWORD dwProcID = 0;
		GetWindowThreadProcessId(hwnd, &dwProcID);
		if (dwProcID == lParam) {			
			//bool temp = IsWindow(hwnd); 
			//if (temp) {
				SendMessage(hwnd, WM_CLOSE, NULL, NULL);
				//return FALSE;
			//}
		}
		return TRUE;
	}

	void showAudience() {
		if (!audience_showing) {
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			// Start the child process. 
			if (!CreateProcess(NULL,   // No module name (use command line)
									   //	"C:\\Users\\CTI\\Documents\\RAP_OpenposeGRPC\\RAP_Openpose\\open_audience.bat",        // Command line
				"\"D:\\Program Files (x86)\\VideoLAN\\VLC\\vlc.exe\" C:\\Users\\CTI\\Desktop\\publico.mp4 --loop -f",
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
				return;
			}			
			audience_showing = true;			
		}
	}

	void recordVideo(std::string student_id) {		
		ZeroMemory(&si_record, sizeof(si_record));
		si_record.cb = sizeof(si_record);
		si_record.dwFlags = STARTF_USESHOWWINDOW;
		si_record.wShowWindow = SW_HIDE;
		ZeroMemory(&pi_record, sizeof(pi_record));
		std::string command = "\"D:\\Program Files (x86)\\VideoLAN\\VLC\\vlc.exe\"  dshow:// :dshow-vdev=\"Kinect V2 Video Sensor\" :dshow-adev=\"Microphone Array (Xbox NUI Sensor)\" :live-caching=300 :sout=#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}:file{dst=D:\\pruebas\\estudiante" + student_id + ".mp4,no-overwrite} :sout-keep";
		// Start the child process. 
		if (!CreateProcess(NULL,   // No module name (use command line)
								   //	"C:\\Users\\CTI\\Documents\\RAP_OpenposeGRPC\\RAP_Openpose\\open_audience.bat",        // Command line
			//"\"D:\\Program Files (x86)\\VideoLAN\\VLC\\vlc.exe\"  dshow:// :dshow-vdev=\"Kinect V2 Video Sensor\" :dshow-adev=\"Microphone Array (Xbox NUI Sensor)\" :live-caching=300 :sout=#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}:file{dst=D:\\pruebas\\estudiante.mp4,no-overwrite} :sout-keep",
			const_cast<char *>(command.c_str()),
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si_record,            // Pointer to STARTUPINFO structure
			&pi_record)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return;
		}
		Sleep(1000);
		video_recording = true;
	}
	void closeAudience() {
		// Close process and thread handles. 
		if (audience_showing) {
			EnumWindows(EnumWindowsProc, pi.dwProcessId);
			Sleep(2000);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			audience_showing = false;
		}
	}

	void stopRecording() {
		if (video_recording) {
			EnumWindows(EnumWindowsProc, pi_record.dwProcessId);
			Sleep(2000);
			CloseHandle(pi_record.hProcess);
			CloseHandle(pi_record.hThread);
			audience_showing = false;
		
		}
	}
}